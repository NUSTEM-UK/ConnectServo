#include <ConnectServo.h>

ConnectServo servo1;

static const uint8_t servo1Pin = D7;

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println();
    servo1.setPin(servo1Pin);
    delay(500);

    Serial.println();
    Serial.println("Starting...");

    servo1.setHome(90);
    delay(1000);

    Serial.println("Populating buffer...");
    for (size_t i = 0; i < 1000; i++)
    {
        servo1.bounce(1, 30, 150);
        if (i%10 == 0) {
            Serial.println(i);
        }
    }
    Serial.println("...done");

}

void loop() {
    servo1.update();
}
