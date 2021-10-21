#include <Arduino.h>
#include <ServoEasing.h>
#include <cppQueue.h>

#define IMPLEMENTATION FIFO
#define QUEUE_SIZE_ITEMS 20

// Generic function pointer. Note variable-length argument list
typedef void (*GenericFP)(void *, ...);

// Servo queue item type
// typedef struct {
//     GenericFP call;             // eg. startEaseTo, moveTo/write, waitForServo
//     int param1;                 // Target position or (for waitForServo) servo to wait on.
//     const char * animationType; // eg. 'EASE_CUBIC_IN_OUT'
//     int servoSpeed;             // Slew rate (perscentage: map to 0-255. Servo theoretical max is around 200 degrees/sec, ~80%)
// } servoQueueItem;

class ServoQueueItem {
    public:
        ServoQueueItem();
        void assign(GenericFP, int, const char *, int);
        GenericFP call;             // eg. startEaseTo, moveTo/write, waitForServo
        int param1;                 // Target position or (for waitForServo) servo to wait on.
        const char * animationType; // eg. 'EASE_CUBIC_IN_OUT'
        int servoSpeed;             // Slew rate (perscentage: map to 0-255. Servo theoretical max is around 200 degrees/sec, ~80%)
};

class ConnectServo : public ServoEasing {
    public:
        ConnectServo();
        void enqueue(ServoQueueItem item);
        ServoQueueItem dequeue();
        // cppQueue servoQueue;
    private:
        cppQueue _servoQueue;
        ServoQueueItem _queueItem1;
        ServoQueueItem _queueItem2;

};
