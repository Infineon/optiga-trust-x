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

#include "sdk_config.h"
#include "nordic_common.h"

#if NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)

#include <stdbool.h>
#include <string.h>

#include "nrf_crypto_ecc.h"
#include "nrf_crypto_ecdh.h"
#include "nrf_crypto_mem.h"
#include "nrf_crypto_rng.h"
#include "nrf_crypto_shared.h"
#include "nrf_assert.h"
#include "optiga_backend_ecc.h"

/*lint -save -e????*/
#include "optiga/optiga_crypt.h"
/*lint -restore*/

int nrf_crypto_backend_optiga_ecc_optiga_rng(void * p_param, unsigned char * p_data, size_t size)
{
#if NRF_MODULE_ENABLED(NRF_CRYPTO_RNG)
    return 0;

#else
    return NRF_ERROR_CRYPTO_FEATURE_UNAVAILABLE;
#endif
}


ret_code_t nrf_crypto_backend_optiga_key_pair_generate(
    void * p_context,
    void * p_private_key,
    void * p_public_key)
{

    return NRF_SUCCESS;
}


ret_code_t nrf_crypto_backend_optiga_public_key_calculate(
    void       * p_context,
    void const * p_private_key,
    void       * p_public_key)
{

    return NRF_ERROR_CRYPTO_INTERNAL;
}


ret_code_t nrf_crypto_backend_optiga_private_key_from_raw(
    void          * p_private_key,
    uint8_t const * p_raw_data)
{

    nrf_crypto_backend_optiga_ecc_private_key_t * p_prv =
            (nrf_crypto_backend_optiga_ecc_private_key_t *)p_private_key;

    nrf_crypto_ecc_curve_info_t const * p_info = p_prv->header.p_info;

    uint8_t const * p_input = p_raw_data;

    STATIC_ASSERT(sizeof(optiga_key_id_t) == 2);
    if (p_info == &g_nrf_crypto_ecc_secp256r1_curve_info) {
        p_prv->oid = p_raw_data[0] | p_raw_data[1] << 8;
        return NRF_SUCCESS;
    }

    return NRF_ERROR_CRYPTO_INTERNAL;
}


ret_code_t nrf_crypto_backend_optiga_private_key_to_raw(
    void    const * p_private_key,
    uint8_t       * p_raw_data)
{
    nrf_crypto_backend_optiga_ecc_private_key_t * p_prv =
            (nrf_crypto_backend_optiga_ecc_private_key_t *)p_private_key;

    nrf_crypto_ecc_curve_info_t const * p_info = p_prv->header.p_info;

    uint8_t const * p_input = p_raw_data;

    STATIC_ASSERT(sizeof(optiga_key_id_t) == 2);
    if (p_info == &g_nrf_crypto_ecc_secp256r1_curve_info) {
        p_raw_data[0] = p_prv->oid & 0xFF;
        p_raw_data[1] = (p_prv->oid >> 8) & 0xFF;
        return NRF_SUCCESS;
    }

    return NRF_ERROR_CRYPTO_INTERNAL;
}


ret_code_t nrf_crypto_backend_optiga_public_key_from_raw(
    void          * p_public_key,
    uint8_t const * p_raw_data)
{
    nrf_crypto_backend_secp256r1_public_key_t * p_pub =
            (nrf_crypto_backend_secp256r1_public_key_t *)p_public_key;

    nrf_crypto_ecc_curve_info_t const * p_info = p_pub->header.p_info;

    uint8_t const * p_input = p_raw_data;

    STATIC_ASSERT(sizeof(optiga_key_id_t) == 2);
    if (p_info == &g_nrf_crypto_ecc_secp256r1_curve_info) {
        p_pub->oid = p_raw_data[0] | (p_raw_data[1] << 8);
        return NRF_SUCCESS;
    }

    return NRF_ERROR_CRYPTO_INTERNAL;
}


ret_code_t nrf_crypto_backend_optiga_public_key_to_raw(
    void const * p_public_key,
    uint8_t    * p_raw_data)
{

    nrf_crypto_backend_secp256r1_public_key_t * p_pub =
            (nrf_crypto_backend_secp256r1_public_key_t *)p_public_key;

    nrf_crypto_ecc_curve_info_t const * p_info = p_pub->header.p_info;

    uint8_t const * p_input = p_raw_data;

    STATIC_ASSERT(sizeof(optiga_key_id_t) == 2);
    if (p_info == &g_nrf_crypto_ecc_secp256r1_curve_info) {
        p_raw_data[0] = p_pub->oid & 0xFF;
        p_raw_data[1] = (p_pub->oid >> 8) & 0xFF;
        return NRF_SUCCESS;
    }

    return NRF_ERROR_CRYPTO_INTERNAL;
}


ret_code_t nrf_crypto_backend_optiga_private_key_free(
    void * p_private_key)
{

    return NRF_ERROR_CRYPTO_INTERNAL;
}

ret_code_t nrf_crypto_backend_optiga_public_key_free(
    void * p_public_key)
{

    return NRF_ERROR_CRYPTO_INTERNAL;
}

#if NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA_ECC_SECP256R1)
const nrf_crypto_ecc_curve_info_t g_nrf_crypto_ecc_secp256r1_curve_info =
{
    .public_key_size      = sizeof(nrf_crypto_backend_secp256r1_public_key_t),
    .private_key_size     = sizeof(nrf_crypto_backend_optiga_ecc_private_key_t),
    .curve_type           = NRF_CRYPTO_ECC_SECP256R1_CURVE_TYPE,
    .raw_private_key_size = NRF_CRYPTO_ECC_SECP256R1_RAW_PRIVATE_KEY_SIZE,  // first two bytes are an OPTIGA OID, rest zeroed
    .raw_public_key_size  = NRF_CRYPTO_ECC_SECP256R1_RAW_PUBLIC_KEY_SIZE,  // first two bytes are an OPTIGA OID, rest zeroed
    .p_backend_data       = (void *)OPTIGA_ECC_NIST_P_256,
};
#endif

#endif // NRF_MODULE_ENABLED(NRF_CRYPTO) && NRF_MODULE_ENABLED(NRF_CRYPTO_BACKEND_OPTIGA)
