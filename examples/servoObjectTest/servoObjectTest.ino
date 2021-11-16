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
    servo1.attach(D5);
    servo1.write(0);
    servo2.attach(D7);
    servo2.write(0);
    delay(500);

    // Queue some moves for the servo objects
    servo1.enqueue(STARTEASETO, 180, EASE_CUBIC_IN_OUT, 30);
    // servo1.enqueue(myItem);
    servo1.enqueue(WRITE, 0);
    servo1.enqueue(STARTEASETO, 90, EASE_SINE_IN, 100);
    servo1.enqueue(WRITE, 180);
    servo1.enqueue(STARTEASETO, 0, EASE_SINE_OUT, 50);

    servo2.enqueue(STARTEASETO, 180, EASE_CUBIC_IN_OUT, 30);
    servo2.enqueue(STARTEASETO, 0, EASE_CUBIC_IN_OUT, 150);
    // 3 wiggles
    // TODO: check if write actually works, or reports
    //       as finished after every dispatch.
    while (i < 3) {
        servo2.enqueue(STARTEASETO, 180, EASE_LINEAR, 255);
        servo2.enqueue(STARTEASETO, 0, EASE_LINEAR, 255);
        i++;
    }
    servo2.enqueue(STARTEASETO, 180, EASE_CUBIC_IN_OUT, 20);

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
