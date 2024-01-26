
#ifndef CL_LIB_ZLIB_H
#define CL_LIB_ZLIB_H

@begin
include "cl_struct.h"
@end

#include <zlib.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libzlib_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libzlib_cll_EXPORTS
#define libzlib_cll_EXPORT __declspec(dllexport)
#else
#define libzlib_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_GZ_FILE_OPEN_ERROR 1
#define ERROR_GZ_FILE_SEEK_ERROR 2
#define ERROR_GZ_FILE_TELL_ERROR 3
#define ERROR_GZ_FILE_WRITE_ERROR 4
#define ERROR_GZ_FILE_READ_ERROR 5

#define ERROR_ZLIB_COMPRESS_INVALID_LEVEL 1
#define ERROR_ZLIB_COMPRESS_ERROR 2
#define ERROR_ZLIB_NOT_ENOUGHT_SPACE_IN_BUFFER 3
#define ERROR_ZLIB_UNCOMPRESS_ERROR 4

// === definition of structure zlib_crc32_s ====================================

typedef ui zlib_crc32_s;
@begin
define zlib_crc32_s basic
@end

#if OPTION_TO_STRING == ENABLED
static inline void zlib_crc32_s_to_string(const zlib_crc32_s *this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void zlib_crc32_s_to_json(const zlib_crc32_s *this,bc_array_s *a_trg);
#endif
static inline void zlib_crc32_s_update(zlib_crc32_s *this,
    unsigned a_size,const void *a_data);
static inline int zlib_crc32_s_valid(const zlib_crc32_s *this,
    zlib_crc32_s a_init,unsigned a_size,const void *a_data);

// === definition of structure gz_file_s =======================================

typedef gzFile gz_file_s;
@begin
define gz_file_s dynamic
@end

static inline void gz_file_s_init(gz_file_s *this);
static inline void gz_file_s_clear(gz_file_s *this);
static inline void gz_file_s_flush_all(gz_file_s *this);
static inline void gz_file_s_swap(gz_file_s *this,gz_file_s *a_second);
static inline void gz_file_s_copy(gz_file_s *this,const gz_file_s *a_src);
static inline int gz_file_s_compare(const gz_file_s *this,const gz_file_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void gz_file_s_to_string(const gz_file_s *this,bc_array_s *a_trg);
#endif

WUR static inline int gz_file_s_open(gz_file_s *this,const char *a_pathname,const char *a_mode);
WUR static inline int gz_file_s_fd_open(gz_file_s *this,int a_fd,const char *a_mode);
WUR static inline int gz_file_s_seek(gz_file_s *this,z_off_t a_offset,int a_whence,z_off_t *a_result);
WUR static inline int gz_file_s_tell(gz_file_s *this,z_off_t *a_offset);
WUR static inline int gz_file_s_write(gz_file_s *this,const void *a_src,unsigned a_size);
WUR static inline int gz_file_s_write_close(gz_file_s *this,const void *a_src,unsigned a_size);
WUR libzlib_cll_EXPORT int gz_file_s_read(gz_file_s *this,bc_array_s *a_trg);
WUR static inline int gz_file_s_read_close(gz_file_s *this,bc_array_s *a_trg);

// === definition of global functions ==========================================

WUR libzlib_cll_EXPORT int zlib_compress(
    const void *a_src,uLong a_size,int a_level,bc_array_s *a_trg);
WUR libzlib_cll_EXPORT int zlib_uncompress(
    const void *a_src,uLong a_size,uLongf a_target_size,bc_array_s *a_trg);

// === inline methods of structure zlib_crc32_s ================================

#if OPTION_TO_STRING == ENABLED
static inline void zlib_crc32_s_to_string(const zlib_crc32_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"zlib_crc32_s{0x%8.8x}",*this);
}/*}}}*/
#endif

#if OPTION_TO_JSON == ENABLED
static inline void zlib_crc32_s_to_json(const zlib_crc32_s *this,bc_array_s *a_trg)
{/*{{{*/
  ui_to_json(this,a_trg);
}/*}}}*/
#endif

static inline void zlib_crc32_s_update(zlib_crc32_s *this,
    unsigned a_size,const void *a_data)
{//{{{
  *this = crc32_z(*this,a_data,a_size);
}//}}}

static inline int zlib_crc32_s_valid(const zlib_crc32_s *this,
    zlib_crc32_s a_init,unsigned a_size,const void *a_data)
{/*{{{*/
  zlib_crc32_s crc = a_init;
  zlib_crc32_s_update(&crc,a_size,a_data);

  return *this == crc;
}/*}}}*/

// === inline methods of structure gz_file_s ===================================

static inline void gz_file_s_init(gz_file_s *this)
{/*{{{*/
  *this = NULL;
}/*}}}*/

static inline void gz_file_s_clear(gz_file_s *this)
{/*{{{*/
  if (*this != NULL)
  {
    gzclose(*this);
  }

  gz_file_s_init(this);
}/*}}}*/

static inline void gz_file_s_flush_all(gz_file_s *this)
{/*{{{*/
}/*}}}*/

static inline void gz_file_s_swap(gz_file_s *this,gz_file_s *a_second)
{/*{{{*/
  gz_file_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void gz_file_s_copy(gz_file_s *this,const gz_file_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int gz_file_s_compare(const gz_file_s *this,const gz_file_s *a_second)
{/*{{{*/
  return *this == *a_second;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void gz_file_s_to_string(const gz_file_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"gz_file_s{%p}",*this);
}/*}}}*/
#endif

static inline int gz_file_s_open(gz_file_s *this,const char *a_pathname,const char *a_mode)
{/*{{{*/
  gz_file_s_clear(this);

  *this = gzopen(a_pathname,a_mode);

  // - ERROR -
  if (*this == NULL)
  {
    throw_error(GZ_FILE_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int gz_file_s_fd_open(gz_file_s *this,int a_fd,const char *a_mode)
{/*{{{*/
  gz_file_s_clear(this);

  *this = gzdopen(a_fd,a_mode);

  // - ERROR -
  if (*this == NULL)
  {
    throw_error(GZ_FILE_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

static inline int gz_file_s_seek(gz_file_s *this,z_off_t a_offset,int a_whence,z_off_t *a_result)
{/*{{{*/
  debug_assert(*this != NULL);

  z_off_t result = gzseek(*this,a_offset,a_whence);

  // - ERROR -
  if (result < 0)
  {
    throw_error(GZ_FILE_SEEK_ERROR);
  }

  if (a_result != NULL)
  {
    *a_result = result;
  }

  return 0;
}/*}}}*/

static inline int gz_file_s_tell(gz_file_s *this,z_off_t *a_offset)
{/*{{{*/
  debug_assert(*this != NULL);

  z_off_t result = gztell(*this);

  // - ERROR -
  if (result < 0)
  {
    throw_error(GZ_FILE_TELL_ERROR);
  }

  *a_offset = result;

  return 0;
}/*}}}*/

static inline int gz_file_s_write(gz_file_s *this,const void *a_src,unsigned a_size)
{/*{{{*/
  debug_assert(*this != NULL);

  // - ERROR -
  if (gzwrite(*this,a_src,a_size) != (int)a_size)
  {
    throw_error(GZ_FILE_WRITE_ERROR);
  }

  return 0;
}/*}}}*/

static inline int gz_file_s_write_close(gz_file_s *this,const void *a_src,unsigned a_size)
{/*{{{*/
  int res = gz_file_s_write(this,a_src,a_size);
  if (res != 0)
  {
    return res;
  }

  gz_file_s_clear(this);

  return 0;
}/*}}}*/

static inline int gz_file_s_read_close(gz_file_s *this,bc_array_s *a_trg)
{/*{{{*/
  int res = gz_file_s_read(this,a_trg);
  if (res != 0)
  {
    return res;
  }

  gz_file_s_clear(this);

  return 0;
}/*}}}*/

#endif

