#include <ServoMessenger.h>

// ServoMessenger initialiser
ServoMessenger::ServoMessenger() {
  uint8_t _servoCount = 0;
  _servoList[NUMBER_OF_SERVOS];
}

void ServoMessenger::registerServo(ConnectServo& newServo, uint8_t newServoPin) {
    _servoList[_servoCount].servoReference = newServo;
    _servoList[_servoCount].servoPin = newServoPin;
    _servoCount++;
    // TODO: remove debug code
    if (_servoCount > NUMBER_OF_SERVOS) {
      Serial.println(F("WAAAAAY TOO MANY SERVOS REGISTERED"));
    }
}

void ServoMessenger::sendServoMessage(uint8_t fromServoPin, uint8_t toServoPin) {
    // Iterate over servoList to match fromServoPin
    for (uint8_t i = 0; i < _servoCount; i++) {
        if (_servoList[i].servoPin == fromServoPin) {
            _servoList[i].servoReference.unblockFromServo(fromServoPin);
        }
    }
}
