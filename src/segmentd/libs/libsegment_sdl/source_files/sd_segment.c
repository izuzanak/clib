
@begin
include "sd_segment.h"
@end

// === constants and definitions ===============================================

// === methods of generated structures =========================================

// -- sd_segment_descr_s --
@begin
methods sd_segment_descr_s
@end

int sd_segment_descr_s_create(sd_segment_descr_s *this,sd_conf_segment_s *a_config)
{/*{{{*/
  sd_segment_descr_s_clear(this);

  // - log message -
  log_info_2("create segment, id: %s",a_config->segment_id.data);

  sd_conf_segment_s_copy(&this->config,a_config);

  if (this->config.type == c_sd_segment_type_FILE)
  {
    sd_segment_file_s *segfile = (sd_segment_file_s *)cmalloc(sizeof(sd_segment_file_s));
    sd_segment_file_s_init(segfile);

    // - create segment handle -
    sd_segment_handle_s_create(&this->handle,segfile,
        (sd_segment_clear_cb_t)sd_segment_file_s_clear,
        (sd_segment_write_record_cb_t)sd_segment_file_s_write_record,
        (sd_segment_get_record_cb_t)sd_segment_file_s_get_record);

    // - create segment file -
    if (sd_segment_file_s_create(segfile,this->config.path.data,this->config.size))
    {
      throw_error(SD_SEGMENT_DESCR_FILE_CREATE_ERROR);
    }
  }
  else
  {
    throw_error(SD_SEGMENT_DESCR_INVALID_SEGMENT_TYPE);
  }

  return 0;
}/*}}}*/

