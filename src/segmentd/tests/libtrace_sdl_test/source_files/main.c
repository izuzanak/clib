
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libtrace_sdl_test";

const char *test_names[] =
{/*{{{*/
  "trace",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_trace,
};/*}}}*/

// === methods of generated structures =========================================

// === test execution functions ================================================

void test_trace()
{/*{{{*/
#define TEST_TRACE_FILE "tests/libtrace_sdl_test/trace/trace.img"

  CONT_INIT_CLEAR(fd_s,fd);
  if ((fd = open(TEST_TRACE_FILE,O_RDWR,0)) == -1)
  {
    cassert(system("dd if=/dev/zero of='" TEST_TRACE_FILE "' bs=30K count=1") == 0); // NOLINT
    cassert(((fd = open(TEST_TRACE_FILE,O_RDWR,0)) != -1));
  }

  struct stat stat;
  cassert(fstat(fd,&stat) == 0 && stat.st_size > 0);
  ulli storage_size = stat.st_size;

  CONT_INIT_CLEAR(mmap_s,mmap);
  cassert(mmap_s_create(&mmap,NULL,storage_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0) == 0);

  unsigned header_size = 128;
  unsigned ts_trace_size = 4096;
  unsigned rec_size = 512;

  CONT_INIT_CLEAR(sd_trace_s,trace);
  cassert(sd_trace_s_create(&trace,
    c_sd_trace_data_type_MMAP,
    mmap.address,header_size,
    NULL,
    mmap.address + header_size + ts_trace_size,mmap.length - (header_size + ts_trace_size),
    c_sd_trace_data_type_MMAP,
    mmap.address + header_size,ts_trace_size,
    NULL,
    rec_size,10,0) == 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  bc_array_s_append_format(&buffer,"Hello trace world!!!");

  time_s time;

  unsigned idx = 0;
  do {
    cassert(clock_s_gettime(CLOCK_REALTIME,&time) == 0);
    cassert(sd_trace_s_write_record(&trace,time,buffer.used,buffer.data) == 0);
  } while(++idx < 256);

  lli tail = sd_trace_s_tail(&trace);
  lli head = sd_trace_s_head(&trace);

  cassert(clock_s_gettime(CLOCK_REALTIME,&time) == 0);

  lli lee_id = sd_trace_s_lee_time(&trace,time);
  lli gre_id = sd_trace_s_gre_time(&trace,time);

  lli lee_id_ms = sd_trace_s_lee_time(&trace,time - 1000000000ULL);
  lli gre_id_ms = sd_trace_s_gre_time(&trace,time - 1000000000ULL);

  // debug output
  fprintf(stderr,"tail: %lld\n",tail);
  fprintf(stderr,"head: %lld\n",head);
  fprintf(stderr,"lee_id: %lld\n",lee_id);
  fprintf(stderr,"gre_id: %lld\n",gre_id);
  fprintf(stderr,"lee_id_ms: %lld\n",lee_id_ms);
  fprintf(stderr,"gre_id_ms: %lld\n",gre_id_ms);

  if (tail != -1 && head != -1)
  {
    do {
      buffer.used = 0;
      cassert(sd_trace_s_read_record(&trace,tail,&time,&buffer) == 0);
    } while(++tail < head);
  }

  //fprintf(stderr,"HEADER QUEUE:\n");
  //DEBUG_PRINT_LINES(sd_trace_queue_s,&trace.header_queue);
  //fprintf(stderr,"TRACE QUEUE:\n");
  //DEBUG_PRINT_LINES(sd_trace_queue_s,&trace.trace_queue);
  //fprintf(stderr,"TIMESTAMP_QUEUE:\n");
  //DEBUG_PRINT_LINES(sd_trace_record_timestamp_queue_s,&trace.timestamp_queue);
  //fprintf(stderr,"TIMESTAMP_TREE:\n");
  //DEBUG_PRINT_LINES(sd_trace_record_timestamp_tree_s,&trace.timestamp_tree);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(librecord_sdl_init(),librecord_sdl_clear());

  return 0;
}/*}}}*/

