
@begin
include "cl_test.h"
@end

// === global functions ========================================================

void execute_tests(char **a_argv,const char *a_test_name,
    const char **a_test_names,test_function_t *a_test_functions,
    unsigned a_test_cnt)
{/*{{{*/
#define PATH_SIZE 256

  char path[PATH_SIZE];

  // - if there are any tests implemented -
  if (a_test_cnt != 0)
  {
    // - check and create test directories -
    const char **n_ptr = a_test_names;
    const char **n_ptr_end = n_ptr + a_test_cnt;
    do {
      cassert(snprintf(path,PATH_SIZE,"tests/%s/%s",a_test_name,*n_ptr) < PATH_SIZE);

      int exist;
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
      struct stat st;
      exist = stat(path,&st) == 0 && S_ISDIR(st.st_mode);
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
      struct _stat st;
      exist = _stat(path,&st) == 0 && st.st_mode & S_IFDIR;
#else
      cassert(0);
#endif

      if (!exist)
      {
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
        cassert(mkdir(path,0777) == 0);
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
        cassert(_mkdir(path) == 0);
#else
        cassert(0);
#endif
      }
    } while(++n_ptr < n_ptr_end);

    // - process all arguments -
    char **a_ptr = a_argv;
    while (*++a_ptr != NULL)
    {
      // - execute all tests -
      if (strcmp(*a_ptr,"all") == 0)
      {
        test_function_t *f_ptr = a_test_functions;
        test_function_t *f_ptr_end = f_ptr + a_test_cnt;
        do {
          (*f_ptr)();
        } while(++f_ptr < f_ptr_end);
      }

      // - print list of available tests -
      else if (strcmp(*a_ptr,"list") == 0)
      {
        const char **n_ptr = a_test_names;
        const char **n_ptr_end = n_ptr + a_test_cnt;
        do {
          printf("%s\n",*n_ptr);
        } while(++n_ptr < n_ptr_end);
      }

      // - search for test by its name -
      else
      {
        const char **n_ptr = a_test_names;
        const char **n_ptr_end = n_ptr + a_test_cnt;
        do {
          if (strcmp(*a_ptr,*n_ptr) == 0)
          {
            a_test_functions[n_ptr - a_test_names]();
            break;
          }
        } while(++n_ptr < n_ptr_end);

        // - test was not found -
        if (n_ptr >= n_ptr_end)
        {
          fprintf(stderr,"Cannot found test with name: %s\n",*a_ptr);
          cassert(0);
        }
      }
    }
  }
}/*}}}*/

