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
* \brief This file implements platform abstraction layer configurations for ifx i2c protocol.
*
* \ingroup  grPAL
* @{
*/

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include "mbed.h"
extern "C" {
	#include "optiga/pal/pal_gpio.h"
	#include "optiga/pal/pal_i2c.h"
}


/*********************************************************************************************************************
 * pal ifx i2c instance
 *********************************************************************************************************************/
/*!< I2C master clock frequency */
#ifndef CONFIG_PAL_I2C_MASTER_FREQ_HZ
	#define PAL_I2C_MASTER_FREQ_HZ          400000           
#else
	#define PAL_I2C_MASTER_FREQ_HZ			CONFIG_PAL_I2C_MASTER_FREQ_HZ
#endif

typedef struct mbedos_i2c_ctx {
	I2C*     p_i2c;
	uint32_t bitrate;
} mbedos_i2c_ctx_t;

I2C i2c_ctx_0(I2C_SDA, I2C_SCL);


mbedos_i2c_ctx_t mbedos_i2c_ctx_0 = { &i2c_ctx_0, PAL_I2C_MASTER_FREQ_HZ };

/**
 * \brief PAL I2C configuration for OPTIGA. 
 */
pal_i2c_t optiga_pal_i2c_context_0 =
{
    /// Pointer to I2C master platform specific context
    (void*)&mbedos_i2c_ctx_0,
    /// Slave address
    0x30,
    /// Upper layer context
    NULL,
    /// Callback event handler
    NULL
};

/*********************************************************************************************************************
 * PAL GPIO configurations defined for XMC4500
 *********************************************************************************************************************/
/**
* \brief PAL vdd pin configuration for OPTIGA. 
 */
pal_gpio_t optiga_vdd_0 =
{
    // Platform specific GPIO context for the pin used to toggle Vdd.
    (void*)NULL  
};

/**
 * \brief PAL reset pin configuration for OPTIGA.
 */
pal_gpio_t optiga_reset_0 =
{
    // Platform specific GPIO context for the pin used to toggle Reset.
    (void*)NULL
};


/**
* @}
*/

