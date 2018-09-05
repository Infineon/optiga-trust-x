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
#if NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_RNG)

#include "nrf_crypto_rng.h"
#include "optiga_backend_rng.h"
#include "optiga/optiga_crypt.h"


ret_code_t nrf_crypto_rng_backend_init(void * const p_context,
                                       void * const p_temp_buffer)
{    
    UNUSED_PARAMETER(p_context);
    UNUSED_PARAMETER(p_temp_buffer);

    return NRF_SUCCESS;
}


ret_code_t nrf_crypto_rng_backend_uninit(void * const p_context)
{
    UNUSED_PARAMETER(p_context);

    return NRF_SUCCESS;
}

// From Solution Reference Manual v1.35, section 4.4.3.4

// The OPTIGA always generates at least 8 bytes of random data
#define OPTIGA_RNG_MIN_SIZE 0x8
// The OPTIGA can at most generate 0x100 bytes of random data at once
#define OPTIGA_RNG_MAX_SIZE 0x100
ret_code_t nrf_crypto_rng_backend_vector_generate(void      * const p_context,
                                                  uint8_t   * const p_target,
                                                  size_t            size,
                                                  bool              use_mutex)
{
    UNUSED_PARAMETER(use_mutex);
    UNUSED_PARAMETER(p_context);

    
    uint8_t backup[OPTIGA_RNG_MIN_SIZE] = {0};
    optiga_lib_status_t err;

    if(size < OPTIGA_RNG_MIN_SIZE) {
      err = optiga_crypt_random(OPTIGA_RNG_TYPE_TRNG, backup, OPTIGA_RNG_MIN_SIZE);
      if(err != OPTIGA_LIB_SUCCESS) {
        return NRF_ERROR_CRYPTO_INTERNAL;
      }

      memcpy(p_target, backup, size);
      return NRF_SUCCESS;
    }

    uint8_t * out_cur = p_target;
    uint8_t * out_end = p_target + size;
    while((out_end - out_cur) > OPTIGA_RNG_MAX_SIZE) {
      err = optiga_crypt_random(OPTIGA_RNG_TYPE_TRNG, out_cur, OPTIGA_RNG_MAX_SIZE);
      if(err != OPTIGA_LIB_SUCCESS) {
        return NRF_ERROR_CRYPTO_INTERNAL;
      }

      out_cur += OPTIGA_RNG_MAX_SIZE;
    }

    err = optiga_crypt_random(OPTIGA_RNG_TYPE_TRNG, out_cur, out_end - out_cur);

    if(err != OPTIGA_LIB_SUCCESS) {
      return NRF_ERROR_CRYPTO_INTERNAL;
    }

    return NRF_SUCCESS;
}


ret_code_t nrf_crypto_rng_backend_reseed(void   * const p_context,
                                         void         * p_temp_buffer,
                                         uint8_t      * p_input_data,
                                         size_t         size)
{
    UNUSED_PARAMETER(p_context);
    UNUSED_PARAMETER(p_temp_buffer);
    UNUSED_PARAMETER(p_input_data);
    UNUSED_PARAMETER(size);
    
    return NRF_ERROR_CRYPTO_FEATURE_UNAVAILABLE;
}

#endif //NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_RNG)
