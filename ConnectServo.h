#include <Arduino.h>
#include <ServoEasing.h>
#include <cppQueue.h>
#include <ServoQueueItem.h>

#define IMPLEMENTATION FIFO
#define QUEUE_SIZE_ITEMS 20

// Parser macros
#define STARTEASETO 0
#define WRITE 1
#define WAIT_FOR_OTHER_SERVO 2
#define WAIT_FOR_LEDS 3
#define MESSAGE_SERVO 4
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


