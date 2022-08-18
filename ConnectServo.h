
#ifndef ConnectServo_h
#define ConnectServo_h

#include <Arduino.h>
#include <ServoEasing.h>
#include <cppQueue.h>
#include "ServoQueueItem.h"
#include "ServoMessenger.h"

#ifdef Kniwwelino_h
#include "Kniwwelino.h"
#endif

#define IMPLEMENTATION FIFO
#define QUEUE_SIZE_ITEMS 50

// Parser macros
#define STARTEASETO 0
#define WRITE 1
#define WAIT_FOR_SERVO 2
#define WAIT_FOR_LEDS 3
#define MESSAGE_SERVO 4
#define WAIT 5
#define GO_HOME 6 // Needed because slightly different behaviour to STARTEASETO

// wait 3 seconds on queue empty before detaching servo
#define SERVO_TIMEOUT_MS 3000
// ms to wait for a plain move (queued after move)
// This isn't quite enough for a 180 degree move, but
// at least the servo twitches. Adding more delay
// could be done with queueWait().
#define SERVO_MOVE_SLEW_WAIT 300

// Helper function to dispatch both sides of the block/unblock callback
void servoWaitForServo(ConnectServo&, ConnectServo&);
// Helper function to update all servos registered with ServoMessenger object
void updateConnectServio();

class ConnectServo : public ServoEasing {
    public:
        ConnectServo();
        void setPin(uint8_t);
        uint8_t getPin();
        // Public values for serial control
        void queueEaseTo(uint8_t, uint8_t, uint16_t);
        void queueMoveTo(uint8_t);
        void queueWaitForServo(uint8_t);
        void queueMessageServo(uint8_t);
        void queueWait(uint16_t);
        ServoQueueItem dequeue();
        void registerServo();
        void unblockFromServo(uint8_t);
        void unblockFromLED(void);
        void setHome(uint8_t);
        void keepActive();
        void detachWhenIdle();
        void update();
        void serialCommandedPosition(uint8_t);
        void wave(uint8_t, uint8_t, uint8_t);
        void flap(uint8_t, uint8_t, uint8_t);
        void bounce(uint8_t, uint8_t, uint8_t);
        #ifdef Kniwwelino_h
        void callKniwwelinoUpdate();
        #endif
    private:
        uint8_t _servoPin;
        cppQueue _servoQueue;
        ServoQueueItem _queueItem1;
        ServoQueueItem _queueItem2;
        // Wait blocks are ints rather than bools so they can store the unblocking object ID (pin number))
        uint8_t _waitingForServo;
        uint8_t _waitingForLED;
        uint8_t _homePosition;
        uint8_t _homeSpeed;
        uint8_t _nextSerialPosition;
        uint8_t _currentSerialPosition;
        bool _goHomeWhenIdle;
        bool _goingHome;
        bool _isHome;
        bool _waitingForTime;
        bool _keepActive;
        unsigned long _targetTime;
        unsigned long _lastUpdate;
        bool _emptiedQueue;
        bool _servoAttached;
        void checkTime();
};

#endif
