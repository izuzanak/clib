
#ifndef CL_LIB_BASIC_H
#define CL_LIB_BASIC_H

// - basic data types -
typedef char bc;
typedef unsigned char uc;
typedef short int si;
typedef unsigned short int usi;
typedef int bi;
typedef unsigned int ui;
typedef long int li;
typedef unsigned long uli;
typedef long long int lli;
typedef unsigned long long int ulli;
typedef float bf;
typedef double bd;
typedef long double ld;

// - basic containers macros -
#define CONT_INIT(TYPE,NAME) \
  TYPE NAME;\
  TYPE ## _init(&(NAME));

#define CONT_CLEAR(TYPE,NAME) \
  __attribute__((cleanup(TYPE ## _clear))) TYPE NAME;

#define CONT_INIT_CLEAR(TYPE,NAME) \
  __attribute__((cleanup(TYPE ## _clear))) TYPE NAME;\
  TYPE ## _init(&(NAME));

// - disable clang unused function warning -
#if __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-function"
#endif

// - define gnu source -
#define _GNU_SOURCE // NOLINT(cert-dcl37-c,cert-dcl51-cpp)

#define ENABLED 1
#define MP_COMMA ,

// - system type selection -
#define SYSTEM_TYPE_UNIX    1 // - for system calls use unix environment
#define SYSTEM_TYPE_WINDOWS 2 // - for system calls use windows environment
#define SYSTEM_TYPE_DSP     3 // - target platform is DSP processor
// --

// - subsystem type selection -
#define SUBSYSTEM_TYPE_LINUX   1
#define SUBSYSTEM_TYPE_WINDOWS 2
#define SUBSYSTEM_TYPE_MSYS2   3
// --

// - atomic subsystem selection -
#define ATOMIC_TYPE_GCC     1 // - for atomic structure implementation use GCC compiler functions
#define ATOMIC_TYPE_WINDOWS 2 // - for atomic structure implementation use win32 library
// --

// - basic system configuration -
#ifdef LINUX
#define SYSTEM_TYPE SYSTEM_TYPE_UNIX
#define SUBSYSTEM_TYPE SUBSYSTEM_TYPE_LINUX
#define ATOMIC_TYPE ATOMIC_TYPE_GCC
#define WUR __attribute__((warn_unused_result))
#endif

#ifdef WINDOWS
#define WUR _Check_return_
#define SYSTEM_TYPE SYSTEM_TYPE_WINDOWS
#define SUBSYSTEM_TYPE SUBSYSTEM_TYPE_WINDOWS
#define ATOMIC_TYPE ATOMIC_TYPE_WINDOWS
#endif

#ifdef MSYS2
#define SYSTEM_TYPE SYSTEM_TYPE_UNIX
#define SUBSYSTEM_TYPE SUBSYSTEM_TYPE_MSYS2
#define ATOMIC_TYPE ATOMIC_TYPE_GCC
#define WUR __attribute__((warn_unused_result))
#define lseek64 lseek
typedef long long DWORD;
#endif

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define EXPORT
#define libbase_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#define EXPORT __declspec(dllexport)
#ifdef libbase_cll_EXPORTS
#define libbase_cll_EXPORT __declspec(dllexport)
#else
#define libbase_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - system long long format -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define HOST_LL_FORMAT "ll"
#define HOST_LL_FORMAT_LENGTH 2
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#define HOST_LL_FORMAT "I64"
#define HOST_LL_FORMAT_LENGTH 3
#endif

// - system includes -
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#include <sys/time.h>
#include <unistd.h>
#endif

#if SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#include <winsock2.h>
#include <windows.h>
#include <signal.h>
#include <direct.h>
#endif

// - basic configuration -
#ifndef NDEBUG
#define OPTION_DEBUG_ASSERT ENABLED
#define OPTION_BRUTAL_ASSERT ENABLED
#define OPTION_MEMCHECK ENABLED
#define OPTION_DEBUG_LEVEL 6
#define OPTION_TO_STRING ENABLED
#else
#define OPTION_DEBUG_LEVEL 0
#endif
#define OPTION_TO_JSON ENABLED
#define OPTION_FROM_JSON ENABLED
#define OPTION_FROM_VAR ENABLED
//#define OPTION_PERFORMANCE_TEST ENABLED

// - error macros -
#define throw_error(ERROR_ID)\
{/*{{{*/\
  debug_message_1(fprintf(stderr,"ERROR: " #ERROR_ID ", %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__));\
  return ERROR_ ## ERROR_ID;\
}/*}}}*/

#define throw_verbose_error(ERROR_ID)\
{/*{{{*/\
  fprintf(stderr,"ERROR: " #ERROR_ID ", %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
  return ERROR_ ## ERROR_ID;\
}/*}}}*/

#define throw_warning(ERROR_ID)\
{/*{{{*/\
  debug_message_1(fprintf(stderr,"ERROR: " #ERROR_ID ", %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__));\
  return 0;\
}/*}}}*/

// - asserts macros -
#define cassert(A_COND)\
{/*{{{*/\
  if (!(A_COND)) {\
    fprintf(stderr,"CASSERT: %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
    fflush(stderr);\
    abort();\
  }\
}/*}}}*/

#if OPTION_DEBUG_ASSERT == ENABLED
#define debug_assert(A_COND)\
{/*{{{*/\
  if (!(A_COND)) {\
    fprintf(stderr,"DEBUG_ASSERT: %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
    fflush(stderr);\
    abort();\
  }\
}/*}}}*/
#else
#define debug_assert(A_COND)
#endif

#if OPTION_BRUTAL_ASSERT == ENABLED
#define brutal_assert(A_COND)\
{/*{{{*/\
  if (!(A_COND)) {\
    fprintf(stderr,"BRUTAL_ASSERT: %s +%d, function: %s\n",__FILE__,__LINE__,__FUNCTION__);\
    fflush(stderr);\
    abort();\
  }\
}/*}}}*/
#else
#define brutal_assert(A_COND)
#endif

// - basic constants -
#define c_array_add 4
#define c_idx_not_exist UINT_MAX

// - pi number definitions -
#define c_pi_number   3.14159265358979323844
#define c_2pi_number  6.28318530717958647688
#define c_pid2_number 1.57079632679489661922

// - logarithm of two (needed by red-black tree container) -
#define c_log_of_2 0.69314718055994530941

// - red-black tree descent stack size -
#ifdef _MSC_VER
#define RB_TREE_STACK_SIZE(TYPE,VALUE) 32
#else
#define RB_TREE_STACK_SIZE(TYPE,VALUE) TYPE ## _get_descent_stack_size(VALUE)
#endif

// - to_string debug macros -
#if OPTION_TO_STRING == ENABLED
#define DEBUG_PRINT(TYPE,VALUE) \
{/*{{{*/\
  CONT_INIT(bc_array_s,buffer);\
  TYPE ## _to_string(VALUE,&buffer);\
  bc_array_s_push(&buffer,'\0');\
  fputs(buffer.data,stderr);\
  fputc('\n',stderr);\
  bc_array_s_clear(&buffer);\
}/*}}}*/
#endif

#if OPTION_TO_STRING == ENABLED
#define DEBUG_PRINT_LINES(TYPE,VALUE) \
{/*{{{*/\
  CONT_INIT(bc_array_s,buffer);\
  TYPE ## _to_string_separator(VALUE,&buffer,1,"\n");\
  bc_array_s_push(&buffer,'\0');\
  fputs(buffer.data,stderr);\
  fputc('\n',stderr);\
  bc_array_s_clear(&buffer);\
}/*}}}*/
#endif

// - debug message macros -
/*{{{*/
#define DEBUG_MESSAGE(LEVEL,MSG) fprintf(stderr,"DLvl. " #LEVEL ": "); MSG

#define debug_message_0(MSG) DEBUG_MESSAGE(0,MSG)

#if OPTION_DEBUG_LEVEL >= 1
#define debug_message_1(MSG) DEBUG_MESSAGE(1,MSG)
#else
#define debug_message_1(MSG)
#endif

#if OPTION_DEBUG_LEVEL >= 2
#define debug_message_2(MSG) DEBUG_MESSAGE(2,MSG)
#else
#define debug_message_2(MSG)
#endif

#if OPTION_DEBUG_LEVEL >= 3
#define debug_message_3(MSG) DEBUG_MESSAGE(3,MSG)
#else
#define debug_message_3(MSG)
#endif

#if OPTION_DEBUG_LEVEL >= 4
#define debug_message_4(MSG) DEBUG_MESSAGE(4,MSG)
#else
#define debug_message_4(MSG)
#endif

#if OPTION_DEBUG_LEVEL >= 5
#define debug_message_5(MSG) DEBUG_MESSAGE(5,MSG)
#else
#define debug_message_5(MSG)
#endif

#if OPTION_DEBUG_LEVEL >= 6
#define debug_message_6(MSG) DEBUG_MESSAGE(6,MSG)
#else
#define debug_message_6(MSG)
#endif

#if OPTION_DEBUG_LEVEL >= 7
#define debug_message_7(MSG) DEBUG_MESSAGE(7,MSG)
#else
#define debug_message_7(MSG)
#endif
/*}}}*/

// - pointer typedef -
typedef void * pointer;
typedef char * bc_pointer;
typedef const char * cbc_pointer;

@begin
define pointer basic
define bc_pointer basic
define cbc_pointer basic
@end

#endif

