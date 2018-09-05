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

#ifndef OPTIGA_BACKEND_RNG_H__
#define OPTIGA_BACKEND_RNG_H__

/** @file
 *
 * @defgroup nrf_crypto_nrf_hw_backend_rng nrf_crypto HW RNG backend
 * @{
 * @ingroup nrf_crypto_backends
 *
 * @brief RNG functionality provided by the nrf_crypto nRF HW RNG backend.
 */

#include "sdk_common.h"
#if NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_RNG)

#include "nrf_crypto_rng_shared.h"


#ifdef __cplusplus
extern "C" {
#endif


#if NRF_MODULE_ENABLED(NRF_CRYPTO_RNG)
#error "More than one RNG backend enabled."
#endif // NRF_MODULE_ENABLED(NRF_CRYPTO_RNG)
#define NRF_CRYPTO_RNG_ENABLED 1

/**
 * @internal @brief Context for nRF RNG peripheral.
 */
typedef struct
{
    nrf_crypto_rng_internal_context_t   header;         //!< Internal common context header.
} nrf_crypto_backend_rng_context_t;

/**
 * @internal @brief Dummy temp buffer for nRF RNG peripheral.
 */
typedef struct
{
    uint32_t reserved;
} nrf_crypto_backend_rng_temp_buffer_t;


#ifdef __cplusplus
}
#endif

#endif // NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_RNG)

/**@} */

#endif // OPTIGA_BACKEND_RNG_H__
