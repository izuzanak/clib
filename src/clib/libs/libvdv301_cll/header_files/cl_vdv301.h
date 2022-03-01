
#ifndef CL_LIB_VDV301_H // NOLINT
#define CL_LIB_VDV301_H

@begin
include "cl_vdv301_gen.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libvdv301_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libvdv301_cll_EXPORTS
#define libvdv301_cll_EXPORT __declspec(dllexport)
#else
#define libvdv301_cll_EXPORT __declspec(dllimport)
#endif
#endif

// === definition of generated structures ======================================

// === inline methods of generated structures ==================================

#endif

