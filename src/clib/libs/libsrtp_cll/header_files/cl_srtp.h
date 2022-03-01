
#ifndef CL_LIB_SRTP_H
#define CL_LIB_SRTP_H

@begin
include "cl_struct.h"
@end

#include <srtp2/srtp.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libsrtp_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libsrtp_cll_EXPORTS
#define libsrtp_cll_EXPORT __declspec(dllexport)
#else
#define libsrtp_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_SRTP_DUMMY_ERROR 1

// === definition of structure srtp_policy_s ===================================

typedef srtp_policy_t srtp_policy_s;

@begin
define srtp_policy_s dynamic
@end

static inline void srtp_policy_s_init(srtp_policy_s *this);
static inline void srtp_policy_s_clear(srtp_policy_s *this);
static inline void srtp_policy_s_flush_all(srtp_policy_s *this);
static inline void srtp_policy_s_swap(srtp_policy_s *this,srtp_policy_s *a_second);
static inline void srtp_policy_s_copy(const srtp_policy_s *this,const srtp_policy_s *a_src);
static inline int srtp_policy_s_compare(const srtp_policy_s *this,const srtp_policy_s *a_second);
#if OPTION_TO_STRING == ENABLED
static inline void srtp_policy_s_to_string(const srtp_policy_s *this,bc_array_s *a_trg);
#endif

static inline void srtp_policy_s_crypto_rtp_default(srtp_policy_s *this);
static inline void srtp_policy_s_crypto_rtcp_default(srtp_policy_s *this);

// === definition of global functions ==========================================

void libsrtp_cll_init();
void libsrtp_cll_clear();

// === inline methods of structure srtp_policy_s ===============================

static inline void srtp_policy_s_init(srtp_policy_s *this)
{/*{{{*/
  memset(this,0,sizeof(srtp_policy_s));
  this->next = NULL;
}/*}}}*/

static inline void srtp_policy_s_clear(srtp_policy_s *this)
{/*{{{*/
  srtp_policy_s_init(this);
}/*}}}*/

static inline void srtp_policy_s_flush_all(srtp_policy_s *this)
{/*{{{*/
}/*}}}*/

static inline void srtp_policy_s_swap(srtp_policy_s *this,srtp_policy_s *a_second)
{/*{{{*/
  srtp_policy_s tmp = *this;
  *this = *a_second;
  *a_second = tmp;
}/*}}}*/

static inline void srtp_policy_s_copy(const srtp_policy_s *this,const srtp_policy_s *a_src)
{/*{{{*/
  (void)this;
  (void)a_src;

  cassert(0);
}/*}}}*/

static inline int srtp_policy_s_compare(const srtp_policy_s *this,const srtp_policy_s *a_second)
{/*{{{*/
  (void)this;
  (void)a_second;

  cassert(0);
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void srtp_policy_s_to_string(const srtp_policy_s *this,bc_array_s *a_trg)
{/*{{{*/
  bc_array_s_append_format(a_trg,"srtp_policy_s{%p}",this);
}/*}}}*/
#endif

static inline void srtp_policy_s_crypto_rtp_default(srtp_policy_s *this)
{/*{{{*/
  srtp_crypto_policy_set_rtp_default(&this->rtp);
}/*}}}*/

static inline void srtp_policy_s_crypto_rtcp_default(srtp_policy_s *this)
{/*{{{*/
  srtp_crypto_policy_set_rtcp_default(&this->rtcp);
}/*}}}*/

#endif

