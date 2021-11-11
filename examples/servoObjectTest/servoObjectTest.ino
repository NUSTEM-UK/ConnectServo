#include <Arduino.h>
#include <ConnectServo.h>

ConnectServo servo1;


// Give ourselves some placeholder things to work with
ServoQueueItem myItem;
int value = 12;
int speed = 30;

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Starting...");
    servo1.attach(D5);
    servo1.write(0);
    delay(500);

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
}

void loop() {
    servo1.update();
}
