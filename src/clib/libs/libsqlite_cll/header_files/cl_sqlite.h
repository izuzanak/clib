
#ifndef CL_LIB_SQLITE_H // NOLINT
#define CL_LIB_SQLITE_H

@begin
include "cl_var.h"
@end

#include <sqlite3.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libsqlite_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libsqlite_cll_EXPORTS
#define libsqlite_cll_EXPORT __declspec(dllexport)
#else
#define libsqlite_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SQLITE_CONN_DB_OPEN_ERROR 1
#define ERROR_SQLITE_CONN_EXECUTE_ERROR 2
#define ERROR_SQLITE_CONN_PREPARE_ERROR 3

// === definition of structure sqlite_stmt_s ===================================

typedef struct sqlite_stmt_s
{
  sqlite3_stmt *stmt;
  int done;
}
sqlite_stmt_s;

@begin
define sqlite_stmt_s dynamic
@end

static inline void sqlite_stmt_s_init(sqlite_stmt_s *this);
static inline void sqlite_stmt_s_clear(sqlite_stmt_s *this);
static inline void sqlite_stmt_s_flush_all(sqlite_stmt_s *this);
static inline void sqlite_stmt_s_swap(sqlite_stmt_s *this,sqlite_stmt_s *a_second);
static inline void sqlite_stmt_s_copy(sqlite_stmt_s *this,const sqlite_stmt_s *a_src);
static inline int sqlite_stmt_s_compare(const sqlite_stmt_s *this,const sqlite_stmt_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void sqlite_stmt_s_to_string(const sqlite_stmt_s *this,bc_array_s *a_trg);
#endif

// TODO (devel) continue ...

// === definition of structure sqlite_s ========================================

typedef sqlite3 * sqlite_s;
@begin
define sqlite_s dynamic
@end

static inline void sqlite_s_init(sqlite_s *this);
static inline void sqlite_s_clear(sqlite_s *this);
static inline void sqlite_s_flush_all(sqlite_s *this);
static inline void sqlite_s_swap(sqlite_s *this,sqlite_s *a_second);
static inline void sqlite_s_copy(sqlite_s *this,const sqlite_s *a_src);
static inline int sqlite_s_compare(const sqlite_s *this,const sqlite_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void sqlite_s_to_string(const sqlite_s *this,bc_array_s *a_trg);
#endif

WUR static inline int sqlite_s_open(sqlite_s *this,const char *a_path);
WUR static inline int sqlite_s_open_v2(sqlite_s *this,const char *a_path,int a_flags);
WUR static inline int sqlite_s_threadsave(void);
WUR libsqlite_cll_EXPORT int sqlite_s_execute(sqlite_s *this,
    const char *a_query,int a_query_size,var_s *a_trg);
WUR libsqlite_cll_EXPORT int sqlite_s_prepare(sqlite_s *this,
    const char *a_query,int a_query_size,sqlite_stmt_s *a_trg);

// === inline methods of structure sqlite_s ====================================

static inline void sqlite_stmt_s_init(sqlite_stmt_s *this)
{/*{{{*/
  this->stmt = NULL;
  this->done = 0;
}/*}}}*/

static inline void sqlite_stmt_s_clear(sqlite_stmt_s *this)
{/*{{{*/
  if (this->stmt != NULL)
  {
    sqlite3_finalize(this->stmt);
  }

  sqlite_stmt_s_init(this);
}/*}}}*/

static inline void sqlite_stmt_s_flush_all(sqlite_stmt_s *this)
{/*{{{*/
}/*}}}*/

static inline void sqlite_stmt_s_swap(sqlite_stmt_s *this,sqlite_stmt_s *a_second)
{/*{{{*/
  sqlite_stmt_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void sqlite_stmt_s_copy(sqlite_stmt_s *this,const sqlite_stmt_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int sqlite_stmt_s_compare(const sqlite_stmt_s *this,const sqlite_stmt_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void sqlite_stmt_s_to_string(const sqlite_stmt_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"sqlite_stmt_s{%p}",this);
}/*}}}*/
#endif

// === inline methods of structure sqlite_s ====================================

static inline void sqlite_s_init(sqlite_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void sqlite_s_clear(sqlite_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    sqlite3_close(*this);
  }

  sqlite_s_init(this);
}/*}}}*/

static inline void sqlite_s_flush_all(sqlite_s *this)
{/*{{{*/
}/*}}}*/

static inline void sqlite_s_swap(sqlite_s *this,sqlite_s *a_second)
{/*{{{*/
  sqlite_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void sqlite_s_copy(sqlite_s *this,const sqlite_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int sqlite_s_compare(const sqlite_s *this,const sqlite_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void sqlite_s_to_string(const sqlite_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"sqlite_s{%p}",*this);
}/*}}}*/
#endif

static inline int sqlite_s_open(sqlite_s *this,const char *a_path)
{/*{{{*/
  sqlite_s_clear(this);

  // - ERROR -
  if (sqlite3_open(a_path,this) != SQLITE_OK)
  {
    sqlite_s_clear(this);

    throw_error(SQLITE_CONN_DB_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sqlite_s_open_v2(sqlite_s *this,const char *a_path,int a_flags)
{/*{{{*/
  sqlite_s_clear(this);

  // - ERROR -
  if (sqlite3_open_v2(a_path,this,a_flags,NULL) != SQLITE_OK)
  {
    sqlite_s_clear(this);

    throw_error(SQLITE_CONN_DB_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int sqlite_s_threadsave(void)
{/*{{{*/
  return sqlite3_threadsafe();
}/*}}}*/

#endif

