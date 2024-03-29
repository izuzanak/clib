
#ifndef CL_TEST_MAIN_H
#define CL_TEST_MAIN_H

@begin
include "cl_test.h"
include "cl_regex.h"
include "cl_sys.h"
include "cl_linux.h"
@end

// === definition of generated structures ======================================

// -- fd_array_s --
@begin
array<fd_s> fd_array_s;
@end

// === inline methods of generated structures ==================================

// -- fd_array_s --
@begin
inlines fd_array_s
@end

// === test execution functions ================================================

void test_fd();
void test_dir();
void test_mmap();
void test_socket_address();
void test_socket();
void test_socket_udp();
void test_socket_tcp();
void test_aio();
void test_pid();
void test_pthread();
void test_epoll();
void test_rtc();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

