
@begin
include "cl_fuse.h"
@end

// === methods of structure fuse_session_s =====================================

int fuse_session_s_create(fuse_session_s *this,string_array_s *a_args,
    struct fuse_lowlevel_ops *a_fuse_ll_ops,void *a_user_data)
{/*{{{*/
  fuse_session_s_clear(this);

  struct fuse_args args = {};

  if (a_args->used != 0)
  {
    string_s *a_ptr = a_args->data;
    string_s *a_ptr_end = a_ptr + a_args->used;
    do {
      if (fuse_opt_add_arg(&args,a_ptr->data))
      {
        throw_error(FUSE_SESSION_CREATE_ERROR);
      }
    } while(++a_ptr < a_ptr_end);
  }

  this->se = fuse_session_new(&args,a_fuse_ll_ops,sizeof(*a_fuse_ll_ops),a_user_data);
  fuse_opt_free_args(&args);

  // - ERROR -
  if (this->se == NULL)
  {
    throw_error(FUSE_SESSION_CREATE_ERROR);
  }

  return 0;
}/*}}}*/

int fuse_session_s_process(fuse_session_s *this)
{/*{{{*/
  int req_size = fuse_session_receive_buf(this->se,&this->buffer);

  if (req_size < 0)
  {
    throw_error(FUSE_SESSION_RECEIVE_BUFFER_ERROR);
  }

  if (req_size > 0)
  {
    fuse_session_process_buf(this->se,&this->buffer);
  }

  return 0;
}/*}}}*/

// === methods of generated structures =========================================

