/**
* MIT License
*
* Copyright (c) 2019 Arrow Electronics
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
/* Optiga based includes */
#include <optiga/pal/pal_gpio.h>
#include <optiga/pal/pal_i2c.h>

/* Zephyr based includes */
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/i2c.h>

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
/* I2C0 on stk3402a EXP header */
#define I2C_SCL     DT_INST_0_SILABS_GECKO_I2C_LOCATION_SCL_2  /* 11 */
#define I2C_SDA     DT_INST_0_SILABS_GECKO_I2C_LOCATION_SDA_2  /* 10 */
#define I2C_PORT    2   /* Port C */
#define I2C_FREQ_HZ DT_INST_0_SILABS_GECKO_I2C_CLOCK_FREQUENCY /* 100 000 Hz */

#define I2C_OPTIGA_ADDRESS 0x30

/* GPIO on stk3402a EXP header */
#define VDD_PORT_NAME   DT_GPIO_GECKO_PORTB_NAME
#define RST_PORT_NAME   DT_GPIO_GECKO_PORTA_NAME
#define VDD_PIN         8   /* PB8 */
#define RST_PIN         6   /* PA6 */

/*********************************************************************************************************************
 * Context structures
 *********************************************************************************************************************/
/* context for i2c devices */
typedef struct {
    uint8_t  p_port;
    uint8_t  p_scl_io;
    uint8_t  p_sda_io;
    uint32_t p_bitrate;
} i2c_ctx_t;

/* context for gpio devices */
typedef struct {
    uint8_t         p_pin;
    struct device  *p_device;
    uint8_t        *p_port_name;
    uint8_t         p_init_flag;
} gpio_ctx_t;

/* inicialize context for stk3402a I2C and GPIO on EXP header */
i2c_ctx_t stk3402a_i2c_ctx = {
    I2C_PORT,
    I2C_SCL,
    I2C_SDA,
    I2C_FREQ_HZ
};

gpio_ctx_t vdd_stk3402a_gpio_ctx = {
    VDD_PIN,
    NULL,
    VDD_PORT_NAME,
    0
};

gpio_ctx_t rst_stk3402a_gpio_ctx = {
    RST_PIN,
    NULL,
    RST_PORT_NAME,
    0
};

/*********************************************************************************************************************
 * Pal ifx i2c instance
 *********************************************************************************************************************/
/**
 * \brief PAL I2C configuration for OPTIGA.
 */
pal_i2c_t optiga_pal_i2c_context_0 = {
    (void*)&stk3402a_i2c_ctx,   /* context */
    I2C_OPTIGA_ADDRESS,         /* address */
    NULL,                       /* upper layer context */
    NULL                        /* callback event handler */
};

/*********************************************************************************************************************
 * PAL GPIO configurations defined for Pearl Gecko (efm32pg_stk3402a)
 *********************************************************************************************************************/
/**
 * \brief PAL vdd pin configuration for OPTIGA.
 */
pal_gpio_t optiga_vdd_0 = {
    /* platform specific GPIO context for the pin used to toggle Vdd */
    (void*)&vdd_stk3402a_gpio_ctx
};

/**
 * \brief PAL reset pin configuration for OPTIGA.
 */
pal_gpio_t optiga_reset_0 = {
    /* platform specific GPIO context for the pin used to toggle Reset */
    (void*)&rst_stk3402a_gpio_ctx
};

/**
* @}
*/
