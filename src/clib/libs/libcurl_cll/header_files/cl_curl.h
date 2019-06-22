
#ifndef __CL_CURL_H
#define __CL_CURL_H

@begin
include "cl_sys.h"
@end

#include <curl/curl.h>

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#include <poll.h>
#endif

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libcurl_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libcurl_cll_EXPORTS
#define libcurl_cll_EXPORT __declspec(dllexport)
#else
#define libcurl_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_CURL_DUMMY_ERROR 1

// === definition of generated structures ======================================

// === inline methods of generated structures ==================================

#endif

