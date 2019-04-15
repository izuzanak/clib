
#ifndef __CL_VARVAL_H
#define __CL_VARVAL_H

@begin
include "cl_var.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libvarval_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libvarval_cll_EXPORTS
#define libvarval_cll_EXPORT __declspec(dllexport)
#else
#define libvarval_cll_EXPORT __declspec(dllimport)
#endif
#endif

// === definition of generated structures ======================================

// === inline methods of generated structures ==================================

#endif

