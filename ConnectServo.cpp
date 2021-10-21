#include <ConnectServo.h>

// ServoQueueItem initializer
ServoQueueItem::ServoQueueItem() {

};

void ServoQueueItem::assign(GenericFP newCall, int newParam1, const char * newAnimationType, int newServoSpeed) {
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
        // We've stopped, so pop the next queue item
        ServoQueueItem item = dequeue();
        self.&(item.call)(item.param1, item.animationType, item.servoSpeed));
    }
};
