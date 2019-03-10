
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_sys.h"
include "cl_xml.h"
@end

// === test execution functions ================================================

void test_xml_parse();
void test_xml_create();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

