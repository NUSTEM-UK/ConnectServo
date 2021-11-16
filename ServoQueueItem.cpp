#include <ServoQueueItem.h>

// ServoQueueItem initializer
ServoQueueItem::ServoQueueItem() {

};

void ServoQueueItem::assign(uint8_t newCall, uint8_t newParam1, uint8_t newAnimationType, uint16_t newServoSpeed, ConnectServo& targetServo) {
    call = newCall;
    param1 = newParam1;
    animationType = newAnimationType;
    servoSpeed = newServoSpeed;
    targetServo = &targetServo;
}
