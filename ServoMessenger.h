#ifndef ServoMessenger_h
#define ServoMessenger_h
#include <Arduino.h>

// Forward declaration of ConnectServo class
class ConnectServo;

#define NUMBER_OF_SERVOS 12

// class RegisteredServo {
//     public:
//         RegisteredServo(ConnectServo&, uint8_t );
//         ConnectServo& servoReference;
//         uint8_t servoPin;
// };
typedef struct {
    ConnectServo* servoReference;
    uint8_t servoPin;
} registeredServo;

class ServoMessenger {
    public:
        ServoMessenger();
        // I'm a bit undecided about whether to pass in servoPin, or call &servo.getPin()
        void registerServo(ConnectServo*, uint8_t);
        void sendServoMessage(uint8_t, uint8_t);
        void updateServos();
        void serialCommand(String);
    private:
        registeredServo _servoList[NUMBER_OF_SERVOS];
        uint8_t _servoCount;
};

#endif
