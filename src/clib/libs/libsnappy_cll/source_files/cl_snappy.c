
@begin
include "cl_snappy.h"
@end

// === global functions ========================================================

int cl_snappy_compress(const char *a_data,size_t a_length,bc_array_s *a_trg)
{/*{{{*/
  size_t length = snappy_max_compressed_length(a_length);
  bc_array_s_reserve(a_trg,length);

  // - ERROR -
  if (snappy_compress(a_data,a_length,a_trg->data + a_trg->used,&length) != SNAPPY_OK)
  {
    throw_error(SNAPPY_COMPRESS_ERROR);
  }

  a_trg->used += length;

  return 0;
}/*}}}*/

int cl_snappy_uncompress(const char *a_data,size_t a_length,bc_array_s *a_trg)
{/*{{{*/
  size_t length;

  // - ERROR -
  if (snappy_uncompressed_length(a_data,a_length,&length) != SNAPPY_OK)
  {
    throw_error(SNAPPY_UNCOMPRESSED_LENGTH_ERROR);
  }

  bc_array_s_reserve(a_trg,length);

  // - ERROR -
  if (snappy_uncompress(a_data,a_length,a_trg->data + a_trg->used,&length) != SNAPPY_OK)
  {
    throw_error(SNAPPY_UNCOMPRESS_ERROR);
  }

  a_trg->used += length;

  return 0;
}/*}}}*/

