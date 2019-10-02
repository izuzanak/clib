
@begin
include "sd_record.h"
@end

// === constants and definitions ===============================================

unsigned g_sd_page_size;

// === methods of generated structures =========================================

// -- sd_record_timestamp_s --
@begin
methods sd_record_timestamp_s
@end

// -- sd_record_header_s --
@begin
methods sd_record_header_s
@end

// === global functions ========================================================

void librecord_sdl_init()
{/*{{{*/

  // - retrieve system page size -
  g_sd_page_size = sysconf(_SC_PAGE_SIZE);

}/*}}}*/

void librecord_sdl_clear()
{/*{{{*/
}/*}}}*/

