
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_linux.h"
include "cl_rtsp.h"
@end

// - error codes -
#define ERROR_RTSP_INFO_FILE_OPEN_ERROR 1
#define ERROR_RTSP_INFO_FILE_READ_ERROR 2

// === definition of generated structures ======================================

// -- rtsp_info_s --
@begin
struct
<
bc_array_s:buffer
file_s:file
>
rtsp_info_s;
@end

WUR int rtsp_info_s_create(rtsp_info_s *this,const char *a_file_name);
WUR int rtsp_info_s_print_packet(rtsp_info_s *this);

// === inline methods of generated structures ==================================

// -- rtsp_info_s --
@begin
inlines rtsp_info_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

