
#ifndef CL_LIB_TEST_H
#define CL_LIB_TEST_H

@begin
include "cl_struct.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libtest_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libtest_cll_EXPORTS
#define libtest_cll_EXPORT __declspec(dllexport)
#else
#define libtest_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - test function type -
typedef void (*test_function_t)();

// === definition of global functions ==========================================

libtest_cll_EXPORT void execute_tests(char **a_argv,const char *a_test_name,
    const char **a_test_names,test_function_t *a_test_functions,
    unsigned a_test_cnt);

#define EXECUTE_TESTS(INIT_CODE,CLEAR_CODE) \
{/*{{{*/\
  memcheck_init();\
\
  INIT_CODE;\
\
  {\
    unsigned test_name_cnt = sizeof(test_names)/sizeof(test_names[0]);\
    unsigned test_function_cnt = sizeof(test_functions)/sizeof(test_functions[0]);\
    cassert(test_name_cnt == test_function_cnt);\
\
    execute_tests(argv,test_name,test_names,test_functions,test_name_cnt);\
  }\
\
  CLEAR_CODE;\
\
  memcheck_release_assert();\
}/*}}}*/

#endif

