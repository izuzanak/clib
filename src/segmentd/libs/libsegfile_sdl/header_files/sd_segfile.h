
#ifndef __SD_SEGFILE_H
#define __SD_SEGFILE_H

@begin
include "cl_logger.h"
include "cl_linux.h"
include "sd_record.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libsegfile_sdl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libsegfile_sdl_EXPORTS
#define libsegfile_sdl_EXPORT __declspec(dllexport)
#else
#define libsegfile_sdl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SD_SEGFILE_FILE_OPEN_ERROR 1
#define ERROR_SD_SEGFILE_FILE_STAT_ERROR 2
#define ERROR_SD_SEGFILE_FILE_TRUNCATE_ERROR 3
#define ERROR_SD_SEGFILE_FILE_SYNC_ERROR 4
#define ERROR_SD_SEGFILE_RECORD_READ_ERROR 5
#define ERROR_SD_SEGFILE_RECORD_SEEK_ERROR 6
#define ERROR_SD_SEGFILE_RECORD_WRITE_ERROR 7
#define ERROR_SD_SEGFILE_RECORD_NOT_EXIST 8

// === constants and definitions ===============================================

// === definition of generated structures ======================================

// -- sd_segment_file_s --
@begin
struct
<
fd_s:fd
ui:data_size
ui:rec_size

lli:last_id
ui:last_index

bc_array_s:rec_data
>
sd_segment_file_s;
@end

WUR libsegfile_sdl_EXPORT int sd_segment_file_s_create(sd_segment_file_s *this,
    const char *a_path,unsigned a_data_size);
WUR libsegfile_sdl_EXPORT int sd_segment_file_s_write_record(sd_segment_file_s *this,
    time_s a_time,unsigned a_size,const char *a_data);
WUR libsegfile_sdl_EXPORT int sd_segment_file_s_read_record(sd_segment_file_s *this,
    time_s *a_time,bc_array_s *a_trg);

// === inline methods of generated structures ==================================

// -- sd_segment_file_s --
@begin
inlines sd_segment_file_s
@end

#endif

