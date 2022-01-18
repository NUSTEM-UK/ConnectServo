#include <ConnectServo.h>

ConnectServo servo1;

static const uint8_t servo1Pin = D7;

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println();
    Serial.println("Starting...");
    servo1.setPin(servo1Pin);
    delay(500);

    servo1.setHome(90);
    delay(3000);

    servo1.wave(3, 45, 135);
    servo1.queueWait(500);
    servo1.flap(3, 30, 150);
    servo1.queueWait(500);
    servo1.bounce(3, 30, 150);
    delay(500);
}

void loop() {
    servo1.update();
}
