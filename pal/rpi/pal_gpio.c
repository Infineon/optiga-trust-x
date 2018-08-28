/**
* MIT License
*
* Copyright (c) 2018 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
*
* \file
*
* \brief This file implements the platform abstraction layer APIs for gpio.
*
* \ingroup  grPAL
* @{
*/

#include "optiga/ifx_i2c/ifx_i2c_config.h"
#include "optiga/pal/pal_gpio.h"
#include "wiringPi.h"
#include "optiga/pal/pal_ifx_i2c_config.h"

#if IFX_I2C_LOG_PAL == 1
#include <stdio.h>
#define LOG(...)  printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define LOG_PREFIX "[IFX-PAL-GPIO] "


void pal_gpio_init(void)
{
	LOG(LOG_PREFIX "pal_gpio_init() >\n");

    uint32_t pin_reset = *((uint32_t*) optiga_reset_0.p_gpio_hw);
    uint32_t pin_vdd   = *((uint32_t*) optiga_vdd_0.p_gpio_hw);

    wiringPiSetup();

    // Init pins
    pinMode(pin_reset, OUTPUT);
    pinMode(pin_vdd,   OUTPUT);

    // Enable power for the *on-board* Trust X device
    digitalWrite(pin_vdd, HIGH);

    LOG(LOG_PREFIX "pal_gpio_init() <\n");
}


void pal_gpio_set_high(const pal_gpio_t* p_gpio_context)
{
    uint32_t pin;

    if (p_gpio_context != NULL && p_gpio_context->p_gpio_hw != NULL)
    {
        pin = *((uint32_t*)(p_gpio_context->p_gpio_hw));
        pinMode(pin, OUTPUT);
        LOG(LOG_PREFIX "pal_gpio_set_high(...): pin=0x%x\n", pin);
        digitalWrite(pin, HIGH);
    }
}


void pal_gpio_set_low(const pal_gpio_t* p_gpio_context)
{
    uint32_t pin;

    if (p_gpio_context != NULL && p_gpio_context->p_gpio_hw != NULL)
    {
        pin = *((uint32_t*)(p_gpio_context->p_gpio_hw));
        pinMode(pin, OUTPUT);
        LOG(LOG_PREFIX "[PAL-GPIO] pal_gpio_set_low(...): pin=0x%x\n", pin);
        digitalWrite(pin, LOW);
    }
}
