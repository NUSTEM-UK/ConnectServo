#include <Arduino.h>
#include <ConnectServo.h>

ConnectServo servo1;


// Give ourselves some placeholder things to work with
ServoQueueItem myItem;
void myFunction1(void *skip, ...);
void myFunction2(void *skip, ...);
int value = 12;
int speed = 30;

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Starting...");
    servo1.attach(D5);
    servo1.write(0);
    delay(500);



    // myItem = {&myFunction1, value, "EASE_CUBIC_IN_OUT", speed};
    myItem.assign(&myFunction1, value, "EASE_CUBIC_IN_OUT", speed);

    // myItem.call = &myFunction1;
    // myItem.param1 = value;
    // myItem.animationType = "EASE_CUBIC_IN_OUT";
    // myItem.servoSpeed = 100;
    // myItem = {&myFunction1, value, 'EASE_CUBIC_IN_OUT', speed};
    servo1.enqueue(myItem);

    // myItem = {&myFunction2, 1, "EASE_BACK_BOUNCINIG", 200};
    myItem.assign(&myFunction2, 1, "EASE_BACK_BOUNCING", 200);

    // myItem.call = &myFunction2;
    // delay(50);
    // myItem.param1 = 1;
    // myItem.animationType = "EASE_BACK_BOUNCING";
    // myItem.servoSpeed = 200;

    // myItem = {&myFunction2, 1, 'EASE_BACK_BOUNCING', 200};
    servo1.enqueue(myItem);

    myItem = servo1.dequeue();
    Serial.print("Dequeued: ");
    // Serial.print(myItem.call);
    // Serial.print(" ");
    Serial.print(myItem.param1);
    Serial.print(" ");
    Serial.print(myItem.animationType);
    Serial.print(" ");
    Serial.println(myItem.servoSpeed);

    myItem = servo1.dequeue();
    Serial.print("Dequeued: ");
    // Serial.print(myItem.call);
    // Serial.print(" ");
    Serial.print(myItem.param1);
    Serial.print(" ");
    Serial.print(myItem.animationType);
    Serial.print(" ");
    Serial.println(myItem.servoSpeed);

}

void loop() {
    servo1.startEaseTo(180, 30);
    while (servo1.isMovingAndCallYield() ) {
        delay(10);
    }
    servo1.write(0);
    while (servo1.isMovingAndCallYield() ) {
        delay(10);
    }
}

void myFunction1(void *skip, ...)
{
    va_list args;
    va_start(args, skip);
    int foo1 = va_arg(args, int);
    Serial.print("myFunction1 = ");
    Serial.println(foo1);
}

void myFunction2(void *skip, ...)
{
    va_list args;
    va_start(args, skip);
    int foo1 = va_arg(args, int);
    float foo2 = (float)va_arg(args, double);

    Serial.print("myFunction2 = ");
    Serial.print(foo1);
    Serial.print(", ");
    Serial.println(foo2);
}
