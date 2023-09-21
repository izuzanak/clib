
@begin
include "cl_cont.h"
@end

// === methods of generated structures =========================================

// -- bc_array_s --
@begin
methods bc_array_s
@end

void bc_array_s_append_format_ap(bc_array_s *this,const char *a_format,va_list a_ap)
{/*{{{*/
  int reserved = this->size - this->used;

  va_list ap;
  va_copy(ap,a_ap);
  int length = vsnprintf(this->data + this->used,reserved,a_format,ap);
  va_end(ap);

  if (length < reserved)
  {
    this->used += length;
  }
  else
  {
    unsigned old_used = this->used;

    unsigned size = length + 1;
    bc_array_s_push_blanks(this,size);

    vsnprintf(this->data + old_used,size,a_format,a_ap);

    // - remove terminating character -
    --this->used;
  }
}/*}}}*/

// -- bc_arrays_s --
@begin
methods bc_arrays_s
@end

// -- json_nice_s --
@begin
methods json_nice_s
@end

// -- bc_block_s --
@begin
methods bc_block_s
@end

// -- ui_array_s --
@begin
methods ui_array_s
@end

unsigned ui_binary_search(const unsigned *a_data,unsigned a_size,unsigned a_value)
{/*{{{*/
  if (a_size == 0)
  {
    return c_idx_not_exist;
  }

  unsigned left = 0;
  unsigned right = a_size - 1;

  do {
    unsigned next = (left + right) >> 1;

    if (a_data[next] < a_value)
    {
      if (next < right)
      {
        left = next + 1;
        continue;
      }

      break;
    }

    if (a_data[next] > a_value)
    {
      if (next > left)
      {
        right = next - 1;
        continue;
      }

      break;
    }

    return next;
  } while(1);

  return c_idx_not_exist;
}/*}}}*/

#if OPTION_MEMCHECK == ENABLED
#define check_malloc(SIZE) malloc(SIZE)
#define check_realloc(LOCATION,SIZE) realloc(LOCATION,SIZE)
#define check_free(LOCATION) free(LOCATION)

// -- mc_block_s --
@begin
methods mc_block_s
@end

// -- mc_block_tree_s --
@begin
methods mc_block_tree_s
@end

// -- memcheck_s --
memcheck_s *g_memcheck = NULL;

@begin
methods memcheck_s
@end

void memcheck_init()
{/*{{{*/
  if (g_memcheck == NULL)
  {
    g_memcheck = (memcheck_s *)cmalloc(sizeof(memcheck_s));
    memcheck_s_init(g_memcheck);

    // - initialize memcheck stat values -
    g_memcheck->alloc_size = 0;
    g_memcheck->total_alloc_size = 0;
    g_memcheck->max_alloc_size = 0;
  }
}/*}}}*/

void memcheck_release()
{/*{{{*/
  if (g_memcheck != NULL)
  {
    memcheck_s_print(g_memcheck);
    memcheck_s_clear(g_memcheck);
    cfree(g_memcheck);
    g_memcheck = NULL;
  }
}/*}}}*/

void memcheck_release_assert()
{/*{{{*/
  if (g_memcheck != NULL)
  {
    memcheck_s_print(g_memcheck);
    cassert(g_memcheck->alloc_size == 0);
    memcheck_s_clear(g_memcheck);
    cfree(g_memcheck);
    g_memcheck = NULL;
  }
}/*}}}*/

void memcheck_s_print(memcheck_s *this)
{/*{{{*/
#define COMMAND_SIZE 256

  char command[COMMAND_SIZE];
  cassert(snprintf(command,COMMAND_SIZE,"ls -l /proc/%d/fd 1>&2",(int)getpid()) < COMMAND_SIZE);
  cassert(system(command) == 0); // NOLINT

  fprintf(stderr,
"memcheck (%u):\n"
"         allocated: %" HOST_LL_FORMAT "u\n"
"   total allocated: %" HOST_LL_FORMAT "u\n"
" maximal allocated: %" HOST_LL_FORMAT "u\n"
  ,this->mc_block_set.size
  ,this->alloc_size
  ,this->total_alloc_size
  ,this->max_alloc_size);
}/*}}}*/

#undef check_malloc
#undef check_realloc
#undef check_free
#endif

