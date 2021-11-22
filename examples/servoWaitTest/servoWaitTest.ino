// #include <Arduino.h>
#include <ConnectServo.h>

ConnectServo servo1;
ConnectServo servo2;

static const uint8_t servo1Pin = D5;
static const uint8_t servo2Pin = D7;

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println();
    Serial.println();
    Serial.println("Starting...");
    servo1.setPin(servo1Pin);
    servo2.setPin(servo2Pin);
    delay(500);

    servo1.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
    servo1.queueEaseTo(0, EASE_CUBIC_IN_OUT, 90);
    servoWaitForServo(servo2, servo1);
    servo1.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
    servo2.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
    servo2.queueMoveTo(0);
    servo2.queueWait(500);

}

void loop() {
    // servo1.update();
    // servo2.update();
    // TODO: Need a timeout on detach, and some neater way of re-attaching the servos.
    if (!servo1.update()) {
        // It's stopped with an empty queue, so we're done
        // Serial.println("Disconnecting servo 1");
        servo1.detach();
        // TODO: Ah, this triggers every pass through the loop. Oops.
    }
    if (!servo2.update()) {
        // It's stopped with an empty queue, so we're done
        // Serial.println("Disconnecting servo 2");
        servo2.detach();
    }
}
