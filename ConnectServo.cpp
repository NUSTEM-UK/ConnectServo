#include <ConnectServo.h>

// Initialize the servo object, passing an initializer list to the cppQueue object
// See http://arduinoetcetera.blogspot.com/2011/01/classes-within-classes-initialiser.html
ConnectServo::ConnectServo() : _servoQueue(sizeof(ServoQueueItem), QUEUE_SIZE_ITEMS, IMPLEMENTATION) {

};

// void ConnectServo::enqueue(ServoQueueItem item) {
//   _servoQueue.push(&item);
// };

void ConnectServo::enqueue(uint8_t newCall, uint8_t newParam1, uint8_t newAnimationType, uint16_t newServoSpeed, ConnectServo *) {
    ServoQueueItem item;
    item.assign(newCall, newParam1, newAnimationType, newServoSpeed, NULL);
    _servoQueue.push(&item);
};

void ConnectServo::enqueue(uint8_t newCall, uint8_t newParam1, uint8_t newAnimationType, uint16_t newServoSpeed) {
    ServoQueueItem item;
    item.assign(newCall, newParam1, newAnimationType, newServoSpeed, NULL);
    _servoQueue.push(&item);
};

void ConnectServo::enqueue(uint8_t newCall, uint8_t newParam1) {
    ServoQueueItem item;
    item.assign(newCall, newParam1, NULL, NULL, NULL);
    _servoQueue.push(&item);
};

void ConnectServo::queueEaseTo(uint8_t newParam1, uint8_t newAnimationType, uint16_t newServoSpeed) {
    ServoQueueItem item;
    item.assign(STARTEASETO, newParam1, newAnimationType, newServoSpeed, NULL);
    _servoQueue.push(&item);
};

void ConnectServo::queueMoveTo(uint8_t newParam1) {
    ServoQueueItem item;
    item.assign(WRITE, newParam1, NULL, NULL, NULL);
    _servoQueue.push(&item);
};

void ConnectServo::queueWaitForServo(ConnectServo* targetServo) {
    ServoQueueItem item;
    item.assign(WAIT_FOR_OTHER_SERVO, NULL, NULL, NULL, targetServo);
    _servoQueue.push(&item);
};

void ConnectServo::queueMessageServo(ConnectServo* targetServo) {
    ServoQueueItem item;
    item.assign(MESSAGE_SERVO, NULL, NULL, NULL, targetServo);
    _servoQueue.push(&item);
};

void ConnectServo::unblockFromServo(void) {
    _waitingForServo = false;
}

ServoQueueItem ConnectServo::dequeue() {
    ServoQueueItem _item;
    _servoQueue.pop(&_item);
    return _item;
};

bool ConnectServo::update() {
    if (!isMovingAndCallYield() && !_waitingForServo && !_waitingForLED) {
        // Serial.println("Servo stopped, retrieving next queue action.");
        // We've stopped, so check if there's anything in the queue
        if (!_servoQueue.isEmpty()) {
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
                case WAIT_FOR_OTHER_SERVO:
                    Serial.println("WAIT FOR SERVO triggered");
                    _waitingForServo = true;
                    item.targetServo->queueMessageServo(this);
                    break;
                case WAIT_FOR_LEDS:
                    Serial.println("WAIT FOR LEDS: Yeah, we need to implement this");
                    break;
                case MESSAGE_SERVO:
                    Serial.println("MESSAGE SERVO triggered");
                    item.targetServo->unblockFromServo();
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
