
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_linux.h"
include "cl_fuse.h"
@end

// === definition of generated structures ======================================

// === inline methods of generated structures ==================================

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

