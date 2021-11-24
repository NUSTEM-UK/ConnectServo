#include <ConnectServo.h>

ServoMessenger ConnectMessenger;

void updateConnectServos() {
    ConnectMessenger.updateServos();
}

// Initialize the servo object, passing an initializer list to the cppQueue object
// See http://arduinoetcetera.blogspot.com/2011/01/classes-within-classes-initialiser.html
ConnectServo::ConnectServo() : _servoQueue(sizeof(ServoQueueItem), QUEUE_SIZE_ITEMS, IMPLEMENTATION) {

};

void ConnectServo::setPin(uint8_t pin) {
    _servoPin = pin;
    attach(_servoPin);
    write(0);
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
        Serial.print("Servo on pin: ");
        Serial.print(_servoPin);
        Serial.println(": BLOCK LIFTED");
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

void ConnectServo::checkTime() {
    if (_waitingForTime) {
        if (millis() > _targetTime) {
            _waitingForTime = false;
        }
    } else if (_emptiedQueue && _servoAttached) {
        if (millis() > _lastUpdate + SERVO_TIMEOUT_MS) {
            Serial.print("Servo on pin: ");
            Serial.print(_servoPin);
            Serial.println(": TIMEOUT, detaching");
            detach();
            // Unset the flag so we don't try to detach again
            _servoAttached = false;
        }
    }
};

void ConnectServo::update() {

    // Check if we're paused
    checkTime();

    // Check to see if we've stopped and shouldn't have.
    // isMovingAndCallYield works for us, despite being deprecated.
    // Alternative of areInterruptsActive() breaks simultaneous
    // servo movement on ESP8266. So... let's not do that.
    // Also need to check our blocking flags.
    if (!isMovingAndCallYield() && !_waitingForServo && !_waitingForLED && !_waitingForTime) {
        // We've stopped, so check if there's anything in the queue
        if (!_servoQueue.isEmpty()) {
            Serial.print("Servo on pin: ");
            Serial.print(_servoPin);
            Serial.println(": stopped, retrieving next queue action.");
            // There's something in the queue, so pop it and execute it
            ServoQueueItem item = dequeue();
            // Item popped from queue, so flag that.
            _emptiedQueue = false;
            // Re-attach the servo
            if (!_servoAttached) {
                Serial.print("Servo on pin: ");
                Serial.print(_servoPin);
                Serial.println(": re-attaching.");
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
                    Serial.print("Servo on pin: ");
                    Serial.print(_servoPin);
                    Serial.print(": START EASING MOVE to ");
                    Serial.println(item.param1);
                    setEasingType(item.animationType);
                    startEaseTo(item.param1, item.servoSpeed);
                    break;
                case WRITE:
                    write(item.param1);
                    Serial.print("Servo on pin: ");
                    Serial.print(_servoPin);
                    Serial.print(": WRITE MOVE to ");
                    Serial.println(item.param1);
                    break;
                case WAIT_FOR_SERVO:
                    Serial.print("Servo on pin: ");
                    Serial.print(_servoPin);
                    Serial.print(": BLOCK FOR SERVO on pin ");
                    Serial.println(item.targetServo);
                    _waitingForServo = item.targetServo;
                    break;
                case WAIT_FOR_LEDS:
                    Serial.println("WAIT FOR LEDS: Yeah, we need to implement this");
                    break;
                case MESSAGE_SERVO:
                    Serial.print("Servo on pin: ");
                    Serial.print(_servoPin);
                    Serial.print(": MESSAGE SERVO triggered to servo on: ");
                    Serial.println(item.targetServo);
                    ConnectMessenger.sendServoMessage(item.targetServo, getPin());
                    break;
                case WAIT:
                    Serial.print("Servo on pin: ");
                    Serial.print(_servoPin);
                    Serial.print(": WAIT for ");
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
            // flag that and take a tiemstamp.
            if (!_emptiedQueue) {
                Serial.print("Servo on pin: ");
                Serial.print(_servoPin);
                Serial.println(": Queue emptied, triggering timeout wait");
                _emptiedQueue = true;
                _lastUpdate = millis();
            }
        }
    }
};
