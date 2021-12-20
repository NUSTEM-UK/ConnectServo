#include <ServoMessenger.h>
#include <ConnectServo.h>

// RegisteredServo initialiser
// Weird syntax, see
// https://stackoverflow.com/questions/30069384/provides-no-initializer-for-reference-member
// RegisteredServo::RegisteredServo(ConnectServo& servoReference, uint8_t serovPin) :
//     servoReference(servoReference), servoPin(servoPin)
// {
//     servoReference = nullptr;
//     servoPin = NULL;
// };


/**
 * Helper function to enqueue events for both sides of a waitfor/message pair
 */
void servoWaitForServo(ConnectServo& servoWaiting, ConnectServo& waitingFor) {
    servoWaiting.queueWaitForServo(waitingFor.getPin());
    waitingFor.queueMessageServo(servoWaiting.getPin());
}

/**
 * Call update() on all ConnectServo objects
 */
void ServoMessenger::updateServos() {
    // Iterate over servoList
    for (uint8_t i = 0; i < _servoCount; i++) {
        _servoList[i].servoReference->update();
    }
}

// ServoMessenger initialiser
ServoMessenger::ServoMessenger() {
  uint8_t _servoCount = 0;
  _servoList[NUMBER_OF_SERVOS];
}

void ServoMessenger::registerServo(ConnectServo* newServo, uint8_t newServoPin) {
    _servoList[_servoCount].servoPin = newServoPin;
    _servoList[_servoCount].servoReference = newServo;
    _servoCount++;
    Serial.print(F("Servo registered on pin: "));
    Serial.println(newServoPin);
    // TODO: remove debug code
    if (_servoCount > NUMBER_OF_SERVOS) {
      Serial.println(F("WAAAAAY TOO MANY SERVOS REGISTERED"));
    }
}

void ServoMessenger::sendServoMessage(uint8_t fromServoPin, uint8_t toServoPin) {
    // Iterate over servoList to match fromServoPin
    for (uint8_t i = 0; i < _servoCount; i++) {
        if (_servoList[i].servoPin == fromServoPin) {
            // Found the servo, call unblockFromServo
            _servoList[i].servoReference->unblockFromServo(toServoPin);
            // _servoList[i].servoReference->unblockFromServo(fromServoPin);
        }
    }
}
