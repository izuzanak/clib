
#ifndef __CL_HTTP_H
#define __CL_HTTP_H

@begin
include "cl_tcp.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libhttp_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libhttp_cll_EXPORTS
#define libhttp_cll_EXPORT __declspec(dllexport)
#else
#define libhttp_cll_EXPORT __declspec(dllimport)
#endif
#endif

typedef int (*http_conn_request_callback_t)(void *a_object,unsigned a_index,int a_complete);
typedef int (*http_conn_response_callback_t)(void *a_object,unsigned a_index);

// - error codes -
#define ERROR_HTTP_SERVER_TCP_SERVER_CREATE_ERROR 1
#define ERROR_HTTP_SERVER_HTTP_REQUEST_ERROR 2
#define ERROR_HTTP_SERVER_HTTP_HEADERS_ERROR 3

#define ERROR_HTTP_CONN_CALLBACK_ERROR 1
#define ERROR_HTTP_CONN_URL_DECODING_ERROR 2

// - maximal message chunk size -
#define HTTP_MAX_CHUNK_SIZE (5*1024*1024)

// - http connection state -
enum
{
  c_http_conn_state_REQUEST = 0,
  c_http_conn_state_HEADERS,
  c_http_conn_state_BODY,
};

// - http request methods -
enum
{
  c_http_method_GET = 0,
  c_http_method_HEAD,
  c_http_method_POST,
  c_http_method_PUT,
};

// - http request terminals -
enum
{
  c_http_req_term_GET = 0,
  c_http_req_term_HEAD,
  c_http_req_term_POST,
  c_http_req_term_PUT,

  c_http_req_term_COMPONENT,
  c_http_req_term_ENC_COMPONENT,

  c_http_req_term_ID,
  c_http_req_term_ENC_ID,

  c_http_req_term_VALUE,
  c_http_req_term_ENC_VALUE,

  c_http_req_term_HTTP,
  c_http_req_term_WHITESPACE,

  // - exit masks -
  c_http_exit_mask_COMPONENTS =
    (1 << c_http_req_term_ID) |
    (1 << c_http_req_term_ENC_ID) |
    (1 << c_http_req_term_WHITESPACE),
};

// - http header terminals -
enum
{
  c_http_head_term_SEPARATOR = 0,
  c_http_head_term_END,
  c_http_head_term_HEADER,
  c_http_head_term_CONTENT_TYPE,
  c_http_head_term_CONTENT_LENGTH,
};

// === definition of generated structures ======================================

// -- bc_buffer_s --
@begin
struct
<
ui:begin
ui:length
>
bc_buffer_s;
@end

// -- bc_buffers_s --
@begin
array<bc_buffer_s> bc_buffers_s;
@end

// -- bc_buffer_pair_s --
@begin
struct
<
bc_buffer_s:id
bc_buffer_s:value
>
bc_buffer_pair_s;
@end

// -- bc_buffer_pairs_s --
@begin
array<bc_buffer_pair_s> bc_buffer_pairs_s;
@end

// -- http_conn_s --
@begin
struct
<
ui:state
ui:input_idx
ui:headers_size
ulli:message_size

ui:method
bc_buffers_s:components
bc_buffer_pairs_s:params
bc_buffer_s:http

bc_buffer_pairs_s:headers

bc_array_s:headers_data
ulli:receive_size
>
http_conn_s;
@end

unsigned http_conn_s_recognize_request_terminal(http_conn_s *this,const bc_array_s *a_source);
unsigned http_conn_s_recognize_header_terminal(http_conn_s *this,const bc_array_s *a_source);
unsigned http_conn_s_decode_url(char *a_ptr,const char *a_ptr_end);

// -- http_conns_s --
@begin
array<http_conn_s> http_conns_s;
@end

// -- http_server_s --
@begin
struct
<
tcp_server_s:server

pointer:conn_request_callback
pointer:conn_response_callback
pointer:cb_object

http_conns_s:conns
>
http_server_s;
@end

WUR libhttp_cll_EXPORT int http_server_s_create(http_server_s *this,
    const char *a_ip,unsigned short a_port,
    http_conn_request_callback_t a_conn_request_callback,
    http_conn_response_callback_t a_conn_response_callback,
    void *a_cb_object);
WUR libhttp_cll_EXPORT int http_server_s_tcp_conn_new(void *a_http_server,unsigned a_index);
WUR libhttp_cll_EXPORT int http_server_s_tcp_conn_drop(void *a_http_server,unsigned a_index);
WUR libhttp_cll_EXPORT int http_server_s_tcp_conn_recv(void *a_http_server,unsigned a_index,bc_array_s *a_message);
WUR libhttp_cll_EXPORT int http_server_s_tcp_conn_send(void *a_http_server,unsigned a_index);

// === inline methods of generated structures ==================================

// -- bc_buffer_s --
@begin
inlines bc_buffer_s
@end

// -- bc_buffers_s --
@begin
inlines bc_buffers_s
@end

// -- bc_buffer_pair_s --
@begin
inlines bc_buffer_pair_s
@end

// -- bc_buffer_pairs_s --
@begin
inlines bc_buffer_pairs_s
@end

// -- http_conn_s --
@begin
inlines http_conn_s
@end

// -- http_conns_s --
@begin
inlines http_conns_s
@end

// -- http_server_s --
@begin
inlines http_server_s
@end

#endif

