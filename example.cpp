#include "AMSpi.h"
#include <unistd.h>  // For sleep()

int main() {
    AMSpi amspi;

    amspi.set74HC595Pins(21, 20, 16);
    amspi.setL293DPins(5, 6, 13, 19);

    std::cout << "GO: clockwise" << std::endl;
    for (int i = AMSpi::DC_Motor_1; i <= AMSpi::DC_Motor_4; ++i) {
        amspi.runMotor(i, true, 100);
    }
    sleep(2);

    std::cout << "Stop" << std::endl;
    for (int i = AMSpi::DC_Motor_1; i <= AMSpi::DC_Motor_4; ++i) {
        amspi.stopMotor(i);
    }
    sleep(1);

    std::cout << "GO: counterclockwise" << std::endl;
    for (int i = AMSpi::DC_Motor_1; i <= AMSpi::DC_Motor_4; ++i) {
        amspi.runMotor(i, false, 100);
    }
    sleep(2);

    std::cout << "Stop" << std::endl;
    for (int i = AMSpi::DC_Motor_1; i <= AMSpi::DC_Motor_4; ++i) {
        amspi.stopMotor(i);
    }
    sleep(1);

    return 0;
}
