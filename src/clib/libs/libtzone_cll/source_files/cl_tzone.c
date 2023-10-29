
@begin
include "cl_tzone.h"
@end

// === constants and definitions ===============================================

#ifndef TZDEFRULESTRING
#define TZDEFRULESTRING ",M3.2.0,M11.1.0"
#endif

#define SECSPERMIN   60
#define MINSPERHOUR  60
#define HOURSPERDAY  24
#define DAYSPERWEEK  7
#define DAYSPERNYEAR 365
#define DAYSPERLYEAR 366
#define SECSPERHOUR  (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY   ((int) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR  12

// === methods of generated structures =========================================

// -- time_zone_s --
@begin
methods time_zone_s
@end

int time_zone_s_create(time_zone_s *this,const char *a_tz)
{/*{{{*/
  const char *stdname;
  const char *dstname;
  size_t stdlen;
  size_t dstlen;

  stdname = a_tz;
  if (*a_tz == '<') {
    a_tz++;
    stdname = a_tz;
    a_tz = tzu_getqzname(a_tz,'>');
    if (*a_tz != '>')
      throw_error(TIME_ZONE_PARSE_ERROR);
    stdlen = a_tz - stdname;
    a_tz++;
  } else {
    a_tz = tzu_getzname(a_tz);
    stdlen = a_tz - stdname;
  }

  if (!stdlen)
    throw_error(TIME_ZONE_PARSE_ERROR);

  a_tz = tzu_getoffset(a_tz,&this->stdoffset);
  if (a_tz == NULL)
    throw_error(TIME_ZONE_PARSE_ERROR);

  if (*a_tz != '\0') {
    if (*a_tz == '<') {
      dstname = ++a_tz;
      a_tz = tzu_getqzname(a_tz,'>');
      if (*a_tz != '>')
        throw_error(TIME_ZONE_PARSE_ERROR);
      dstlen = a_tz - dstname;
      a_tz++;
    } else {
      dstname = a_tz;
      a_tz = tzu_getzname(a_tz);
      dstlen = a_tz - dstname; /* length of DST abbr. */
    }
    if (!dstlen)
      throw_error(TIME_ZONE_PARSE_ERROR);


    if (*a_tz != '\0' && *a_tz != ',' && *a_tz != ';') {
      a_tz = tzu_getoffset(a_tz,&this->dstoffset);
      if (a_tz == NULL)
        throw_error(TIME_ZONE_PARSE_ERROR);
    } else {
      this->dstoffset = this->stdoffset - SECSPERHOUR;
    }

    if (*a_tz == '\0') {
      a_tz = TZDEFRULESTRING;
    }

    if (*a_tz == ',' || *a_tz == ';') {
      a_tz++;
      if ((a_tz = tzu_getrule(a_tz,&this->start)) == NULL)
        throw_error(TIME_ZONE_PARSE_ERROR);
      if (*a_tz++ != ',')
        throw_error(TIME_ZONE_PARSE_ERROR);
      if ((a_tz = tzu_getrule(a_tz,&this->end)) == NULL)
        throw_error(TIME_ZONE_PARSE_ERROR);
      if (*a_tz != '\0')
        throw_error(TIME_ZONE_PARSE_ERROR);

      this->year = -1;
      this->rule_mode = 1;
    }
    else
    {
      throw_error(TIME_ZONE_PARSE_ERROR);
    }
  } else
  {
    this->rule_mode = 0;
  }

  return 0;
}/*}}}*/

void time_zone_s_to_local(time_zone_s *this,time_s a_src,time_s *a_trg)
{/*{{{*/
  time_t utc_time = a_src / 1000000000ULL;
  unsigned subsec = a_src % 1000000000ULL;

  if (this->rule_mode)
  {
    struct tm *time_spl = gmtime(&utc_time);

    int year = time_spl->tm_year + 1900;
    if (year != this->year)
    {
      this->starttime = tzu_transtime(year,&this->start,this->stdoffset);
      this->endtime = tzu_transtime(year,&this->end,this->dstoffset);
      this->year = year;
    }

    // Actual sec position, from 1.1.this_year
    int actualtime = time_spl->tm_yday * SECSPERDAY;
    actualtime += time_spl->tm_hour * SECSPERHOUR;
    actualtime += time_spl->tm_min * SECSPERMIN;
    actualtime += time_spl->tm_sec;

    // check actual time in interval
    if ((actualtime >= this->starttime) && (actualtime <= this->endtime))
    {
      this->utc_offset = -this->dstoffset;
    }
    else
    {
      this->utc_offset = -this->stdoffset;
    }
  }
  else
  {
    this->utc_offset = -this->stdoffset;
  }

  *a_trg = (utc_time + this->utc_offset)*1000000000ULL + subsec;
}/*}}}*/

