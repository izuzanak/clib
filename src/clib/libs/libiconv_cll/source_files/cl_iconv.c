
@begin
include "cl_iconv.h"
@end

// === methods of structure iconv_s ============================================

int iconv_s_convert(iconv_s *this,unsigned a_size,const char *a_src,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(*this != NULL);

  if (a_size > 0)
  {
    // - store original used position -
    unsigned orig_used = a_trg->used;

    // - prepare output space (initially same size as input) -
    unsigned out_size = a_size;
    bc_array_s_push_blanks(a_trg,out_size + 1);

    // - input pointer and bytes left -
    char *inbuf = (char *)a_src;
    size_t inbytesleft = a_size;

    // - output pointer and bytes left -
    char *outbuf = a_trg->data + orig_used;
    size_t outbytesleft = out_size;

    do {

      if (iconv(*this,&inbuf,&inbytesleft,&outbuf,&outbytesleft) == (size_t)(-1))
      {
        switch (errno)
        {
        case E2BIG:
          {
            // - double output buffer -
            unsigned used_so_far = out_size - outbytesleft;
            unsigned new_out_size = out_size << 1;
            unsigned additional = new_out_size - out_size;

            bc_array_s_push_blanks(a_trg,additional + 1);

            // - update pointers (data may have been reallocated) -
            outbuf = a_trg->data + orig_used + used_so_far;
            outbytesleft += additional;

            out_size = new_out_size;
          }
          break;

        // - ERROR -
        case EILSEQ:
        case EINVAL:
          {
            // - remove partial output -
            a_trg->used = orig_used;

            // - reset conversion state -
            iconv(*this,NULL,NULL,NULL,NULL);

            throw_error(ICONV_CONVERT_ERROR);
          }
          break;
        default:
          cassert(0);
        }
      }

    } while(inbytesleft > 0);

    // - adjust used to actual output size -
    a_trg->used = orig_used + (out_size - outbytesleft);
  }

  // - add terminating zero (not counted in used) -
  bc_array_s_push(a_trg,'\0');
  --a_trg->used;

  return 0;
}/*}}}*/
