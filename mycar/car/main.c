/*
 * Copyright (C) 2021 Otto-von-Guericke-Universität Magdeburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Blinky application
 *
 * @author      Marian Buschsieweke <marian.buschsieweke@ovgu.de>
 *
 * @}
 */

#include <stdio.h>

#include "periph/gpio.h"
#include "xtimer.h"


const gpio_t TRIG=32;
const gpio_t ECHO=33;
const gpio_t L_FRONT=26;
const gpio_t L_BACK=19;
const gpio_t R_FRONT=21;
const gpio_t R_BACK=22;
gpio_t inputs=ECHO;
gpio_t outputs[5]={TRIG,L_BACK,L_FRONT,R_BACK,R_FRONT};

static void delay(int val)
{
    xtimer_usleep(val);
}
static void init_pins(void) {
    int i = 0;
    gpio_init(inputs,GPIO_IN);
    for(i=0;i<5;i++) {
        if(i==0) {
            gpio_init(outputs[i],GPIO_OUT);
        } else {
            gpio_init(outputs[i],GPIO_OD_PU);
        }
    }
}

int main(void)
{
    uint32_t prev, current;
    while (1) {
        init_pins();
        xtimer_init();
        uint32_t read = 1;
        gpio_clear(TRIG);
        delay(2);
        gpio_set(TRIG);
        delay(10);
        gpio_clear(TRIG);
        prev=xtimer_now_usec();
        while(read)read=gpio_read(ECHO);
        current=xtimer_now_usec();
        delay(4);
        current = (current - prev)*59/2000;
        fprintf(stderr,"Current Distance is %lu(cm)\n", current);
        if(current>0 && current < 300) {
        
            if(current<50) {
                gpio_clear(L_FRONT);
                gpio_set(L_BACK);
                gpio_clear(R_FRONT);
                gpio_set(R_BACK);
                delay(500000);
                gpio_set(L_FRONT);
                gpio_clear(L_BACK);
                gpio_clear(R_FRONT);
                gpio_set(R_BACK);
                delay(100000);
            } else {
                gpio_set(L_FRONT);
                gpio_clear(L_BACK);
                gpio_clear(R_BACK);
                gpio_set(R_FRONT);
            }
        }

    }

    return 0;
}
