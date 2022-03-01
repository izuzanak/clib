
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_image.h"
include "cl_quirc.h"
@end

#define ERROR_QUIRC_PAYLOAD_READ_JPEG_DATA_ERROR 1
#define ERROR_QUIRC_PAYLOAD_QUIRC_PROCESS_ERROR 2

// === definition of global functions ==========================================

WUR int process_jpeg_file(const char *a_path,bc_array_s *a_trg);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

