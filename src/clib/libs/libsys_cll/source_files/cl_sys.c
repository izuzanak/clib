
@begin
include "cl_sys.h"
@end

// === methods of structure stream_s ===========================================

int stream_s_read(stream_s *this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != NULL);

  const unsigned c_buffer_add = 1024;

  unsigned read_cnt;
  do
  {
    unsigned old_used = a_trg->used;
    bc_array_s_push_blanks(a_trg,c_buffer_add);
    read_cnt = fread(a_trg->data + old_used,1,c_buffer_add,*this);
  }
  while(read_cnt >= c_buffer_add);

  // - ERROR -
  if (ferror(*this))
  {
    throw_error(STREAM_READ_ERROR);
  }

  a_trg->used = (a_trg->used - c_buffer_add) + read_cnt;

  return 0;
}/*}}}*/

int stream_s_read_max(stream_s *this,bc_array_s *a_trg,size_t a_size)
{/*{{{*/
  debug_assert(*this != NULL);

  // - prepare target buffer -
  bc_array_s_reserve(a_trg,a_size);

  unsigned read_cnt = fread(a_trg->data + a_trg->used,1,a_size,*this);

  if (read_cnt < a_size)
  {
    // - ERROR -
    if (ferror(*this))
    {
      throw_error(STREAM_READ_ERROR);
    }
  }

  a_trg->used += read_cnt;

  return 0;
}/*}}}*/

