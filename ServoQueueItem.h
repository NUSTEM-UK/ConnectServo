#include <Arduino.h>

// Generic function pointer. Note variable-length argument list
// typedef void (*GenericFP)(void *, ...);

// Servo queue item type
// typedef struct {
//     GenericFP call;             // eg. startEaseTo, moveTo/write, waitForServo
//     int param1;                 // Target position or (for waitForServo) servo to wait on.
//     const char * animationType; // eg. 'EASE_CUBIC_IN_OUT'
//     int servoSpeed;             // Slew rate (perscentage: map to 0-255. Servo theoretical max is around 200 degrees/sec, ~80%)
// } servoQueueItem;

// Forward declaration of ConnectServo class
class ConnectServo;

class ServoQueueItem {
    public:
        ServoQueueItem();
        // call, target position, animation type, servo speed
        void assign(uint8_t, uint8_t, uint8_t, uint16_t, ConnectServo&);
        uint8_t call;                   // int representing call; we'll parse in ConnectServo::update()
        uint8_t param1;                 // Target position or (for waitForServo) servo to wait on.
        uint8_t animationType; // eg. 'EASE_CUBIC_IN_OUT'
        uint16_t servoSpeed;             // Slew rate (perscentage: map to 0-255. Servo theoretical max is around 200 degrees/sec, ~80%)
        ConnectServo& targetServo;
};
