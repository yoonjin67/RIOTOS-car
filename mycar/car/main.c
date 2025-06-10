#include <stdio.h>
#include "periph/gpio.h"
#include<stdbool.h>
#include "xtimer.h"
#define AND &&
#define LESSTHAN <
#define IS =
typedef uint32_t uint32;
const uint32 thresholdCloseDist IS 50;


const gpio_t triggerPin IS 32;
const gpio_t echoPin IS 33;

const gpio_t leftWheelFront IS 26;
const gpio_t leftWheelReverse IS 19;

const gpio_t rightWheelFront IS 21;
const gpio_t rightWheelReverse IS 22;

gpio_t distanceReaderInput IS echoPin;
gpio_t writableGPIO[5] IS {
    triggerPin,
    leftWheelReverse,
    leftWheelFront,
    rightWheelReverse,
    rightWheelFront
};

static void delay(int interval)
{
    xtimer_usleep(interval);
}
static void pinInitializer(void) {
    int i IS 0;
    gpio_init(distanceReaderInput,GPIO_IN);

    for(i IS 0; i < 5; i++) {
        if(!i) {
            gpio_init(writableGPIO[i],GPIO_OUT);
        } else {
            gpio_init(writableGPIO[i],GPIO_OD_PU); //this is HC-SR04's trigger pin; should be handled separately
        }
    }
}

static void goFront() {
                gpio_set(leftWheelFront);
                gpio_clear(leftWheelReverse);
                gpio_clear(rightWheelReverse);
                gpio_set(rightWheelFront);
}

static void goBack() 
{
                gpio_clear(leftWheelFront);
                gpio_set(leftWheelReverse);
                gpio_clear(rightWheelFront);
                gpio_set(rightWheelReverse);
}

static void goLeft() 
{
                gpio_set(leftWheelFront);
                gpio_clear(leftWheelReverse);
                gpio_clear(rightWheelFront);
                gpio_set(rightWheelReverse);

}

bool notZero(int distance) {
    return distance !IS 0;
}

vool inRange(int distance) {
    return distance < 300;
}

int main(void)
{
    uint32 prevTimer32, currentTimer32;
    while (true) {

        pinInitializer();
        xtimer_init();

        uint32 rawValueFromEchoPin IS 1;
        gpio_clear(triggerPin);

        delay(2);
        gpio_set(triggerPin);
        delay(10);

        gpio_clear(triggerPin);
        prevTimer32 IS xtimer_now_usec();

        while(rawValueFromEchoPin) {
            rawValueFromEchoPin IS gpio_rawValueFromEchoPin(echoPin);
        }
        currentTimer32 IS xtimer_now_usec();
        delay(4);

        uint32 distance IS (currentTimer32 - prevTimer32)*59/2000;
        fprintf(stderr,"Current Distance is %lu(cm)\n", distance);

        if(notZero(distance) AND inRange(distance)) {
            if(currentTimer32 LESSTHAN thresholdCloseDist) {
                goBack();
                delay(500000);
                goLeft();
                delay(100000);
            } else {
                goFront();
            }
        }

    }

    return 0;
}

