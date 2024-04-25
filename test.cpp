#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>

class AMSpi {
    static constexpr int DC_Motor_1 = 0;
    static constexpr int DC_Motor_2 = 1;
    static constexpr int DC_Motor_3 = 2;
    static constexpr int DC_Motor_4 = 3;

    int latchPin, clockPin, dataPin;
    int motorPins[4];
    bool isRunning[4] = {false, false, false, false};

public:
    AMSpi() {
        wiringPiSetupGpio(); // Use Broadcom GPIO pin numbers
    }

    void set74HC595Pins(int latch, int clock, int data) {
        latchPin = latch;
        clockPin = clock;
        dataPin = data;
        pinMode(latchPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(dataPin, OUTPUT);
    }

    void setL293DPins(int m1, int m2, int m3, int m4) {
        motorPins[DC_Motor_1] = m1;
        motorPins[DC_Motor_2] = m2;
        motorPins[DC_Motor_3] = m3;
        motorPins[DC_Motor_4] = m4;
        for (int i = 0; i < 4; i++) {
            pinMode(motorPins[i], OUTPUT);
            softPwmCreate(motorPins[i], 0, 100); // Initialize PWM control with range 0-100
        }
    }

    void runMotor(int motorIndex, bool clockwise, int speed) {
        if (clockwise) {
            digitalWrite(motorPins[motorIndex], HIGH);
        } else {
            digitalWrite(motorPins[motorIndex], LOW);
        }
        if (!isRunning[motorIndex]) {
            softPwmWrite(motorPins[motorIndex], speed);
            isRunning[motorIndex] = true;
        }
    }

    void stopMotor(int motorIndex) {
        softPwmWrite(motorPins[motorIndex], 0);
        isRunning[motorIndex] = false;
    }
};
