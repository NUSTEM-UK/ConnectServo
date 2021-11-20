#include <Arduino.h>
#include <ConnectServo.h> // TODO: circular declaration?

// Forward declaration of ConnectServo class
class ConnectServo;

#define NUMBER_OF_SERVOS 12

typedef struct {
    ConnectServo& servoReference;
    uint8_t servoPin;
} registeredServo;

class ServoMessenger {
    public:
        ServoMessenger();
        // I'm a bit undecided about whether to pass in servoPin, or call &servo.getPin()
        void registerServo(ConnectServo&, uint8_t);
        void sendServoMessage(uint8_t, uint8_t);
    private:
        registeredServo _servoList[];
        uint8_t _servoCount;
};
