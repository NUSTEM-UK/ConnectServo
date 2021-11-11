#include <ConnectServo.h>

// ServoQueueItem initializer
ServoQueueItem::ServoQueueItem() {

};

void ServoQueueItem::assign(int newCall, int newParam1, uint8_t newAnimationType, int newServoSpeed) {
    call = newCall;
    param1 = newParam1;
    animationType = newAnimationType;
    servoSpeed = newServoSpeed;
}

// Initialize the servo object, passing an initializer list to the cppQueue object
// See http://arduinoetcetera.blogspot.com/2011/01/classes-within-classes-initialiser.html
ConnectServo::ConnectServo() : _servoQueue(sizeof(ServoQueueItem), QUEUE_SIZE_ITEMS, IMPLEMENTATION) {

};

void ConnectServo::enqueue(ServoQueueItem item) {
  _servoQueue.push(&item);
};

ServoQueueItem ConnectServo::dequeue() {
    ServoQueueItem _item;
    _servoQueue.pop(&_item);
    return _item;
};

void ConnectServo::update() {
    if (!isMovingAndCallYield()) {
        Serial.println("Servo stopped, retrieving next queue action.");
        // We've stopped, so check if there's anything in the queue
        if (!_servoQueue.isEmpty()) {
            Serial.println("Popping next action");
            // There's something in the queue, so pop it and execute it
            ServoQueueItem item = dequeue();
            // Get the targetFunction from item.call

            switch (item.call) {
                case STARTEASETO:
                    startEaseTo(item.param1, item.animationType, item.servoSpeed);
                    Serial.print("Ease move dispatched: ");
                    Serial.print(item.param1);
                    Serial.print(" ");
                    Serial.print(item.animationType);
                    Serial.print(" ");
                    Serial.println(item.servoSpeed);
                    break;
                case WRITE:
                    write(item.param1);
                    Serial.print("Write move dispatched: ");
                    Serial.println(item.param1);
                    break;
                case WAIT_FOR_OTHER_SERVO:
                    Serial.println("WAIT FOR SERVO: Yeah, we need to implement this");
                    break;
                case WAIT_FOR_LEDS:
                    Serial.println("WAIT FOR LEDS: Yeah, we need to implement this");
                    break;
                default:
                    break;
            }
        }
    }
};
