
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_channel.h"
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

