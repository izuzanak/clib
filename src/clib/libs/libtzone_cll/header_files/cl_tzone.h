
#ifndef CL_LIB_TZONE_H
#define CL_LIB_TZONE_H

@begin
include "cl_tzone_util.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libtzone_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libtzone_cll_EXPORTS
#define libtzone_cll_EXPORT __declspec(dllexport)
#else
#define libtzone_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_TIME_ZONE_PARSE_ERROR 1

// === definition of generated structures ======================================

// -- time_zone_s --
@begin
struct
<
bi:rule_mode
bi:stdoffset
bi:dstoffset
tz_rule_s:start
tz_rule_s:end
bi:year
bi:starttime
bi:endtime
bi:utc_offset
>
time_zone_s;
@end

WUR libtzone_cll_EXPORT int time_zone_s_create(time_zone_s *this,const char *a_tz);
libtzone_cll_EXPORT void time_zone_s_to_local(time_zone_s *this,time_s a_src,time_s *a_trg);

// === inline methods of generated structures ==================================

// -- time_zone_s --
@begin
inlines time_zone_s
@end

#endif

