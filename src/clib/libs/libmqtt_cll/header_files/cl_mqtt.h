
#ifndef __CL_MQTT_H
#define __CL_MQTT_H

@begin
include "cl_logger.h"
include "cl_tcp.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libmqtt_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libmqtt_cll_EXPORTS
#define libmqtt_cll_EXPORT __declspec(dllexport)
#else
#define libmqtt_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_MQTT_CONN_EPOLL_ERROR 1
#define ERROR_MQTT_CONN_TIMER_READ_ERROR 2
#define ERROR_MQTT_CONN_CALLBACK_ERROR 3
#define ERROR_MQTT_CONN_SCHEDULE_MESSAGE_ERROR 4
#define ERROR_MQTT_CONN_SEND_MESSAGE_ERROR 5
#define ERROR_MQTT_CONN_PROCESS_PACKET_ERROR 6
#define ERROR_MQTT_CONN_PROCESS_PROPERTIES_ERROR 7
#define ERROR_MQTT_CONN_GET_TIME_ERROR 8
#define ERROR_MQTT_CONN_OUT_OF_PACKET_IDENTIFIERS 9
#define ERROR_MQTT_CONN_NEXT_PACKET_ID_ERROR 10
#define ERROR_MQTT_INVALID_VARIABLE_BYTE_INTEGER 11
#define ERROR_MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE 12
#define ERROR_MQTT_UNKNOWN_CONTROL_PACKET_TYPE 13
#define ERROR_MQTT_INVALID_CONTROL_PACKET_PROPERTY 14
#define ERROR_MQTT_INVALID_CONNECT_PACKET 15
#define ERROR_MQTT_INVALID_CONNACK_PACKET 16
#define ERROR_MQTT_INVALID_PUBLISH_PACKET 17
#define ERROR_MQTT_INVALID_PUBACK_PACKET 18
#define ERROR_MQTT_INVALID_PUBREC_PACKET 19
#define ERROR_MQTT_INVALID_PUBREL_PACKET 20
#define ERROR_MQTT_INVALID_PUBCOMP_PACKET 21
#define ERROR_MQTT_INVALID_SUBSCRIBE_PACKET 22
#define ERROR_MQTT_INVALID_SUBACK_PACKET 23
#define ERROR_MQTT_INVALID_UNSUBSCRIBE_PACKET 24
#define ERROR_MQTT_INVALID_UNSUBACK_PACKET 25
#define ERROR_MQTT_SEND_PUBLISH_ERROR 26
#define ERROR_MQTT_SEND_PUBREL_ERROR 27
#define ERROR_MQTT_SEND_SUBSCRIBE_ERROR 28
#define ERROR_MQTT_SEND_UNSUBSCRIBE_ERROR 29
#define ERROR_MQTT_INVALID_PUBLISH_REQUEST 30
#define ERROR_MQTT_INVALID_SUBSCRIBE_REQUEST 31
#define ERROR_MQTT_INVALID_UNSUBSCRIBE_REQUEST 32

typedef int (*cl_mqtt_cbreq_t)(void *a_object,unsigned a_index,unsigned a_type,va_list a_ap);

// === constants and definitions ===============================================

#define MQTT_VAR_BYTE_INT_MAX_ONE   0x80
#define MQTT_VAR_BYTE_INT_MAX_TWO   0x4000
#define MQTT_VAR_BYTE_INT_MAX_THREE 0x200000
#define MQTT_VAR_BYTE_INT_MAX_FOUR  0x10000000

#define MQTT_TCP_RETRY_INTERVAL {{0,0},{0,500000000ULL}}
#define MQTT_TCP_CHECK_INTERVAL {{1,0},{1,0}}

#define MQTT_PING_PERIOD  5000000000ULL
#define MQTT_PING_TIMEOUT 30000000000ULL

// - callback reasons -
enum
{/*{{{*/
  cl_mqtt_cbreq_ERROR = 0,
  cl_mqtt_cbreq_CONNECTED,
  cl_mqtt_cbreq_DISCONNECTED,
  cl_mqtt_cbreq_DROPPED,

  cl_mqtt_cbreq_PUBLISHED,
  cl_mqtt_cbreq_SUBSCRIBED,
  cl_mqtt_cbreq_UNSUBSCRIBED,
  cl_mqtt_cbreq_RECEIVED,
};/*}}}*/

// - type of control packet -
enum
{/*{{{*/
  MQTT_PACKET_TYPE_CONNECT     = 1,
  MQTT_PACKET_TYPE_CONNACK     = 2,
  MQTT_PACKET_TYPE_PUBLISH     = 3,
  MQTT_PACKET_TYPE_PUBACK      = 4,
  MQTT_PACKET_TYPE_PUBREC      = 5,
  MQTT_PACKET_TYPE_PUBREL      = 6,
  MQTT_PACKET_TYPE_PUBCOMP     = 7,
  MQTT_PACKET_TYPE_SUBSCRIBE   = 8,
  MQTT_PACKET_TYPE_SUBACK      = 9,
  MQTT_PACKET_TYPE_UNSUBSCRIBE = 10,
  MQTT_PACKET_TYPE_UNSUBACK    = 11,
  MQTT_PACKET_TYPE_PINGREQ     = 12,
  MQTT_PACKET_TYPE_PINGRESP    = 13,
  MQTT_PACKET_TYPE_DISCONNECT  = 14,
  MQTT_PACKET_TYPE_AUTH        = 15,

  // - not mqtt standard -
  MQTT_PACKET_TYPE_WILLPROPS = 16,
};/*}}}*/

// - type of control packet data -
enum
{/*{{{*/
  MQTT_DATA_TYPE_BYTE = 0,
  MQTT_DATA_TYPE_TWO_BYTE_INTEGER,
  MQTT_DATA_TYPE_FOUR_BYTE_INTEGER,
  MQTT_DATA_TYPE_VARIABLE_BYTE_INTEGER,
  MQTT_DATA_TYPE_UTF_8_ENCODED_STRING,
  MQTT_DATA_TYPE_UTF_8_STRING_PAIR,
  MQTT_DATA_TYPE_BINARY_DATA,
};/*}}}*/

// - mqtt ping modes -
enum
{/*{{{*/
  MQTT_PING_MODE_OFF = 0,    // ping is not send at all
  MQTT_PING_MODE_CONTINUOUS, // ping is send continuously
  MQTT_PING_MODE_IDLE_COMM,  // ping is send when data communication is idle
  MQTT_PING_MODE_COUNT,      // count of ping modes
};/*}}}*/

// === definition of generated structures ======================================

// -- usi_queue_s --
@begin
queue<usi> usi_queue_s;
@end

// -- mqtt_buffer_s --
@begin
struct
<
ui:size
cbc_pointer:data
>
mqtt_buffer_s;
@end

// -- mqtt_prop_s --
@begin
struct
<
uc:code
ui:offset
>
mqtt_prop_s;
@end

// -- mqtt_prop_array_s --
@begin
array<mqtt_prop_s> mqtt_prop_array_s;
@end

// -- mqtt_prop_descr_s --
@begin
struct
<
cbc_pointer:descr
ui:control_pkt_mask
uc:code
uc:type
>
mqtt_prop_descr_s;
@end

// -- mqtt_publish_s --
@begin
struct
<
usi:packet_id
bi:released
string_s:topic
bc_array_s:payload
uc:qos
bi:retain
bc_array_s:props
>
mqtt_publish_s;
@end

// -- mqtt_publish_array_s --
@begin
array<mqtt_publish_s> mqtt_publish_array_s;
@end

// -- mqtt_subscribe_s --
@begin
struct
<
usi:packet_id
string_array_s:filters
uc:max_qos
bc_array_s:props
>
mqtt_subscribe_s;
@end

// -- mqtt_subscribe_array_s --
@begin
array<mqtt_subscribe_s> mqtt_subscribe_array_s;
@end

// -- mqtt_conn_s --
@begin
struct
<
string_s:server_ip
usi:server_port
tcp_conn_s:connection

bc_array_s:connect_props
mqtt_publish_s:will
string_s:user_name
string_s:password
string_s:client_id

usi:next_packet_id
usi_queue_s:packet_ids

mqtt_publish_array_s:published
mqtt_subscribe_array_s:subscribed
bc_array_s:pubrec

usi:packet_id
mqtt_buffer_s:topic
mqtt_buffer_s:payload
mqtt_buffer_s:properties

mqtt_prop_array_s:prop_refs

uc:ping_mode
ulli:ping_period
ulli:ping_timeout

time_s:ping_time
time_s:resp_time

bi:connected
bi:disconnecting

pointer:mqtt_callback
pointer:cb_object
ui:cb_index

epoll_timer_s:epoll_timer
>
mqtt_conn_s;
@end

WUR libmqtt_cll_EXPORT int mqtt_conn_s_create(mqtt_conn_s *this,
  const char *a_client_id,const char *a_server_ip,unsigned short a_server_port,
  cl_mqtt_cbreq_t a_mqtt_callback,
  void *a_cb_object,unsigned a_cb_index);
WUR static inline int mqtt_conn_s_callback(mqtt_conn_s *this,unsigned a_type,...);
WUR static inline int mqtt_conn_s_send_message(mqtt_conn_s *this,bc_array_s *a_message);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_process_properties(mqtt_conn_s *this,uint8_t a_pkt_type,uint32_t a_size,const char *a_data);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_process_packet(mqtt_conn_s *this,uint8_t pkt_type,uint32_t size,const char *data);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_conn_recv(void *a_mqtt_conn,unsigned a_index,bc_array_s *a_message);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_fd_event(void *a_mqtt_conn,unsigned a_index,epoll_event_s *a_epoll_event);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_check_time_event(void *a_mqtt_conn,unsigned a_index,epoll_event_s *a_epoll_event);

static inline void mqtt_two_byte_enc(uint16_t a_value,bc_array_s *a_trg);
static inline uint16_t mqtt_two_byte_dec(const char *a_src);
static inline void mqtt_four_byte_enc(uint32_t a_value,bc_array_s *a_trg);
static inline uint32_t mqtt_four_byte_dec(const char *a_src);
WUR static inline int mqtt_var_byte_len(uint32_t a_value,uint32_t *a_trg);
WUR libmqtt_cll_EXPORT int mqtt_var_byte_enc(uint32_t a_value,bc_array_s *a_trg);
WUR libmqtt_cll_EXPORT int mqtt_var_byte_dec(
  const char *a_src,const char *a_src_end,const char **a_end,uint32_t *a_trg);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_next_packet_id(mqtt_conn_s *this,uint16_t *a_packet_id);

WUR libmqtt_cll_EXPORT int mqtt_conn_s_send_publish(mqtt_conn_s *this,
    mqtt_publish_s *a_publish,int a_dup);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_send_pubrel(mqtt_conn_s *this,
    uint16_t a_packet_id,uint8_t a_reason_code);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_send_subscribe(mqtt_conn_s *this,
    mqtt_subscribe_s *a_subscribe);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_send_unsubscribe(mqtt_conn_s *this,
    mqtt_subscribe_s *a_subscribe);

WUR libmqtt_cll_EXPORT int mqtt_conn_s_publish(mqtt_conn_s *this,
    string_s *a_topic,bc_array_s *a_payload,bc_array_s *a_props,
    uint8_t a_qos, int a_retain,uint16_t *a_packet_id);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_subscribe(mqtt_conn_s *this,
    string_array_s *a_filters,bc_array_s *a_props,
    uint8_t a_max_qos,uint16_t *a_packet_id);
WUR libmqtt_cll_EXPORT int mqtt_conn_s_unsubscribe(mqtt_conn_s *this,
    string_array_s *a_filters,bc_array_s *a_props,
    uint16_t *a_packet_id);

// === inline methods of generated structures ==================================

// -- usi_queue_s --
@begin
inlines usi_queue_s
@end

// -- mqtt_buffer_s --
@begin
inlines mqtt_buffer_s
@end

// -- mqtt_prop_s --
@begin
inlines mqtt_prop_s
@end

// -- mqtt_prop_array_s --
@begin
inlines mqtt_prop_array_s
@end

// -- mqtt_prop_descr_s --
@begin
inlines mqtt_prop_descr_s
@end

// -- mqtt_publish_s --
@begin
inlines mqtt_publish_s
@end

// -- mqtt_publish_array_s --
@begin
inlines mqtt_publish_array_s
@end

// -- mqtt_subscribe_s --
@begin
inlines mqtt_subscribe_s
@end

// -- mqtt_subscribe_array_s --
@begin
inlines mqtt_subscribe_array_s
@end

// -- mqtt_conn_s --
@begin
inlines mqtt_conn_s
@end

static inline int mqtt_conn_s_callback(mqtt_conn_s *this,unsigned a_type,...)
{/*{{{*/
  va_list ap;

  va_start(ap,a_type);
  int res = ((cl_mqtt_cbreq_t)this->mqtt_callback)(this->cb_object,this->cb_index,a_type,ap);
  va_end(ap);

  return res;
}/*}}}*/

static inline int mqtt_conn_s_send_message(mqtt_conn_s *this,bc_array_s *a_message)
{/*{{{*/

  // - log message -
  log_info_2("mqtt conn, --> %u",a_message->used);

  if (tcp_conn_s_schedule_message(&this->connection,a_message))
  {
    throw_error(MQTT_CONN_SCHEDULE_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

static inline void mqtt_two_byte_enc(uint16_t a_value,bc_array_s *a_trg)
{/*{{{*/
  uint16_t net_value = htons(a_value);
  bc_array_s_append(a_trg,2,(char *)&net_value);
}/*}}}*/

static inline uint16_t mqtt_two_byte_dec(const char *a_src)
{/*{{{*/
  uint16_t net_value;
  memcpy(&net_value,a_src,2);
  return ntohs(net_value);
}/*}}}*/

static inline void mqtt_four_byte_enc(uint32_t a_value,bc_array_s *a_trg)
{/*{{{*/
  uint32_t net_value = htonl(a_value);
  bc_array_s_append(a_trg,4,(char *)&net_value);
}/*}}}*/

static inline uint32_t mqtt_four_byte_dec(const char *a_src)
{/*{{{*/
  uint32_t net_value;
  memcpy(&net_value,a_src,4);
  return ntohl(net_value);
}/*}}}*/

static inline int mqtt_var_byte_len(uint32_t a_value,uint32_t *a_trg)
{/*{{{*/
  if (a_value >= MQTT_VAR_BYTE_INT_MAX_FOUR)
  {
    throw_error(MQTT_INVALID_VARIABLE_BYTE_INTEGER);
  }

  *a_trg += a_value < MQTT_VAR_BYTE_INT_MAX_TWO ?
    (a_value < MQTT_VAR_BYTE_INT_MAX_ONE ? 1 : 2)
    : (a_value < MQTT_VAR_BYTE_INT_MAX_THREE ? 3 : 4);

  return 0;
}/*}}}*/

#endif

