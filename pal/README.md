# Porting Guide

[tocstart]: # (toc start)

Contents:
  1. [Update I2C master platform specific context](#i2c_master)
  2. [Update platform specific context for GPIOs](#platform_spec_context)
  3. [Update PAL I2C APIs](#pal_i2c_api)
  4. [Update PAL GPIO API](#pal_gpio_api)
  5. [Update PAL Timer API](#pal_os_timer_api)
  6. [Update Event management](#pal_os_event_api)
  7. [Initialisation](#initialisation)

[tocend]: # (toc end)

The Platform Abstraction Layer (PAL) APIs have to be updated to integrate the OPTIGA™ Trust X host libraries
in the local host target platform.
The PAL reference code for the XMC4500 Relax kit (with DAVE™ IDE) is provided as a reference. The
implementation can be referred in “<repo_root>/pal/xmc_dave” and the header files are available in
“<repo_root>/optiga/include” with the required APIs used by upper layers. The header files are platform
agnostic and would not require any change. The low level drivers used by PAL for XMC4500 are configured and generated using DAVE™ IDE.

The hardware/platform resource configuration with respect to I<sup>2</sup>C master and GPIOs (Vdd and Reset) are to be
updated in pal_ifx_i2c_config.c. These configurations are used by the IFX I2C implementation to communicate
with OPTIGA™ Trust X.

<a name="i2c_master"></a>
## Update I2C master platform specific context (e.g. `(void*)&i2c_master_0`)

```c
/**
* \brief PAL I2C configuration for OPTIGA
*/
pal_i2c_t optiga_pal_i2c_context_0 =
{
    /// Pointer to I2C master **platform specific context**
    (void*)&i2c_master_0,
    /// Slave address
    0x30,
    /// Upper layer context
    NULL,
    /// Callback event handler
    NULL
};
```
Not all hardware platforms do support transmission of big I<sup>2</sup>C frames. OPTIGA™ Trust X hostcode is pre-configured to use at least 277 bytes per I<sup>2</sup>C frame (this usually limits internal transmit and receive I<sup>2</sup>C buffers). In case of shorter frame buffers, you will need then either modify the internal buffer sizes for I<sup>2</sup>C module (preferably), or modify the `DL_MAX_FRAME_SIZE` macro (300 bytes by default) in the _<repo\_root>/optiga/include/optiga/ifx\_i2c/ifx\_i2c\_config.h_ file

<a name="platform_spec_context"></a>
## Update platform specific context for GPIOs (Vdd and Reset) [e.g. (void*)&pin_3_4]
```c
/**
* \brief Vdd pin configuration for OPTIGA
*/
pal_gpio_t optiga_vdd_0 =
{
    // Platform specific GPIO context for the pin used to toggle Vdd
    (void*)&pin_3_4
};

/**
* \brief Reset pin configuration for OPTIGA
*/
pal_gpio_t optiga_reset_0 =
{
    // Platform specific GPIO context for the pin used to toggle Reset
    (void*)&pin_3_3
};
```

<a name="pal_i2c_api"></a>
## Update PAL I2C APIs [pal_i2c.c] to communicate with OPTIGA™ Trust X

The pal_i2c is expected to provide the APIs for I2C driver initialization, de-initialization, read, write and set
bitrate kind of operations
    * `pal_i2c_init`
    * `pal_i2c_deinit`
    * `pal_i2c_read`
    * `pal_i2c_write`
    * `pal_i2c_set_bitrate` (optional)

In few target platforms, the I2C master driver initialization (pal_i2c_init) is done during the platform start up. In
such an environment, there is no need to implement pal_i2c_init and pal_i2c_deinit functions. Otherwise,
these (pal_i2c_init & pal_i2c_deinit) functions must be implemented as per the upper layer expectations based
on the need. The details of these expectations are available in the Host library API documentation (chm).
The reference implementation of PAL I2C based on XMC4500 Relax kit does not need to have the platform I2C
driver initialization explicitly done as part of pal_i2c_init as it is taken care by the DAVE™ library initialization.
Hence pal_i2c_init & pal_i2c_deinit are not implemented.

In addition to the above specified APIs, the PAL I2C must handle the events from the low level I2C driver and
invoke the upper layer handlers registered with PAL I2C context for the respective transaction as shown in the
below example.
```c
//I2C driver callback function when the transmit is completed successfully
void i2c_master_end_of_transmit_callback(void)
{
    invoke_upper_layer_callback(gp_pal_i2c_current_ctx, (uint8_t)PAL_I2C_EVENT_TX_SUCCESS);
}

// I2C driver callback function when the receive is completed successfully
void i2c_master_end_of_receive_callback(void)
{
    invoke_upper_layer_callback(gp_pal_i2c_current_ctx, (uint8_t)PAL_I2C_EVENT_SUCCESS);
}
```
In above example the I2C driver callback, when transmit is successful invokes the handler to inform the result

<a name="pal_gpio_api"></a>
## Update PAL GPIO API [pal_gpio.c] to power on and reset the OPTIGA™ Trust X
    * `pal_gpio_set_high`
    * `pal_gpio_set_low`

<a name="pal_os_timer_api"></a>
## Update PAL Timer API [pal_os_timer.c] to enable timer
    * `pal_os_timer_get_time_in_milliseconds`
    * `pal_os_timer_delay_in_milliseconds`

<a name="pal_os_event_api"></a>
## Update Event management for the asynchronous interactions for IFX I2C [pal_os_event.c]
    * `pal_os_event_register_callback_oneshot`
    * `pal_os_event_trigger_registered_callback`

The pal_os_event_register_callback_oneshot function is expected to register the handler and context
provided as part of input parameters and triggers the timer for the requested time.

```c
void pal_os_event_register_callback_oneshot( register_callback callback, void* callback_args, uint32_t time_us)
{
    callback_registered = callback;
    callback_ctx = callback_args;

    //lint --e{534} suppress "Return value is not required to be checked"
    TIMER_SetTimeInterval(&scheduler_timer , (time_us*100));

    TIMER_Start(&scheduler_timer);
}
```

And the handler registered must be invoked once the timer is elapsed as shown in
pal_os_event_trigger_registered_callback. The pal_os_event_trigger_registered_callback is to be
registered with event timer interrupt to get trigerred when timer expires.
```c
void pal_os_event_trigger_registered_callback(void)
{
    TIMER_ClearEvent(&scheduler_timer);
    //lint --e{534} suppress "Return value is not required to be checked"
    TIMER_Stop(&scheduler_timer);
    TIMER_Clear(&scheduler_timer);

    if (callback_registered)
    {
        callback_registered((void*)callback_ctx);
    }
}
```

Other PAL implementations according to this guide can be found inside the [<repo_root>/pal](https://github.com/Infineon/optiga-trust-x/tree/develop/pal) folder 

## Initialisation

When you succesfully port the PAL on your target platform, you may run the following example to verify whether your code is working.

Depending on a selected platform, the initialization routine might vary.
The function `optiga_init()` should be called before any call to Crypt API or Util API
Below is an examples which work for most of platforms:

```c
#include "optiga/optiga_util.h"
#include "optiga/ifx_i2c/ifx_i2c_config.h"

/* The communication context `ifx_i2c_context_0` is declared in the header file ifx_i2c_config.h */
optiga_comms_t optiga_comms = {(void*)&ifx_i2c_context_0,NULL,NULL, OPTIGA_COMMS_SUCCESS};

static int32_t optiga_init(void)
{
    int32_t status = (int32_t) OPTIGA_LIB_ERROR;

    do
    {
        /* Optionally you might need to initialize some of the Trust X PAL functions. If you have them implemented
           In this case they should be declared as extern.
           pal_gpio_init();
           pal_os_event_init();
        */
        status = optiga_util_open_application(&optiga_comms);
        if(OPTIGA_LIB_SUCCESS != status)
        {
            printf( ("Failure: CmdLib_OpenApplication(): 0x%04X\n\r", status) );
            break;
        }

        status = OPTIGA_LIB_SUCCESS;
    } while(0);

    return status;
}

```

