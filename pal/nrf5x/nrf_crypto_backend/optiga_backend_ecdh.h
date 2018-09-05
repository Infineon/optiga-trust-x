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

#ifndef OPTIGA_BACKEND_ECDH_H__
#define OPTIGA_BACKEND_ECDH_H__

#include "sdk_config.h"
#include "nordic_common.h"

#if NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)

#include "nrf_crypto_ecc.h"
#include "nrf_crypto_ecdh_shared.h"


#ifdef __cplusplus
extern "C" {
#endif

// This file is only needed to satisfy the nrf_crypto interface


/** @internal See @ref nrf_crypto_backend_ecdh_compute_fn_t.
 */
ret_code_t nrf_crypto_backend_optiga_ecdh_compute(
    void       * p_context,
    void const * p_private_key,
    void const * p_public_key,
    uint8_t    * p_shared_secret);


#if NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_ECC_SECP256R1)
// Aliases for one common OPTIGA implementation
#define nrf_crypto_backend_secp256r1_ecdh_compute nrf_crypto_backend_optiga_ecdh_compute
typedef uint32_t nrf_crypto_backend_secp256r1_ecdh_context_t;
#define NRF_CRYPTO_BACKEND_SECP256R1_ECDH_CONTEXT_SIZE 0
#endif // NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_ECC_SECP256R1)


#ifdef __cplusplus
}
#endif

#endif // NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)

#endif // OPTIGA_BACKEND_ECDH_H__
