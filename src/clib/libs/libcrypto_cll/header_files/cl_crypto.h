
#ifndef __CL_CRYPTO_H
#define __CL_CRYPTO_H

@begin
include "cl_struct.h"
@end

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libcrypto_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libcrypto_cll_EXPORTS
#define libcrypto_cll_EXPORT __declspec(dllexport)
#else
#define libcrypto_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_CRYPTO_DUMMY_ERROR 1

// === definition of global functions ==========================================

void libcrypto_cll_init();
void libcrypto_cll_clear();

#endif

