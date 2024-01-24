
#ifndef ID_TEST_MAIN_H
#define ID_TEST_MAIN_H

@begin
include "cl_test.h"
include "id_idb.h"
@end

// === test execution functions ================================================

void test_update_extractor();
void test_update_index();
void test_update_index_text();
void test_update_indexes_text();
void test_remove_docs();
void test_reopen_database();

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

