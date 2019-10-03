
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libsegfile_sdl_test";

const char *test_names[] =
{/*{{{*/
  "segfile",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_segfile,
};/*}}}*/

// === test execution functions ================================================

void test_segfile()
{/*{{{*/
  CONT_INIT_CLEAR(sd_segment_file_s,segfile);
  cassert(sd_segment_file_s_create(&segfile,"tests/libsegfile_sdl_test/segfile/segfile.img",512) == 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  time_s time;

  // - sd_segment_file_s_get_record -
  bc_array_s record;
  if (sd_segment_file_s_get_record(&segfile,&time,&record))
  {
    fprintf(stderr,"No record created yet\n");
  }
  else
  {
    fprintf(stderr,"Record: %.*s\n",record.used,record.data);
  }

  //fprintf(stderr,"BEFORE: ");
  //DEBUG_PRINT(sd_record_s,(sd_record_s *)segfile.rec_data.data);

  cassert(clock_s_gettime(CLOCK_REALTIME,&time) == 0);

  buffer.used = 0;
  bc_array_s_append_format(&buffer,"Hello segment: ");
  time_s_to_string(&time,&buffer);

  // - sd_segment_file_s_write_record -
  cassert(sd_segment_file_s_write_record(&segfile,
        time,buffer.used,buffer.data) == 0);

  // - sd_segment_file_s_get_record -
  cassert(sd_segment_file_s_get_record(&segfile,&time,&record) == 0);
  fprintf(stderr,"Record: %.*s\n",record.used,record.data);

  //fprintf(stderr,"AFTER: ");
  //DEBUG_PRINT(sd_record_s,(sd_record_s *)segfile.rec_data.data);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

