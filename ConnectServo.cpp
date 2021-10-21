#include <ConnectServo.h>

// ServoQueueItem initializer
ServoQueueItem::ServoQueueItem() {

};

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
