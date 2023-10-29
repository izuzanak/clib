
#ifndef CL_LIB_TZONE_UTIL_H
#define CL_LIB_TZONE_UTIL_H

@begin
include "cl_time.h"
@end

enum {
  TZ_RULE_JULIAN_DAY = 0,       /* Jn = Julian day */
  TZ_RULE_DAY_OF_YEAR,          /* n = day of year */
  TZ_RULE_MONTH_NTH_DAY_OF_WEEK /* Mm.n.d = month, week, day of week */
};

// === definition of generated structures ======================================

// -- tz_rule_s --
@begin
struct
<
bi:r_type
bi:r_day
bi:r_week
bi:r_mon
bi:r_time
>
tz_rule_s;
@end

// === definition of global functions ==========================================

const char *tzu_getzname(const char *strp);
const char *tzu_getqzname(const char *strp,char delim);
const char *tzu_getnum(const char *strp,int *nump,int min,int max);
const char *tzu_getsecs(const char *strp,int *secsp);
const char *tzu_getoffset(const char *strp,int *offsetp);
const char *tzu_getrule(const char *strp,tz_rule_s *rulep);
int tzu_transtime(int year,const tz_rule_s *rulep,int offset);

// === inline methods of generated structures ==================================

// -- tz_rule_s --
@begin
inlines tz_rule_s
@end

#endif

