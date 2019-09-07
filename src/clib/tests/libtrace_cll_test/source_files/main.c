
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
  CONT_INIT_CLEAR(fd_s,fd);
  fd = open("/dev/shm/loopbackfile.img",O_RDWR,0);

  struct stat stat;
  cassert(fstat(fd,&stat) == 0 && stat.st_size > 0);
  ulli storage_size = stat.st_size;

  CONT_INIT_CLEAR(mmap_s,mmap);
  cassert(mmap_s_create(&mmap,NULL,storage_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0) == 0);

  unsigned rec_size = sizeof(trace_record_s) + 512;
  unsigned rec_cnt = storage_size/rec_size;

  fprintf(stderr,"rec_size: %u, rec_cnt: %u\n",rec_size,rec_cnt);

  CONT_INIT_CLEAR(trace_record_queue_s,queue);
  trace_record_queue_s_set_buffer(&queue,rec_cnt,rec_size,mmap.address);

#if 1
  {
    queue.used = rec_cnt;
    queue.begin = 100;

    unsigned idx = 0;
    while(idx < queue.used)
    {
      trace_record_s *record = trace_record_queue_s_at(&queue,idx);
      fprintf(stderr,"%s\n",record->data);

      ++idx;
    }

    //DEBUG_PRINT_LINES(trace_record_queue_s,&queue);
  }
#else
  unsigned idx = 0;
  do {
    // - remove tail record -
    if (queue.used >= rec_cnt)
    {
      trace_record_queue_s_next(&queue);
    }

    trace_record_queue_s_insert_blank(&queue);
    trace_record_s *record = trace_record_queue_s_last(&queue);

    trace_record_header_s header = {idx,0,123};

    record->header = header;
    snprintf(record->data,512,"{\"record_id\":%u}",idx);

  } while(++idx < (rec_cnt + 100));
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(,);

  return 0;
}/*}}}*/

