
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_rtsp.h"
include "cl_crypto.h"
@end

// - error codes -
#define ERROR_RTSP_CLIENT_DIGEST_ERROR 1

// === definition of generated structures ======================================

// -- rtsp_client_list_s --
@begin
list<rtsp_client_s> rtsp_client_list_s;
@end

// === inline methods of generated structures ==================================

// -- rtsp_client_list_s --
@begin
inlines rtsp_client_list_s
@end

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

