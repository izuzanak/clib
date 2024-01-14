
@begin
include "cl_zlib.h"
@end

// === methods of structure gz_file_s ==========================================

int gz_file_s_read(gz_file_s *this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != NULL);

  const int c_buffer_add = 1024;

  int read_cnt;
  do
  {
    unsigned old_used = a_trg->used;
    bc_array_s_push_blanks(a_trg,c_buffer_add);
    read_cnt = gzread(*this,a_trg->data + old_used,c_buffer_add);
  }
  while(read_cnt >= c_buffer_add);

  if (read_cnt < 0 || (read_cnt == 0 && !gzeof(*this)))
  {
    throw_error(GZ_FILE_READ_ERROR);
  }

  a_trg->used = (a_trg->used - c_buffer_add) + read_cnt;

  return 0;
}/*}}}*/

// === global functions ========================================================

int zlib_compress(const void *a_src,uLong a_size,int a_level,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  if (a_level < 0 || a_level > 9)
  {
    throw_error(ZLIB_COMPRESS_INVALID_LEVEL);
  }

  // - retrieve maximal target length -
  uLongf target_length = compressBound(a_size);

  // - reserve target space -
  bc_array_s_reserve(a_trg,target_length);
  
  // - ERROR -
  if (compress2((Bytef *)(a_trg->data + a_trg->used),&target_length,a_src,a_size,a_level) != Z_OK)
  {
    throw_error(ZLIB_COMPRESS_ERROR);
  }

  a_trg->used += target_length;

  return 0;
}/*}}}*/

int zlib_uncompress(const void *a_src,uLong a_size,uLongf a_target_size,bc_array_s *a_trg)
{/*{{{*/

  // - ERROR -
  if (a_target_size <= 0)
  {
    throw_error(ZLIB_NOT_ENOUGHT_SPACE_IN_BUFFER);
  }

  // - retrieve target length -
  uLongf target_length = a_target_size;

  // - reserve target space -
  bc_array_s_reserve(a_trg,target_length);

  // - uncompress data -
  int res = uncompress((Bytef *)(a_trg->data + a_trg->used),&target_length,a_src,a_size);

  // - ERROR -
  if (res != Z_OK)
  {
    if (res == Z_BUF_ERROR)
    {
      throw_error(ZLIB_NOT_ENOUGHT_SPACE_IN_BUFFER);
    }
    else
    {
      throw_error(ZLIB_UNCOMPRESS_ERROR);
    }
  }

  a_trg->used += target_length;

  return 0;
}/*}}}*/

