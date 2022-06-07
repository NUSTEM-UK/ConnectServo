#include <ConnectServo.h>

ConnectServo servo1;

static const uint8_t servo1Pin = D4;
int servo_min = 0;
int servo_max = 180;
boolean servo_going_up = true;

void nextServoAction() {
    // Set speed to random int between 20 and 150
    int speed = random(10, 100);
    if (servo_going_up) {
        servo1.queueEaseTo(servo_min, EASE_CUBIC_IN_OUT, speed);
        servo_going_up = false;
        Serial.print("Going down at: ");
        Serial.println(speed);
    } else {
        servo1.queueEaseTo(servo_max, EASE_CUBIC_IN_OUT, speed);
        servo_going_up = true;
        Serial.print("Going up at: ");
        Serial.println(speed);
    }
}

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println();
    Serial.println("Starting...");
    servo1.setPin(servo1Pin);
    delay(500);

}

void loop() {
    servo1.update();
    // Check if servo is moving. If not, queue next action
    if (!servo1.isMoving()) {
        nextServoAction();
    }
}
