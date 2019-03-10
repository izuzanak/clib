
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libsys_cll_test";

const char *test_names[] =
{/*{{{*/
  "pollfd",
  "file",
  "pipe",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_pollfd,
  test_file,
  test_pipe,
};/*}}}*/

// === methods of generated structures =========================================

// -- pollfd_array_s --
@begin
methods pollfd_array_s
@end

// -- pollfd_tree_s --
@begin
methods pollfd_tree_s
@end

// -- file_array_s --
@begin
methods file_array_s
@end

// -- pipe_array_s --
@begin
methods pipe_array_s
@end

// === test execution functions ================================================

void test_pollfd()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define POLLFD_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  pollfd_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

#define POLLFD_TREE_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  pollfd_tree_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT(pollfd_array_s,pollfd_array);
  int idx = 0;
  do {
    pollfd_s pollfd = {idx,0,0};
    pollfd_array_s_push(&pollfd_array,&pollfd);
  } while(++idx < 3);
  POLLFD_ARRAY_S_TO_BUFFER(&pollfd_array);
  cassert(strcmp(buffer.data,"[pollfd_s{0,0,0},pollfd_s{1,0,0},pollfd_s{2,0,0}]") == 0);

  CONT_INIT(pollfd_tree_s,pollfd_tree);
  idx = 0;
  do {
    pollfd_s pollfd = {idx,0,0};
    pollfd_tree_s_insert(&pollfd_tree,&pollfd);
  } while(++idx < 3);
  POLLFD_TREE_S_TO_BUFFER(&pollfd_tree);
  cassert(strcmp(buffer.data,"[pollfd_s{0,0,0},pollfd_s{1,0,0},pollfd_s{2,0,0}]") == 0);

  pollfd_tree_s_clear(&pollfd_tree);
  pollfd_array_s_clear(&pollfd_array);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_file()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define FILE_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  file_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT(file_array_s,file_array);
  file_array_s_push_blanks(&file_array,3);
  FILE_ARRAY_S_TO_BUFFER(&file_array);

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
  cassert(strcmp(buffer.data,"[file_s{(nil)},file_s{(nil)},file_s{(nil)}]") == 0);
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
  cassert(strcmp(buffer.data,"[file_s{00000000},file_s{00000000},file_s{00000000}]") == 0);
#endif

  // - file_s_open -
  // - stream_s_write -
  const char *text = "Hello world!\n";
  cassert(file_s_open(&file_array.data[0],"tests/libsys_cll_test/file/write.txt","wb") == 0);
  cassert(file_array.data[0] != NULL);
  cassert(stream_s_write(&file_array.data[0],text,strlen(text)) == 0);

  // - stream_s_write_cnt -
  size_t writed;
  cassert(stream_s_write_cnt(&file_array.data[0],text,strlen(text),&writed) == 0);
  cassert(writed == strlen(text));

  // - stream_s_fflush -
  cassert(stream_s_fflush(&file_array.data[0]) == 0);

  // - stream_s_read -
  cassert(file_s_open(&file_array.data[1],"tests/libsys_cll_test/file/write.txt","rb") == 0);
  buffer.used = 0;
  cassert(stream_s_read(&file_array.data[1],&buffer) == 0);

  CONT_INIT(string_s,string);
  string_s_set_format(&string,"%s%s",text,text);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,string.data) == 0);

  // - file_s_read_close -
  cassert(file_s_open(&file_array.data[2],"tests/libsys_cll_test/file/write.txt","rb") == 0);
  buffer.used = 0;
  cassert(file_s_read_close(&file_array.data[2],&buffer) == 0);
  cassert(file_array.data[2] == NULL);

  string_s_set_format(&string,"%s%s",text,text);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,string.data) == 0);

  // - stream_s_read_cnt -
  cassert(file_s_open(&file_array.data[2],"tests/libsys_cll_test/file/write.txt","rb") == 0);
  buffer.used = 0;
  cassert(stream_s_read_cnt(&file_array.data[2],&buffer,strlen(text)) == 0);

  // - stream_s_read_max -
  cassert(file_s_open(&file_array.data[2],"tests/libsys_cll_test/file/write.txt","rb") == 0);
  buffer.used = 0;
  cassert(stream_s_read_max(&file_array.data[2],&buffer,strlen(text)) == 0);

  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,text) == 0);

  // - file_s_tell -
  // - file_s_seek -
  long offset;
  cassert(file_s_tell(&file_array.data[2],&offset) == 0);
  cassert(offset == strlen(text));
  cassert(file_s_seek(&file_array.data[2],strlen("Hello "),SEEK_CUR) == 0);
  buffer.used = 0;
  cassert(stream_s_read(&file_array.data[2],&buffer) == 0);

  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"world!\n") == 0);

  string_s_clear(&string);
  file_array_s_clear(&file_array);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_pipe()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

#define PIPE_ARRAY_S_TO_BUFFER(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  pipe_array_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

  CONT_INIT(pipe_array_s,pipe_array);
  pipe_array_s_push_blanks(&pipe_array,3);
  PIPE_ARRAY_S_TO_BUFFER(&pipe_array);

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
  cassert(strcmp(buffer.data,"[pipe_s{(nil)},pipe_s{(nil)},pipe_s{(nil)}]") == 0);
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
  cassert(strcmp(buffer.data,"[pipe_s{00000000},pipe_s{00000000},pipe_s{00000000}]") == 0);
#endif

#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
  // - pipe_s_popen -
  // - stream_s_write -
  const char *text = "Hello world!\n";
  cassert(pipe_s_popen(&pipe_array.data[0],"cat > tests/libsys_cll_test/pipe/write.txt","w") == 0);
  cassert(pipe_array.data[0] != NULL);
  cassert(stream_s_write(&pipe_array.data[0],text,strlen(text)) == 0);

  // - stream_s_write_cnt -
  size_t writed;
  cassert(stream_s_write_cnt(&pipe_array.data[0],text,strlen(text),&writed) == 0);
  cassert(writed == strlen(text));

  // - stream_s_fflush -
  // - pipe_s_clear -
  cassert(stream_s_fflush(&pipe_array.data[0]) == 0);
  pipe_s_clear(&pipe_array.data[0]);

  // - stream_s_read -
  cassert(pipe_s_popen(&pipe_array.data[1],"cat tests/libsys_cll_test/pipe/write.txt","r") == 0);
  buffer.used = 0;
  cassert(stream_s_read(&pipe_array.data[1],&buffer) == 0);

  CONT_INIT(string_s,string);
  string_s_set_format(&string,"%s%s",text,text);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,string.data) == 0);

  // - stream_s_read -
  cassert(pipe_s_popen(&pipe_array.data[2],"cat tests/libsys_cll_test/pipe/write.txt","r") == 0);
  buffer.used = 0;
  cassert(pipe_s_read_close(&pipe_array.data[2],&buffer) == 0);
  cassert(pipe_array.data[2] == NULL);

  string_s_set_format(&string,"%s%s",text,text);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,string.data) == 0);

  // - stream_s_read_cnt -
  cassert(pipe_s_popen(&pipe_array.data[2],"cat tests/libsys_cll_test/pipe/write.txt","r") == 0);
  buffer.used = 0;
  cassert(stream_s_read_cnt(&pipe_array.data[2],&buffer,strlen(text)) == 0);

  // - stream_s_read_max -
  cassert(pipe_s_popen(&pipe_array.data[2],"cat tests/libsys_cll_test/pipe/write.txt","r") == 0);
  buffer.used = 0;
  cassert(stream_s_read_max(&pipe_array.data[2],&buffer,strlen(text)) == 0);

  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,text) == 0);

  string_s_clear(&string);
#endif

  pipe_array_s_clear(&pipe_array);
  bc_array_s_clear(&buffer);
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

