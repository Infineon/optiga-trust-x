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

#ifndef OPTIGA_BACKEND_ECC_H__
#define OPTIGA_BACKEND_ECC_H__

#include "sdk_config.h"
#include "nordic_common.h"

#if NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)

#include <stdint.h>
#include <stdbool.h>
#include "nrf_crypto_ecc_shared.h"

/*lint -save -e????*/
#include "optiga/optiga_crypt.h"
/*lint -restore*/


#ifdef __cplusplus
extern "C" {
#endif


/** @internal @brief Common structure holding private key for mbed TLS.
 */
typedef struct
{
    nrf_crypto_internal_ecc_key_header_t header; /**< @internal @brief Common ECC key header */
    optiga_key_id_t oid; // OID where the private key is stored
} nrf_crypto_backend_optiga_ecc_private_key_t;


/** @internal @brief Common structure holding public key for mbed TLS.
 */
typedef struct
{
    nrf_crypto_internal_ecc_key_header_t header; /**< @internal @brief Common ECC key header */
    optiga_key_id_t oid; // OID where the public key is stored
} nrf_crypto_backend_secp256r1_public_key_t;


/** @internal See @ref nrf_crypto_backend_ecc_key_pair_generate_fn_t.
 */
ret_code_t nrf_crypto_backend_optiga_key_pair_generate(
    void * p_context,
    void * p_private_key,
    void * p_public_key);


/** @internal See @ref nrf_crypto_backend_ecc_public_key_calculate_fn_t.
*/
ret_code_t nrf_crypto_backend_optiga_public_key_calculate(
    void       * p_context,
    void const * p_private_key,
    void       * p_public_key);


/** @internal See @ref nrf_crypto_backend_ecc_private_key_from_raw_fn_t.
*/
ret_code_t nrf_crypto_backend_optiga_private_key_from_raw(
    void          * p_private_key,
    uint8_t const * p_raw_data);


/** @internal See @ref nrf_crypto_backend_ecc_private_key_to_raw_fn_t.
*/
ret_code_t nrf_crypto_backend_optiga_private_key_to_raw(
    void    const * p_private_key,
    uint8_t       * p_raw_data);


/** @internal See @ref nrf_crypto_backend_ecc_public_key_from_raw_fn_t.
*/
ret_code_t nrf_crypto_backend_optiga_public_key_from_raw(
    void          * p_public_key,
    uint8_t const * p_raw_data);


/** @internal See @ref nrf_crypto_backend_ecc_public_key_to_raw_fn_t.
*/
ret_code_t nrf_crypto_backend_optiga_public_key_to_raw(
    void const * p_public_key,
    uint8_t    * p_raw_data);


/** @internal See @ref nrf_crypto_backend_ecc_key_free_fn_t.
*/
ret_code_t nrf_crypto_backend_optiga_private_key_free(
    void * p_private_key);


/** @internal See @ref nrf_crypto_backend_ecc_key_free_fn_t.
*/
ret_code_t nrf_crypto_backend_optiga_public_key_free(
    void * p_public_key);

#if NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_ECC_SECP256R1)

#if NRF_MODULE_ENABLED(NRF_CRYPTO_ECC_SECP256R1)
#error "More than one backend enabled for secp256r1 (NIST 256-bit).");
#endif
#define NRF_CRYPTO_ECC_SECP256R1_ENABLED 1

// Aliases for one common OPTIGA implementation
#define nrf_crypto_backend_secp256r1_key_pair_generate    nrf_crypto_backend_optiga_key_pair_generate
#define nrf_crypto_backend_secp256r1_public_key_calculate nrf_crypto_backend_optiga_public_key_calculate
#define nrf_crypto_backend_secp256r1_private_key_from_raw nrf_crypto_backend_optiga_private_key_from_raw
#define nrf_crypto_backend_secp256r1_private_key_to_raw   nrf_crypto_backend_optiga_private_key_to_raw
#define nrf_crypto_backend_secp256r1_public_key_from_raw  nrf_crypto_backend_optiga_public_key_from_raw
#define nrf_crypto_backend_secp256r1_public_key_to_raw    nrf_crypto_backend_optiga_public_key_to_raw
#define nrf_crypto_backend_secp256r1_private_key_free     nrf_crypto_backend_optiga_private_key_free
#define nrf_crypto_backend_secp256r1_public_key_free      nrf_crypto_backend_optiga_public_key_free
// OPTIGA does not require context, so its size is 0.
#define NRF_CRYPTO_BACKEND_SECP256R1_KEY_PAIR_GENERATE_CONTEXT_SIZE    0
#define NRF_CRYPTO_BACKEND_SECP256R1_PUBLIC_KEY_CALCULATE_CONTEXT_SIZE 0
// All OPTIGA curve types share the same private key data structures
typedef nrf_crypto_backend_optiga_ecc_private_key_t nrf_crypto_backend_secp256r1_private_key_t;
// Dummy typedef for unused context
typedef uint32_t nrf_crypto_backend_secp256r1_key_pair_generate_context_t;
typedef uint32_t nrf_crypto_backend_secp256r1_public_key_calculate_context_t;

#endif // NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_ECC_SECP256R1)

#ifdef __cplusplus
}
#endif

#endif // NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)

#endif // OPTIGA_BACKEND_ECC_H__
