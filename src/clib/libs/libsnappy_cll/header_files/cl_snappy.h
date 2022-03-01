
#ifndef CL_LIB_SNAPPY_H
#define CL_LIB_SNAPPY_H

@begin
include "cl_struct.h"
@end

#include <snappy-c.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libsnappy_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libsnappy_cll_EXPORTS
#define libsnappy_cll_EXPORT __declspec(dllexport)
#else
#define libsnappy_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SNAPPY_COMPRESS_ERROR 1
#define ERROR_SNAPPY_UNCOMPRESSED_LENGTH_ERROR 2
#define ERROR_SNAPPY_UNCOMPRESS_ERROR 3

// === definition of global functions ==========================================

WUR libsnappy_cll_EXPORT int cl_snappy_compress(const char *a_data,size_t a_length,bc_array_s *a_trg);
WUR libsnappy_cll_EXPORT int cl_snappy_uncompress(const char *a_data,size_t a_length,bc_array_s *a_trg);

// === inline global functions =================================================

#endif

