
#ifndef ConnectServo_h
#define ConnectServo_h

#include <Arduino.h>
#include <ServoEasing.h>
#include <cppQueue.h>
#include <ServoQueueItem.h>
#include <ServoMessenger.h>

#define IMPLEMENTATION FIFO
#define QUEUE_SIZE_ITEMS 20

// Parser macros
#define STARTEASETO 0
#define WRITE 1
#define WAIT_FOR_SERVO 2
#define WAIT_FOR_LEDS 3
#define MESSAGE_SERVO 4

extern ServoMessenger ConnectMessenger;

class ConnectServo : public ServoEasing {
    public:
        ConnectServo();
        void setPin(uint8_t);
        uint8_t getPin();
        void queueEaseTo(uint8_t, uint8_t, uint16_t);
        void queueMoveTo(uint8_t);
        void queueWaitForServo(uint8_t);
        void queueMessageServo(uint8_t);
        // void enqueue(uint8_t, uint8_t, uint8_t, uint16_t, ConnectServo);
        // void enqueue(uint8_t, uint8_t, uint8_t, uint16_t);
        // void enqueue(uint8_t, uint8_t);
        ServoQueueItem dequeue();
        void registerServo();
        void unblockFromServo(uint8_t);
        void unblockFromLED(void);
        bool update();
    private:
        uint8_t _servoPin;
        cppQueue _servoQueue;
        ServoQueueItem _queueItem1;
        ServoQueueItem _queueItem2;
        // Wait blocks are ints rather than bools so they can store the unblocking object ID (pin number))
        uint8_t _waitingForServo;
        uint8_t _waitingForLED;
};

#endif
