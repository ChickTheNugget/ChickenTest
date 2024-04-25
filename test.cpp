#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>

// Constants to define GPIO pins
const int DIR_LATCH = 21;  // Example pin for latch
const int DIR_CLK = 20;    // Example pin for clock
const int DIR_SER = 16;    // Example pin for serial

const int MOTOR_PIN[4] = {5, 6, 13, 19}; // Pins for 4 motors

class AMSpi {
public:
    AMSpi() {
        // Initialize wiringPi and set up GPIO pins
        wiringPiSetupGpio(); // Using Broadcom pin numbers

        pinMode(DIR_LATCH, OUTPUT);
        pinMode(DIR_CLK, OUTPUT);
        pinMode(DIR_SER, OUTPUT);

        for (int i = 0; i < 4; ++i) {
            pinMode(MOTOR_PIN[i], OUTPUT);
            softPwmCreate(MOTOR_PIN[i], 0, 100); // Setup PWM within 0-100 range
        }
    }

    ~AMSpi() {
        // Cleanup on destruction
        for (int i = 0; i < 4; ++i) {
            softPwmStop(MOTOR_PIN[i]);
        }
    }

    // Simulates a shift register write function
    void shiftWrite(int value) {
        digitalWrite(DIR_LATCH, LOW);
        for (int i = 0; i < 8; ++i) {
            digitalWrite(DIR_CLK, LOW);
            digitalWrite(DIR_SER, (value & (1 << i)) ? HIGH : LOW);
            digitalWrite(DIR_CLK, HIGH);
        }
        digitalWrite(DIR_LATCH, HIGH);
    }

    void runMotor(int motorIndex, bool clockwise, int speed) {
        // Assuming 'shiftWrite' controls motor direction through a shift register
        int direction = clockwise ? 0b00000001 : 0b00000010; // Example direction bits
        shiftWrite(direction << motorIndex);
        softPwmWrite(MOTOR_PIN[motorIndex], speed);
    }

    void stopMotor(int motorIndex) {
        softPwmWrite(MOTOR_PIN[motorIndex], 0);
    }
};

int main() {
    AMSpi amspi;

    // Example usage
    amspi.runMotor(0, true, 50);  // Run motor 1 clockwise at 50% speed
    delay(2000);                  // Run for 2 seconds
    amspi.stopMotor(0);           // Stop motor 1

    return 0;
}
