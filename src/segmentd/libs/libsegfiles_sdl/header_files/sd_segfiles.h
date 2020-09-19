
#ifndef __SD_SEGFILES_H
#define __SD_SEGFILES_H

@begin
include "sd_record.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libsegfiles_sdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libsegfiles_sdl_EXPORTS
#define libsegfiles_sdl_EXPORT __declspec(dllexport)
#else
#define libsegfiles_sdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SD_SEGFILES_FILE_OPEN_ERROR 1
#define ERROR_SD_SEGFILES_FILE_STAT_ERROR 2
#define ERROR_SD_SEGFILES_FILE_RENAME_ERROR 3
#define ERROR_SD_SEGFILES_RECORD_WRITE_ERROR 4
#define ERROR_SD_SEGFILES_RECORD_READ_ERROR 5
#define ERROR_SD_SEGFILES_RECORD_NOT_EXIST 6

// === constants and definitions ===============================================

// === definition of generated structures ======================================

// -- sd_segment_files_s --
@begin
struct
<
string_s:path
string_s:path_tmp

ui:data_size
ui:rec_size

lli:last_id
bc_array_s:rec_data
>
sd_segment_files_s;
@end

WUR libsegfiles_sdl_EXPORT int sd_segment_files_s_create(sd_segment_files_s *this,
    const char *a_path,unsigned a_data_size);
WUR libsegfiles_sdl_EXPORT int sd_segment_files_s_write_record(sd_segment_files_s *this,
    time_s a_time,unsigned a_size,const char *a_data);
WUR libsegfiles_sdl_EXPORT int sd_segment_files_s_get_record(sd_segment_files_s *this,
    time_s *a_time,bc_array_s *a_record);

// === inline methods of generated structures ==================================

// -- sd_segment_files_s --
@begin
inlines sd_segment_files_s
@end

#endif

