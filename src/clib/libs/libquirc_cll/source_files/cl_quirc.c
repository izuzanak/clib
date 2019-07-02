
@begin
include "cl_quirc.h"
@end

// === methods of structure quirc_s ============================================

int quirc_s_create(quirc_s *this,int a_width,int a_height)
{/*{{{*/
  quirc_s_clear(this);

  // - ERROR -
  if (a_width <= 0 || a_height <= 0)
  {
    throw_error(QUIRC_INVALID_IMAGE_SIZE_PROPERTIES);
  }

  // - ERROR -
  if ((*this = quirc_new()) == NULL)
  {
    throw_error(QUIRC_CREATE_INSTANCE_ERROR);
  }

  // - ERROR -
  if (quirc_resize(*this,a_width,a_height))
  {
    quirc_destroy(*this);

    throw_error(QUIRC_CREATE_INSTANCE_ERROR);
  }

  return 0;
}/*}}}*/

// === methods of structure quirc_code_s =======================================

// === methods of structure quirc_data_s =======================================

