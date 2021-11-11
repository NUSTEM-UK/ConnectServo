#include <Arduino.h>
#include <ConnectServo.h>

ConnectServo servo1;
ConnectServo servo2;

// Instantiate an object to hold our tempoorary values
ServoQueueItem myItem;

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
    myItem.assign(STARTEASETO, 180, EASE_CUBIC_IN_OUT, 30);
    servo1.enqueue(myItem);
    myItem.assign(WRITE, 0, 0, 0);
    servo1.enqueue(myItem);
    myItem.assign(STARTEASETO, 90, EASE_SINE_IN, 100);
    servo1.enqueue(myItem);
    myItem.assign(WRITE, 180, 0, 0);
    servo1.enqueue(myItem);
    myItem.assign(STARTEASETO, 0, EASE_SINE_OUT, 50);
    servo1.enqueue(myItem);

    myItem.assign(STARTEASETO, 180, EASE_CUBIC_IN_OUT, 30);
    servo2.enqueue(myItem);
    myItem.assign(STARTEASETO, 0, EASE_CUBIC_IN_OUT, 150);
    servo2.enqueue(myItem);
    myItem.assign(STARTEASETO, 180, EASE_LINEAR, 255);
    servo2.enqueue(myItem);
    myItem.assign(STARTEASETO, 0, EASE_LINEAR, 255);
    servo2.enqueue(myItem);

    myItem.assign(STARTEASETO, 180, EASE_LINEAR, 255);
    servo2.enqueue(myItem);
    myItem.assign(STARTEASETO, 0, EASE_LINEAR, 255);
    servo2.enqueue(myItem);

    myItem.assign(STARTEASETO, 180, EASE_LINEAR, 255);
    servo2.enqueue(myItem);
    myItem.assign(STARTEASETO, 0, EASE_LINEAR, 255);
    servo2.enqueue(myItem);

    myItem.assign(STARTEASETO, 180, EASE_CUBIC_IN_OUT, 150);


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
