#include <ConnectServo.h>

ServoMessenger ConnectMessenger;

// Initialize the servo object, passing an initializer list to the cppQueue object
// See http://arduinoetcetera.blogspot.com/2011/01/classes-within-classes-initialiser.html
ConnectServo::ConnectServo() : _servoQueue(sizeof(ServoQueueItem), QUEUE_SIZE_ITEMS, IMPLEMENTATION) {

};

void ConnectServo::setPin(uint8_t pin) {
    _servoPin = pin;
    attach(_servoPin);
    write(0);
    registerServo();
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
    // item.assign(WRITE, newParam1, NULL, NULL, NULL);
    // TODO: Check if we can get write() to work, but it looks like
    // ServoEasing thinks the transit time is 0 so dequeing is cruddy.
    // Hence we fudge it with a maximum-rate linear ease to.
    // FIXME: this doesn't complete before the servo is detached either.
    //        needs timeout on detach at end of move.
    item.assign(STARTEASETO, newParam1, EASE_LINEAR, 1023, NULL);
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

ServoQueueItem ConnectServo::dequeue() {
    ServoQueueItem _item;
    _servoQueue.pop(&_item);
    return _item;
};

bool ConnectServo::update() {
    // Need to call processEvent() on the event manager, to trigger release of
    // messages. TODO: move this into ConnectLib


    // Now check to see if we've stopped and shouldn't have
    if (!isMovingAndCallYield() && !_waitingForServo && !_waitingForLED) {
        // We've stopped, so check if there's anything in the queue
        if (!_servoQueue.isEmpty()) {
            Serial.print("Servo on pin: ");
            Serial.print(_servoPin);
            Serial.println(": stopped, retrieving next queue action.");
            // Serial.println("Popping next action");
            // There's something in the queue, so pop it and execute it
            ServoQueueItem item = dequeue();
            // Get the targetFunction from item.call

            // Decaode the call and dispatch.
            // This is hacky, but I can't pass a pointer to a member function
            // since - as best I can tell - C++ doesn't do that.
            // Hence the parser macro definitions
            switch (item.call) {
                case STARTEASETO:
                    Serial.print("Servo on pin: ");
                    Serial.print(_servoPin);
                    Serial.println(": START EASING MOVE");
                    // FIXME: This is to work around over-zealous detach in
                    //        example code main loop. Needs a timeout on detach.
                    attach(_servoPin);
                    setEasingType(item.animationType);
                    startEaseTo(item.param1, item.servoSpeed);
                    // Serial.print("Ease move dispatched: ");
                    // Serial.print(item.param1);
                    // Serial.print(" ");
                    // Serial.print(item.animationType);
                    // Serial.print(" ");
                    // Serial.println(item.servoSpeed);
                    break;
                case WRITE:
                    write(item.param1);
                    // Serial.print("Write move dispatched: ");
                    Serial.println(item.param1);
                    break;
                case WAIT_FOR_SERVO:
                    Serial.print("Servo on pin: ");
                    Serial.print(_servoPin);
                    Serial.print(": BLOCK FOR SERVO on pin ");
                    Serial.println(item.targetServo);
                    _waitingForServo = item.targetServo;
                    // uint8_t targetServoID = item.targetServo;
                    // gConnectEventManager.addListener(getPin(), this->unblockFromServo);
                    // Queue a MessageServo event passing item.targetServo as identifier

                    // item.targetServo->queueMessageServo(this);
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
                default:
                    break;
            }
            return true;
        } else {
            return false;
        }
    }
};


/**
 * pretty sure we don't need any of this still
 *
 */

// void ConnectServo::enqueue(ServoQueueItem item) {
//   _servoQueue.push(&item);
// };

// void ConnectServo::enqueue(uint8_t newCall, uint8_t newParam1, uint8_t newAnimationType, uint16_t newServoSpeed, ConnectServo *) {
//     ServoQueueItem item;
//     item.assign(newCall, newParam1, newAnimationType, newServoSpeed, NULL);
//     _servoQueue.push(&item);
// };

// void ConnectServo::enqueue(uint8_t newCall, uint8_t newParam1, uint8_t newAnimationType, uint16_t newServoSpeed) {
//     ServoQueueItem item;
//     item.assign(newCall, newParam1, newAnimationType, newServoSpeed, NULL);
//     _servoQueue.push(&item);
// };

// void ConnectServo::enqueue(uint8_t newCall, uint8_t newParam1) {
//     ServoQueueItem item;
//     item.assign(newCall, newParam1, NULL, NULL, NULL);
//     _servoQueue.push(&item);
// };
