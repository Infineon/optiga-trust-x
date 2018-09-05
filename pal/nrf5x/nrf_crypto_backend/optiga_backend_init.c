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
*/

#include "sdk_common.h"
#include "sdk_config.h"

#if NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)

#include "nrf_crypto_init.h"
#include "nrf_crypto_rng.h"
#include "nrf_log.h"
/*lint -save -e????*/
#include "optiga/optiga_util.h"
#include "optiga/ifx_i2c/ifx_i2c.h"
/*lint -restore*/


// NOTE(chr): making these global here is not good style
optiga_comms_t optiga_comms = {(void*)&ifx_i2c_context_0,NULL,NULL};
volatile static host_lib_status_t optiga_comms_status;

/*************************************************************************
*  functions
*************************************************************************/
static void optiga_comms_event_handler(void* upper_layer_ctx, host_lib_status_t event)
{
    optiga_comms_status = event;
}

static int32_t optiga_init(void)
{
  int32_t status = (int32_t) OPTIGA_LIB_ERROR;
  sOpenApp_d sOpenApp;

  
  // HACK(chr): need to init PAL, cmd lib seems not to do it

  pal_gpio_init();
  pal_os_event_init();

  do
  {
    // OPTIGA Initialisation phase
    //Invoke optiga_comms_open to initialize the IFX I2C Protocol and security chip
    optiga_comms_status = OPTIGA_COMMS_BUSY;
    optiga_comms.upper_layer_handler = optiga_comms_event_handler;
    status = optiga_comms_open(&optiga_comms);
    if(OPTIGA_COMMS_SUCCESS != status)
    {
      NRF_LOG_INFO("Failure: optiga_comms_open(): 0x%04X", status);
      break;
    }

    NRF_LOG_INFO("Success: optiga_comms_open(): 0x%04X", status);

    //Wait until IFX I2C initialization is complete
    while(optiga_comms_status == OPTIGA_COMMS_BUSY);
    if(optiga_comms_status != OPTIGA_COMMS_SUCCESS)
    {
      NRF_LOG_INFO("Failure: optiga_comms_open(): 0x%04X", optiga_comms_status);
      break;
    }

    status = optiga_util_open_application(&optiga_comms);
    if(OPTIGA_LIB_SUCCESS != status)
    {
      NRF_LOG_INFO("Failure: CmdLib_OpenApplication(): 0x%04X", status);
      break;
    }

    NRF_LOG_INFO("Success: CmdLib_OpenApplication(): 0x%04X", status);

    status = OPTIGA_LIB_SUCCESS;
  } while(0);

  return status;
}

/** @internal @brief Function to initialize OPTIGA backend - open the application.
 */
static ret_code_t optiga_backend_init(void)
{
    if(optiga_init() != OPTIGA_LIB_SUCCESS) {
      return NRF_ERROR_INTERNAL;
    }

    #if defined(NRF_CRYPTO_RNG_AUTO_INIT_ENABLED) && (NRF_CRYPTO_RNG_AUTO_INIT_ENABLED == 1)

    ret_code_t ret_val;
    ret_val = nrf_crypto_rng_init(NULL, NULL);
    return ret_val;

#elif defined(NRF_CRYPTO_RNG_AUTO_INIT_ENABLED) && (NRF_CRYPTO_RNG_AUTO_INIT_ENABLED == 0)

    return NRF_SUCCESS;

#else

    #warning NRF_CRYPTO_RNG_AUTO_INIT_ENABLED define not found in sdk_config.h (Is the sdk_config.h valid?).

#endif // NRF_CRYPTO_RNG_AUTO_INIT_ENABLED

}


/** @internal @brief Function to uninitialize OPTIGA backend - currently no implementation is required.
 */
static ret_code_t optiga_backend_uninit(void)
{
    // Empty implementation
    return NRF_SUCCESS;
}


CRYPTO_BACKEND_REGISTER(nrf_crypto_backend_info_t const optiga_backend) =
{
    .init_fn    = optiga_backend_init,
    .uninit_fn  = optiga_backend_uninit,
};

#endif // NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)
