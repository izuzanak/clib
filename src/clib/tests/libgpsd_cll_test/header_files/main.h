
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_gpsd.h"
@end

// === test execution functions ================================================

void test_gpsd_conn();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

