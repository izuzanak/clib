
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libtrace_cll_test";

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
  //cassert(system("dd if=/dev/zero of=/dev/shm/trace.img bs=30K count=1") == 0);

  CONT_INIT_CLEAR(fd_s,fd);
  fd = open("/dev/shm/trace.img",O_RDWR,0);

  struct stat stat;
  cassert(fstat(fd,&stat) == 0 && stat.st_size > 0);
  ulli storage_size = stat.st_size;

  CONT_INIT_CLEAR(mmap_s,mmap);
  cassert(mmap_s_create(&mmap,NULL,storage_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0) == 0);

  unsigned header_size = 128;
  unsigned ts_trace_size = 4096;
  unsigned rec_size = 512;

  CONT_INIT_CLEAR(trace_s,trace);
  cassert(trace_s_create(&trace,
    mmap.address,header_size,
    mmap.address + header_size,mmap.length - header_size,
    mmap.address + header_size + ts_trace_size,mmap.length - header_size - ts_trace_size,
    rec_size) == 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  bc_array_s_copy_resize(&buffer,rec_size);
  bc_array_s_append_format(&buffer,"Hello trace world!!!");

  time_s time;

  unsigned idx = 0;
  do {
    cassert(clock_s_gettime(CLOCK_REALTIME,&time) == 0);
    cassert(trace_s_write_record(&trace,time,buffer.data) == 0);
  } while(++idx < 5);

  lli tail = trace_s_tail(&trace);
  lli head = trace_s_head(&trace);
  do {
    buffer.used = 0;
    cassert(trace_s_read_record(&trace,tail,&time,&buffer) == 0);
  } while(++tail < head);

  //fprintf(stderr,"HEADER QUEUE:\n");
  //DEBUG_PRINT_LINES(trace_queue_s,&trace.header_queue);
  //fprintf(stderr,"TRACE QUEUE:\n");
  //DEBUG_PRINT_LINES(trace_queue_s,&trace.trace_queue);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libtrace_cll_init(),libtrace_cll_clear());

  return 0;
}/*}}}*/

