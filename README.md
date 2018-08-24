# Infineon's OPTIGA&trade; Trust X1 hostcode

## Description

<img src="https://github.com/Infineon/Assets/blob/master/Pictures/OPTIGA-Trust-X.png">

This repository contains a C library for [OPTIGA™ Trust X1](www.infineon.com/optiga-trust-x) security chip.

## Summary
The [OPTIGA™ Trust X1](https://www.infineon.com/dgdl/Infineon-DPS310-DS-v01_00-EN.pdf) is a security solution based on a secure micrcontroller. Each device is shipped with a unique elliptic-curve keypair and a corresponding X.509 certificate. OPTIGA™ Trust X enables easy integration into existing PKI infrastructure.

## Key Features and Benefits
* High-end security controller
* Turnkey solution
* One-way authentication using ECDSA
* Mutual authentication using DTLS client (IETF standard RFC 6347)
* Secure communication using DTLS
* Compliant with the USB Type-C™ Authentication standard
* I2C interface
* Up to 10 KB user memory
* Cryptographic support: ECC256, AES128, SHA-256, TRNG, DRNG
* PG-USON-10-2 package (3 x 3 mm)
* Standard & extended temperature ranges
* Full system integration support
* Common Criteria Certified EAL6+ (high) hardware
* Cryptographic Tool Box based on ECC NIST P256, P384 and SHA256 (sign, verify, key generation, ECDH, session key derivation)   

## Hostcode strucuture
### Repository overview
The four folders in this repository are: 
1) **certifcates** - X.509 CA certificates used either to verify an End Device Certificate (stored inside OPTIGA™ Trust X) or used as an Trust Anchor CA stored by default inside a dedicated Trust Anchor certificate slot inside OPTIGA™ Trust X
    * **Infineon OPTIGA(TM) Trust X CA 101** - Certificate Authority certificate used to issue productive security chip samples
    * **OPTIGA_Trust_X_InfineonTestCA** - Certificate Authority certificate used to issue test security ship samples
2) **examples**
    * **optiga** - a collection of examples showing usage of various tool box functions of the security chip
    * **ecdsa_utils** - helping function to encode/decode ecdsa signatures to/from ASN.1 format
    * **mbedtls** - porting functions to offload crypto operation onto the security chip for mbedTLS (compatible with versions >= 2.7)
3) **optiga** - core library, find more in the [Hostcode architecture](#hostcode_architecture) section
4) **pal** - Supported Platform Abstraction Layer implementation. See [Porting guide](#porting_guide)

### <a name="hostcode_architecture"></a>Hostcode architecture

![hostcode architecture](https://raw.githubusercontent.com/Infineon/Assets/master/Pictures/optiga_trust_x_stack_generic.jpg)

Detailed modules description you can find on [Wiki page](https://github.com/Infineon/optiga-trust-x/wiki) of this project

### Hostcode architecture with mapped sources/headers
The figure uses relative to <repo_root>/optiga/ paths

![hostcode architecture mapped](https://raw.githubusercontent.com/Infineon/Assets/master/Pictures/optiga_trust_x_stack_generic_mapped.jpg)

## Get started and Application Notes
Get started guide is based on the OPTIGA™ Trust X evaluation kit hardware and DAVE™ 4 (Digital Application Virtual Engineer) IDE. The detailed description you can find in the respective GitHub [repository](https://github.com/Infineon/getstarted-optiga-trust-x).

Infineon's GitHub offers various application notes for OPTIGA™ Trust X security solutions, you can find get started guide mentioned above there as well. The up-to-date list of application notes is [here](https://github.com/Infineon/appnotes-optiga-trust-x)

## Evaluation and developement kits
* [OPTIGA™ Trust X evaluation and demonstration kit](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-x-eval-kit/)
* OPTIGA™ Trust X Shield2Go (link pending)

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
