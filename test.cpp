#include <stdio.h>
#include <wiringPi.h>

#define DC_Motor_1 1
#define DC_Motor_2 2
#define DC_Motor_3 3
#define DC_Motor_4 4

int DIR_LATCH, DIR_CLK, DIR_SER;

int MOTORS[4][8] = {
    {0, 4, 8, 12, 16, 20, 24, 28},
    {0, 2, 4, 6, 8, 10, 12, 14},
    {0, 32, 64, 96, 128, 160, 192, 224},
    {0, 1, 2, 3, 4, 5, 6, 7}
};

void shiftWrite(int value) {
    digitalWrite(DIR_LATCH, LOW);
    for (int i = 7; i >= 0; i--) {
        digitalWrite(DIR_CLK, LOW);
        digitalWrite(DIR_SER, (value >> i) & 1);
        digitalWrite(DIR_CLK, HIGH);
    }
    digitalWrite(DIR_LATCH, HIGH);
}

void set74HC595Pins(int latch, int clk, int ser) {
    DIR_LATCH = latch;
    DIR_CLK = clk;
    DIR_SER = ser;

    pinMode(DIR_LATCH, OUTPUT);
    pinMode(DIR_CLK, OUTPUT);
    pinMode(DIR_SER, OUTPUT);
}

void setL293DPins(int pwm0a, int pwm0b, int pwm2a, int pwm2b) {
    MOTORS[0][0] = pwm0a;
    MOTORS[1][0] = pwm0b;
    MOTORS[2][0] = pwm2a;
    MOTORS[3][0] = pwm2b;

    if (pwm0a != -1) {
        pinMode(pwm0a, OUTPUT);
    }
    if (pwm0b != -1) {
        pinMode(pwm0b, OUTPUT);
    }
    if (pwm2a != -1) {
        pinMode(pwm2a, OUTPUT);
    }
    if (pwm2b != -1) {
        pinMode(pwm2b, OUTPUT);
    }
}

void runDCMotor(int dcMotor, int clockwise, int speed) {
    int direction = clockwise ? 0 : 1;
    int allMotorsDirection = MOTORS[dcMotor - 1][direction + 1];
    for (int i = 0; i < 4; i++) {
        if (i == dcMotor - 1) {
            continue;
        }
        if (MOTORS[i][7] != -1) {
            allMotorsDirection += MOTORS[i][7];
        }
    }
    shiftWrite(allMotorsDirection);

    if (speed == -1) {
        if (MOTORS[dcMotor - 1][0] != -1) {
            digitalWrite(MOTORS[dcMotor - 1][0], HIGH);
        }
    } else if (speed >= 0 && speed <= 100) {
        if (MOTORS[dcMotor - 1][0] != -1) {
            analogWrite(MOTORS[dcMotor - 1][0], (speed * 1023) / 100);
        }
    }
}

void stopDCMotor(int dcMotor) {
    int allMotorsDirection = MOTORS[dcMotor - 1][7];
    for (int i = 0; i < 4; i++) {
        if (i == dcMotor - 1) {
            continue;
        }
        if (MOTORS[i][7] != -1) {
            allMotorsDirection += MOTORS[i][7];
        }
    }
    shiftWrite(allMotorsDirection);

    if (MOTORS[dcMotor - 1][0] != -1) {
        digitalWrite(MOTORS[dcMotor - 1][0], LOW);
    }
}

int main() {
    wiringPiSetupGPIO();
    // Set PINs for controlling shift register
    set74HC595Pins(21, 20, 16);
    // Set PINs for controlling all 4 motors
    setL293DPins(5, 6, 13, 19);

    printf("GO: clockwise\n");
    runDCMotor(DC_Motor_1, 1, -1);
    delay(2000);

    printf("Stop\n");
    stopDCMotor(DC_Motor_1);
    delay(1000);

    printf("GO: counterclockwise\n");
    runDCMotor(DC_Motor_1, 0, -1);
    delay(2000);

    printf("Stop\n");
    stopDCMotor(DC_Motor_1);
    delay(1000);

    printf("GO: clockwise with 50%% of maximum speed\n");
    runDCMotor(DC_Motor_1, 1, 50);
    delay(2000);

    printf("Stop\n");
    stopDCMotor(DC_Motor_1);
    delay(1000);

    printf("Stop and Exit\n");
    stopDCMotors(DC_Motor_1, DC_Motor_2, DC_Motor_3, DC_Motor_4);

    return 0;
}
