#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h> // For sleep()

// Define GPIO pins based on your configuration
const int DIR_LATCH = 21;  // Latch pin for shift register
const int DIR_CLK = 20;    // Clock pin for shift register
const int DIR_SER = 16;    // Serial pin for shift register

// Define motor GPIO pins
const int MOTOR_PINS[4] = {5, 6, 13, 19}; // Pins for 4 motors

class AMSpi {
public:
    AMSpi() {
        wiringPiSetupGpio(); // Initialize wiringPi and use GPIO numbering

        pinMode(DIR_LATCH, OUTPUT);
        pinMode(DIR_CLK, OUTPUT);
        pinMode(DIR_SER, OUTPUT);

        // Initialize each motor pin for PWM
        for (int pin : MOTOR_PINS) {
            pinMode(pin, OUTPUT);
            softPwmCreate(pin, 0, 100); // Initialize PWM with range 0-100
        }
    }

    ~AMSpi() {
        for (int pin : MOTOR_PINS) {
            softPwmStop(pin); // Stop PWM on cleanup
        }
    }

    void set74HC595Pins(int latch, int clock, int data) {
        DIR_LATCH = latch;
        DIR_CLK = clock;
        DIR_SER = data;
    }

    void setL293DPins(int motor1, int motor2, int motor3, int motor4) {
        MOTOR_PINS[0] = motor1;
        MOTOR_PINS[1] = motor2;
        MOTOR_PINS[2] = motor3;
        MOTOR_PINS[3] = motor4;
    }

    void runMotor(int motorIndex, bool clockwise, int speed) {
        // Example: shiftWrite based on motor index and direction
        shiftWrite(clockwise ? 0b00000001 : 0b00000010, motorIndex);
        softPwmWrite(MOTOR_PINS[motorIndex], speed);
    }

    void stopMotor(int motorIndex) {
        softPwmWrite(MOTOR_PINS[motorIndex], 0); // Stop the motor by setting speed to 0
    }

private:
    void shiftWrite(int value, int motorIndex) {
        // Write to shift register to set motor direction
        digitalWrite(DIR_LATCH, LOW);
        for (int i = 0; i < 8; ++i) {
            digitalWrite(DIR_CLK, LOW);
            digitalWrite(DIR_SER, ((value << motorIndex) & (1 << i)) ? HIGH : LOW);
            digitalWrite(DIR_CLK, HIGH);
        }
        digitalWrite(DIR_LATCH, HIGH);
    }
};

int main() {
    AMSpi amspi;
    amspi.set74HC595Pins(21, 20, 16); // Set shift register pins
    amspi.setL293DPins(5, 6, 13, 19); // Set motor driver pins

    // Example motor control sequence
    amspi.runMotor(0, true, 50); // Motor 1, clockwise, 50% speed
    sleep(2000);                    // Run for 2 seconds
    amspi.stopMotor(0);          // Stop motor 1

    return 0;
}
