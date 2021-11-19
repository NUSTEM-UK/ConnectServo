#include <Arduino.h>
#include <ConnectServo.h>

ConnectServo servo1;
ConnectServo servo2;

// Instantiate an object to hold our tempoorary values
ServoQueueItem myItem;

uint8_t i = 0;

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Starting...");
    servo1.setPin(D5);
    servo2.setPin(D7);
    delay(500);

    servo1.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
    servo1.queueEaseTo(0, EASE_CUBIC_IN_OUT, 90);
    // servo2.queueWaitForServo(&servo1);
    // servo1.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
    servo2.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
}

void loop() {
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
