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
* \brief This file implements the platform abstraction layer APIs for gpio.
*
* \ingroup  grPAL
*
* @{
*/

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
/* Optiga based includes */
#include <optiga/pal/pal_gpio.h>

/* Zephyr based includes */
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
#define PIN_LOW     0
#define PIN_HIGH    1

/**********************************************************************************************************************
 * LOCAL DATA
 *********************************************************************************************************************/
/* gpio context */
typedef struct {
    uint8_t         p_pin;
    struct device  *p_device;
    uint8_t        *p_port_name;
    uint8_t         p_init_flag;
} gpio_ctx_t;

/**********************************************************************************************************************
 * LOCAL ROUTINES
 *********************************************************************************************************************/
static void pal_init_gpio(gpio_ctx_t* ctx);

static void pal_init_gpio(gpio_ctx_t* ctx)
{
    ctx->p_device = device_get_binding(ctx->p_port_name);

    if (gpio_pin_configure(ctx->p_device, ctx->p_pin, GPIO_DIR_OUT) == 0) {
        ctx->p_init_flag = 1;
    }
}

/**********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

/**
* Sets the gpio pin to high state
*
* <b>API Details:</b>
*      The API sets the pin high, only if the pin is assigned to a
*      valid gpio context.<br>
*      Otherwise the API returns without any faliure status.<br>
*
*\param[in] p_gpio_context Pointer to pal layer gpio context
*/
void pal_gpio_set_high(const pal_gpio_t* p_gpio_context)
{
    gpio_ctx_t *current_ctx;
    if ((p_gpio_context != NULL) && (p_gpio_context->p_gpio_hw != NULL)) {
        current_ctx = (gpio_ctx_t*)(p_gpio_context->p_gpio_hw);
        if (current_ctx->p_init_flag == 0) {
            pal_init_gpio(current_ctx);
        }
        gpio_pin_write(current_ctx->p_device, current_ctx->p_pin, PIN_HIGH);
    }
}

/**
* Sets the gpio pin to low state
*
* <b>API Details:</b>
*      The API set the pin low, only if the pin is assigned to a
*      valid gpio context.<br>
*      Otherwise the API returns without any faliure status.<br>
*
*\param[in] p_gpio_context Pointer to pal layer gpio context
*/
void pal_gpio_set_low(const pal_gpio_t* p_gpio_context)
{
    gpio_ctx_t *current_ctx;
    if ((p_gpio_context != NULL) && (p_gpio_context->p_gpio_hw != NULL)) {
        current_ctx = (gpio_ctx_t*)(p_gpio_context->p_gpio_hw);
        if (current_ctx->p_init_flag == 0) {
            pal_init_gpio(current_ctx);
        }
        gpio_pin_write(current_ctx->p_device, current_ctx->p_pin, PIN_LOW);
    }
}

/**
* @}
*/
