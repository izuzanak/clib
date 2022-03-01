
#ifndef OD_TEST_MAIN_H
#define OD_TEST_MAIN_H

@begin
include "cl_test.h"
include "od_odb.h"
@end

// === test execution functions ================================================

void test_set_get_value();
void test_add_remove_node();
void test_find_remove_node();
void test_nodes_path_tree();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

