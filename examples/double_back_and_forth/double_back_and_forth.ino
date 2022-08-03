#include <ConnectServo.h>

ConnectServo servo1;
ConnectServo servo2;

static const uint8_t servo1Pin = D4;
int servo1_min = 4;
int servo1_max = 54;
boolean servo1_going_up = true;

static const uint8_t servo2Pin = D5;
int servo2_min = 90;
int servo2_max = 176;
boolean servo2_going_up = true;

void servo1nextAction() {
    // Set speed to random int between 20 and 150
    int speed = random(10, 100);
    if (servo1_going_up) {
        servo1.queueEaseTo(servo1_min, EASE_CUBIC_IN_OUT, speed);
        servo1_going_up = false;
        Serial.print("Servo 1 going down at: ");
        Serial.println(speed);
    } else {
        servo1.queueEaseTo(servo1_max, EASE_CUBIC_IN_OUT, speed);
        servo1_going_up = true;
        Serial.print("Servo 1 going up at: ");
        Serial.println(speed);
    }
}

void servo2nextAction() {
    // Set speed to random int between 20 and 150
    int speed = random(10, 100);
    if (servo2_going_up) {
        servo2.queueEaseTo(servo2_min, EASE_CUBIC_IN_OUT, speed);
        servo2_going_up = false;
        Serial.print("Servo 2 going down at: ");
        Serial.println(speed);
    } else {
        servo2.queueEaseTo(servo2_max, EASE_CUBIC_IN_OUT, speed);
        servo2_going_up = true;
        Serial.print("Servo 2 going up at: ");
        Serial.println(speed);
    }
}

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println();
    Serial.println("Starting...");
    servo1.setPin(servo1Pin);
    servo2.setPin(servo2Pin);
    delay(500);

}

void loop() {
    servo1.update();
    servo2.update();
    // Check if servo is moving. If not, queue next action
    if (!servo1.isMoving()) {
        servo1nextAction();
    }
    if (!servo2.isMoving()) {
        servo2nextAction();
    }
}
