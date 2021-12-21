#include <ConnectServo.h>

ServoMessenger ConnectMessenger;

// This was a helper fucntion for ConnectLib, but for whatever reason it turned
// out to be rather slow. We now reference ServoMessenger ConnectMessenger as
// extern in Connect.h, and invoke ConnectMessenger.updateServo() in connectLoop().
void updateConnectServos() {
    ConnectMessenger.updateServos();
}

// Initialize the servo object, passing an initializer list to the cppQueue object
// See http://arduinoetcetera.blogspot.com/2011/01/classes-within-classes-initialiser.html
ConnectServo::ConnectServo() : _servoQueue(sizeof(ServoQueueItem), QUEUE_SIZE_ITEMS, IMPLEMENTATION) {
    // Set some sane defaults
    _goHomeWhenIdle = false;
    _isHome = false;
    _homeSpeed = 15;
    _homePosition= 90;
};

void ConnectServo::setPin(uint8_t pin) {
    _servoPin = pin;
    attach(_servoPin);
    write(_homePosition);
    registerServo();
    _servoAttached = true;
}

uint8_t ConnectServo::getPin() {
    return _servoPin;
}

void ConnectServo::registerServo() {
    ConnectMessenger.registerServo(this, _servoPin);
}

void ConnectServo::queueEaseTo(uint8_t newParam1, uint8_t newAnimationType, uint16_t newServoSpeed) {
    ServoQueueItem item;
    item.assign(STARTEASETO, newParam1, newAnimationType, newServoSpeed, NULL);
    _servoQueue.push(&item);
};

void ConnectServo::queueMoveTo(uint8_t newParam1) {
    ServoQueueItem item;
    // Can do this either with write() or a maximum-rate startEaseTo().
    item.assign(WRITE, newParam1, NULL, NULL, NULL);
    // item.assign(STARTEASETO, newParam1, EASE_LINEAR, 1023, NULL);
    _servoQueue.push(&item);
    // Queue a short pause so there's somet movement.
    item.assign(WAIT, NULL, NULL, SERVO_MOVE_SLEW_WAIT, NULL);
    _servoQueue.push(&item);
};

void ConnectServo::queueWaitForServo(uint8_t targetServoID) {
    ServoQueueItem item;
    item.assign(WAIT_FOR_SERVO, NULL, NULL, NULL, targetServoID);
    _servoQueue.push(&item);
};

void ConnectServo::queueMessageServo(uint8_t targetServo) {
    ServoQueueItem item;
    item.assign(MESSAGE_SERVO, NULL, NULL, NULL, targetServo);
    _servoQueue.push(&item);
};

void ConnectServo::unblockFromServo(uint8_t signallingServoID) {
    if (signallingServoID == _waitingForServo) {
        // Signal received from blocking object
        Serial.print(F("Servo on pin: "));
        Serial.print(_servoPin);
        Serial.println(F(": BLOCK LIFTED"));
        _waitingForServo = false;
    }
}

void ConnectServo::queueWait(uint16_t waitTime) {
    ServoQueueItem item;
    item.assign(WAIT, NULL, NULL, waitTime, NULL);
    _servoQueue.push(&item);
};

ServoQueueItem ConnectServo::dequeue() {
    ServoQueueItem _item;
    _servoQueue.pop(&_item);
    return _item;
};

void ConnectServo::setHome(uint8_t homePosition) {
    // Set the defined home position, and enable go home movement (implicit in the set)
    _homePosition = homePosition;
    _goHomeWhenIdle = true;
};

void ConnectServo::checkTime() {
    // TODO: This is pretty gnarly, and conflates WAIT queued actions with
    //       servo timeout handling. Should probably refactor.
    // Check if we're explicitly in a pause
    if (_waitingForTime) {
        if (millis() > _targetTime) {
            _waitingForTime = false;
        }
    } else if (_emptiedQueue && _servoAttached && !_isHome && _goHomeWhenIdle && !_goingHome) {
        // First pass through, we're looking to go home.
        if (millis() > _lastUpdate + SERVO_TIMEOUT_MS) {
            // If we're not in a pause, and the queue is empty, and we're not home,
            // and we're supposed to go home when idle, go home.
            Serial.print(F("Servo on pin: "));
            Serial.print(_servoPin);
            Serial.println(F(": TIMEOUT, going home"));
            // We can move directly home; going via queue gets us in an _isHome loop...
            setEasingType(EASE_CUBIC_IN_OUT);
            startEaseTo(_homePosition, _homeSpeed);
            // ...and set flag to show we're moving home
            _goingHome = true;
        }
    } else if (_goHomeWhenIdle && _goingHome) {
        // check if we're still going home
        if (!isMovingAndCallYield()) {
            // We've arrived home, so set flags accordingly
            _goingHome = false;
            _isHome = true;
            _lastUpdate = millis();
        }
    } else if (_emptiedQueue && _servoAttached && (_isHome || !_goHomeWhenIdle) ) {
        // second pass through, we've gone home now we're looking to detach.
        if (millis() > _lastUpdate + SERVO_TIMEOUT_MS) {
            Serial.print(F("Servo on pin: "));
            Serial.print(_servoPin);
            Serial.println(F(": TIMEOUT, detaching"));
            detach();
            // Unset the flag so we don't try to detach again
            _servoAttached = false;
        }
    }
};

void ConnectServo::update() {

    // Check if we're paused and handle timeouts
    checkTime();

    // Check to see if we've stopped and shouldn't have.
    // isMovingAndCallYield works for us, despite being deprecated.
    // Alternative of areInterruptsActive() breaks simultaneous
    // servo movement on ESP8266. So... let's not do that.
    // Also need to check our blocking flags.
    if (!isMovingAndCallYield() && !_waitingForServo && !_waitingForLED && !_waitingForTime) {
        // We've stopped, so check if there's anything in the queue
        if (!_servoQueue.isEmpty()) {
            Serial.print(F("Servo on pin: "));
            Serial.print(_servoPin);
            Serial.println(F(": stopped, retrieving next queue action."));
            // There's something in the queue, so pop it and execute it
            ServoQueueItem item = dequeue();
            // Item popped from queue, so flag that.
            _emptiedQueue = false;
            // Re-attach the servo
            if (!_servoAttached) {
                Serial.print(F("Servo on pin: "));
                Serial.print(_servoPin);
                Serial.println(F(": re-attaching."));
                attach(_servoPin);
                _servoAttached = true;
            }

            // Get the targetFunction from item.call
            // Decaode the call and dispatch.
            // This is hacky, but I can't pass a pointer to a member function
            // since - as best I can tell - C++ doesn't do that.
            // Hence the parser macro definitions
            switch (item.call) {
                case STARTEASETO:
                    Serial.print(F("Servo on pin: "));
                    Serial.print(_servoPin);
                    Serial.print(F(": START EASING MOVE to "));
                    Serial.println(item.param1);
                    setEasingType(item.animationType);
                    startEaseTo(item.param1, item.servoSpeed);
                    // Flag that we've moved from home
                    _isHome = false;
                    break;
                case WRITE:
                    write(item.param1);
                    Serial.print(F("Servo on pin: "));
                    Serial.print(_servoPin);
                    Serial.print(F(": WRITE MOVE to "));
                    Serial.println(item.param1);
                    // Flag that we've moved from home
                    _isHome = false;
                    break;
                case WAIT_FOR_SERVO:
                    Serial.print(F("Servo on pin: "));
                    Serial.print(_servoPin);
                    Serial.print(F(": BLOCK FOR SERVO on pin "));
                    Serial.println(item.targetServo);
                    _waitingForServo = item.targetServo;
                    break;
                case WAIT_FOR_LEDS:
                    // TODO: Implement
                    Serial.println(F("WAIT FOR LEDS: Yeah, we need to implement this"));
                    break;
                case MESSAGE_SERVO:
                    Serial.print(F("Servo on pin: "));
                    Serial.print(_servoPin);
                    Serial.print(F(": MESSAGE SERVO triggered to servo on: "));
                    Serial.println(item.targetServo);
                    ConnectMessenger.sendServoMessage(item.targetServo, getPin());
                    break;
                case WAIT:
                    Serial.print(F("Servo on pin: "));
                    Serial.print(_servoPin);
                    Serial.print(F(": WAIT for "));
                    Serial.println(item.servoSpeed);
                    // Set wait flag
                    _waitingForTime = true;
                    // Set wait end point
                    _targetTime = millis() + item.servoSpeed;
                    break;
                default:
                    break;
            }
        } else {
            // If this is the first tme we've fallen through queue check,
            // flag that and take a timestamp.
            if (!_emptiedQueue) {
                Serial.print(F("Servo on pin: "));
                Serial.print(_servoPin);
                Serial.println(F(": Queue emptied, triggering timeout actions"));
                _emptiedQueue = true;
                _lastUpdate = millis();
            }
        }
    }
};
