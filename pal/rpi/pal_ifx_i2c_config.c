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
* \brief This file implements the platform abstraction layer APIs for i2c interface config.
*
* \ingroup  grPAL
* @{
*/

#include "optiga/pal/pal_gpio.h"
#include "optiga/pal/pal_i2c.h"
#include "optiga/ifx_i2c/ifx_i2c_config.h" // IFX_I2C_BASE_ADDR


pal_i2c_t optiga_pal_i2c_context_0 =
{
    // Pointer to I2C master platform specific context
    NULL,
    // Slave address
    IFX_I2C_BASE_ADDR,
    // Upper layer context
    NULL,
    // Callback event handler
    NULL
};


static uint32_t gpio_pin_vdd   = 2;
static uint32_t gpio_pin_reset = 0;

pal_gpio_t optiga_vdd_0 =
{
    (void*) &gpio_pin_vdd
};

pal_gpio_t optiga_reset_0 =
{
    (void*) &gpio_pin_reset
};
