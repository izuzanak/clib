
#ifndef __CL_ATOMIC_H
#define __CL_ATOMIC_H

@begin
include "cl_cont.h"
@end

// === definition of structure atomic_s ========================================

@begin
define atomic_s dynamic
@end

typedef struct atomic_s
{
#if ATOMIC_TYPE == ATOMIC_TYPE_GCC
  volatile int counter;
#elif ATOMIC_TYPE == ATOMIC_TYPE_WINDOWS
  volatile LONG counter;
#endif
} atomic_s;

static inline void atomic_s_init(atomic_s *this);
static inline void atomic_s_clear(atomic_s *this);
static inline void atomic_s_swap(atomic_s *this,atomic_s *a_second);
static inline void atomic_s_copy(atomic_s *this,const atomic_s *a_src);
static inline int atomic_s_compare(const atomic_s *this,const atomic_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void atomic_s_to_string(const atomic_s *this,bc_array_s *a_trg);
#endif
static inline int atomic_s_value(atomic_s *this);
static inline void atomic_s_set(atomic_s *this,int a_value);
static inline void atomic_s_inc(atomic_s *this);
static inline void atomic_s_dec(atomic_s *this);
static inline void atomic_s_add(atomic_s *this,int a_value);
static inline int atomic_s_dec_and_test(atomic_s *this);
static inline int atomic_s_inc_and_test(atomic_s *this,int a_test);

// === inline methods of structure atomic_s ====================================

static inline void atomic_s_init(atomic_s *this)
{/*{{{*/
}/*}}}*/

static inline void atomic_s_clear(atomic_s *this)
{/*{{{*/
}/*}}}*/

static inline void atomic_s_swap(atomic_s *this,atomic_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

static inline void atomic_s_copy(atomic_s *this,const atomic_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int atomic_s_compare(const atomic_s *this,const atomic_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void atomic_s_to_string(const atomic_s *this,bc_array_s *a_trg)
{/*{{{*/
  (void)this;

  bc_array_s_append_format(a_trg,"atomic_s{%d}",this->counter);
}/*}}}*/
#endif

static inline int atomic_s_value(atomic_s *this)
{/*{{{*/
#if ATOMIC_TYPE == ATOMIC_TYPE_GCC
  return this->counter;
#elif ATOMIC_TYPE == ATOMIC_TYPE_WINDOWS
  return this->counter;
#else
  cassert(0);
#endif
}/*}}}*/

static inline void atomic_s_set(atomic_s *this,int a_value)
{/*{{{*/
#if ATOMIC_TYPE == ATOMIC_TYPE_GCC
  this->counter = a_value;
#elif ATOMIC_TYPE == ATOMIC_TYPE_WINDOWS
  InterlockedExchange(&this->counter,a_value);
#else
  cassert(0);
#endif
}/*}}}*/

static inline void atomic_s_inc(atomic_s *this)
{/*{{{*/
#if ATOMIC_TYPE == ATOMIC_TYPE_GCC
  __sync_add_and_fetch(&this->counter,1);
#elif ATOMIC_TYPE == ATOMIC_TYPE_WINDOWS
  InterlockedIncrement(&this->counter);
#else
  cassert(0);
#endif
}/*}}}*/

static inline void atomic_s_dec(atomic_s *this)
{/*{{{*/
#if ATOMIC_TYPE == ATOMIC_TYPE_GCC
  __sync_sub_and_fetch(&this->counter,1);
#elif ATOMIC_TYPE == ATOMIC_TYPE_WINDOWS
  InterlockedDecrement(&this->counter);
#else
  cassert(0);
#endif
}/*}}}*/

static inline void atomic_s_add(atomic_s *this,int a_value)
{/*{{{*/
#if ATOMIC_TYPE == ATOMIC_TYPE_GCC
  __sync_add_and_fetch(&this->counter,a_value);
#elif ATOMIC_TYPE == ATOMIC_TYPE_WINDOWS
  InterlockedExchangeAdd(&this->counter,a_value);
#else
  cassert(0);
#endif
}/*}}}*/

static inline int atomic_s_dec_and_test(atomic_s *this)
{/*{{{*/
#if ATOMIC_TYPE == ATOMIC_TYPE_GCC
  return __sync_sub_and_fetch(&this->counter,1) == 0;
#elif ATOMIC_TYPE == ATOMIC_TYPE_WINDOWS
  return InterlockedDecrement(&this->counter) == 0;
#else
  cassert(0);
#endif
}/*}}}*/

static inline int atomic_s_inc_and_test(atomic_s *this,int a_test)
{/*{{{*/
#if ATOMIC_TYPE == ATOMIC_TYPE_GCC
  return __sync_add_and_fetch(&this->counter,1) == a_test;
#elif ATOMIC_TYPE == ATOMIC_TYPE_WINDOWS
  return InterlockedIncrement(&this->counter) == a_test;
#else
  cassert(0);
#endif
}/*}}}*/

#endif

