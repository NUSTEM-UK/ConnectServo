#include <Arduino.h>
#include <ServoEasing.h>
#include <cppQueue.h>

#define IMPLEMENTATION FIFO
#define QUEUE_SIZE_ITEMS 20

// Parser macros
#define STARTEASETO 0
#define WRITE 1
#define WAIT_FOR_OTHER_SERVO 2
#define WAIT_FOR_LEDS 3
#define MESSAGE_SERVO 4

// Generic function pointer. Note variable-length argument list
// typedef void (*GenericFP)(void *, ...);

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
        // call, target position, animation type, servo speed
        void assign(uint8_t, uint8_t, uint8_t, uint16_t, ConnectServo*);
        uint8_t call;                   // int representing call; we'll parse in ConnectServo::update()
        uint8_t param1;                 // Target position or (for waitForServo) servo to wait on.
        uint8_t animationType; // eg. 'EASE_CUBIC_IN_OUT'
        uint16_t servoSpeed;             // Slew rate (perscentage: map to 0-255. Servo theoretical max is around 200 degrees/sec, ~80%)
        ConnectServo* targetServo;
};
class ConnectServo : public ServoEasing {
    public:
        ConnectServo();
        void queueEaseTo(uint8_t, uint8_t, uint16_t);
        void queueMoveTo(uint8_t);
        void queueWaitForServo(ConnectServo*);
        void queueMessageServo(ConnectServo*);
        void enqueue(uint8_t, uint8_t, uint8_t, uint16_t, ConnectServo *);
        void enqueue(uint8_t, uint8_t, uint8_t, uint16_t);
        void enqueue(uint8_t, uint8_t);
        ServoQueueItem dequeue();
        void unblockFromServo(void);
        void unblockFromLED(void);
        bool update();
    private:
        cppQueue _servoQueue;
        ServoQueueItem _queueItem1;
        ServoQueueItem _queueItem2;
        bool _waitingForServo;
        bool _waitingForLED;
};


