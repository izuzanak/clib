
@begin
include "cl_mqtt.h"
@end

#define PACKET_PROP_CNT 43
const mqtt_prop_descr_s g_mqtt_packet_props[PACKET_PROP_CNT] =
{/*{{{*/
  {""                                 , 0x000000, 0xff, 0},
  {"Payload Format Indicator"         , 0x010008, 0x01, MQTT_DATA_TYPE_BYTE},
  {"Message Expiry Interval"          , 0x010008, 0x02, MQTT_DATA_TYPE_FOUR_BYTE_INTEGER},
  {"Content Type"                     , 0x010008, 0x03, MQTT_DATA_TYPE_UTF_8_ENCODED_STRING},
  {""                                 , 0x000000, 0xff, 0},
  {""                                 , 0x000000, 0xff, 0},
  {""                                 , 0x000000, 0xff, 0},
  {""                                 , 0x000000, 0xff, 0},
  {"Response Topic"                   , 0x010008, 0x08, MQTT_DATA_TYPE_UTF_8_ENCODED_STRING},
  {"Correlation Data"                 , 0x010008, 0x09, MQTT_DATA_TYPE_BINARY_DATA},
  {""                                 , 0x000000, 0xff, 0},
  {"Subscription Identifier"          , 0x000108, 0x0b, MQTT_DATA_TYPE_VARIABLE_BYTE_INTEGER},
  {""                                 , 0x000000, 0xff, 0},
  {""                                 , 0x000000, 0xff, 0},
  {""                                 , 0x000000, 0xff, 0},
  {""                                 , 0x000000, 0xff, 0},
  {""                                 , 0x000000, 0xff, 0},
  {"Session Expiry Interval"          , 0x004006, 0x11, MQTT_DATA_TYPE_FOUR_BYTE_INTEGER},
  {"Assigned Client Identifier"       , 0x000004, 0x12, MQTT_DATA_TYPE_UTF_8_ENCODED_STRING},
  {"Server Keep Alive"                , 0x000004, 0x13, MQTT_DATA_TYPE_TWO_BYTE_INTEGER},
  {""                                 , 0x000000, 0xff, 0},
  {"Authentication Method"            , 0x008006, 0x15, MQTT_DATA_TYPE_UTF_8_ENCODED_STRING},
  {"Authentication Data"              , 0x008006, 0x16, MQTT_DATA_TYPE_BINARY_DATA},
  {"Request Problem Information"      , 0x000002, 0x17, MQTT_DATA_TYPE_BYTE},
  {"Will Delay Interval"              , 0x010000, 0x18, MQTT_DATA_TYPE_FOUR_BYTE_INTEGER},
  {"Request Response Information"     , 0x000002, 0x19, MQTT_DATA_TYPE_BYTE},
  {"Response Information"             , 0x000004, 0x1a, MQTT_DATA_TYPE_UTF_8_ENCODED_STRING},
  {""                                 , 0x000000, 0xff, 0},
  {"Server Reference"                 , 0x004004, 0x1c, MQTT_DATA_TYPE_UTF_8_ENCODED_STRING},
  {""                                 , 0x000000, 0xff, 0},
  {""                                 , 0x000000, 0xff, 0},
  {"Reason String"                    , 0x00caf4, 0x1f, MQTT_DATA_TYPE_UTF_8_ENCODED_STRING},
  {""                                 , 0x000000, 0xff, 0},
  {"Receive Maximum"                  , 0x000006, 0x21, MQTT_DATA_TYPE_TWO_BYTE_INTEGER},
  {"Topic Alias Maximum"              , 0x000006, 0x22, MQTT_DATA_TYPE_TWO_BYTE_INTEGER},
  {"Topic Alias"                      , 0x000008, 0x23, MQTT_DATA_TYPE_TWO_BYTE_INTEGER},
  {"Maximum QoS"                      , 0x000004, 0x24, MQTT_DATA_TYPE_BYTE},
  {"Retain Available"                 , 0x000004, 0x25, MQTT_DATA_TYPE_BYTE},
  {"User Property"                    , 0x01cffe, 0x26, MQTT_DATA_TYPE_UTF_8_STRING_PAIR},
  {"Maximum Packet Size"              , 0x000006, 0x27, MQTT_DATA_TYPE_FOUR_BYTE_INTEGER},
  {"Wildcard Subscription Available"  , 0x000004, 0x28, MQTT_DATA_TYPE_BYTE},
  {"Subscription Identifier Available", 0x000004, 0x29, MQTT_DATA_TYPE_BYTE},
  {"Shared Subscription Available"    , 0x000004, 0x2a, MQTT_DATA_TYPE_BYTE},
};/*}}}*/

// === methods of generated structures =========================================

// -- usi_queue_s --
@begin
methods usi_queue_s
@end

// -- mqtt_buffer_s --
@begin
methods mqtt_buffer_s
@end

// -- mqtt_prop_s --
@begin
methods mqtt_prop_s
@end

// -- mqtt_prop_array_s --
@begin
methods mqtt_prop_array_s
@end

// -- mqtt_prop_descr_s --
@begin
methods mqtt_prop_descr_s
@end

// -- mqtt_publish_s --
@begin
methods mqtt_publish_s
@end

// -- mqtt_publish_array_s --
@begin
methods mqtt_publish_array_s
@end

// -- mqtt_subscribe_s --
@begin
methods mqtt_subscribe_s
@end

// -- mqtt_subscribe_array_s --
@begin
methods mqtt_subscribe_array_s
@end

// -- mqtt_conn_s --
@begin
methods mqtt_conn_s
@end

int mqtt_conn_s_create(mqtt_conn_s *this,
  const char *a_client_id,const char *a_server_ip,unsigned short a_server_port,
  cl_mqtt_cbreq_t a_mqtt_callback,
  void *a_cb_object,unsigned a_cb_index)
{/*{{{*/
  mqtt_conn_s_clear(this);

  // - log message -
  log_info_2("mqtt client, create, server_ip: %s, server_port: %hu",
      a_server_ip,a_server_port);

  string_s_set_ptr(&this->server_ip,a_server_ip);
  this->server_port = a_server_port;
  string_s_set_ptr(&this->client_id,a_client_id);

  this->next_packet_id = 1;

  this->packet_id = 0;

  this->ping_mode = MQTT_PING_MODE_CONTINUOUS;
  this->ping_period = MQTT_PING_PERIOD;
  this->ping_timeout = MQTT_PING_TIMEOUT;

  this->ping_time = 0;
  this->resp_time = 0;

  // - reset connected and disconnecting flags -
  this->connected = 0;
  this->disconnecting = 0;

  this->mqtt_callback = a_mqtt_callback;
  this->cb_object = a_cb_object;
  this->cb_index = a_cb_index;

  // - register epoll timer -
  struct itimerspec itimerspec = {{0,0},{0,1}};
  if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,mqtt_conn_s_check_time_event,this,0))
  {
    throw_error(MQTT_CONN_EPOLL_ERROR);
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_process_properties(mqtt_conn_s *this,uint8_t a_pkt_type,uint32_t a_size,const char *a_data)
{/*{{{*/

  // - set properties -
  mqtt_buffer_s_set(&this->properties,a_size,a_data);

  // - reset property references -
  this->prop_refs.used = 0;

  if (a_size > 0)
  {
    // - process all properties -
    const char *data_begin = a_data;
    const char *data_end   = a_data + a_size;
    do {
      uint8_t code = a_data[0];

      const mqtt_prop_descr_s *prop_descr;
      if (code >= PACKET_PROP_CNT || (prop_descr = &g_mqtt_packet_props[code])->code != code
          || (prop_descr->control_pkt_mask & (1 << (a_pkt_type >> 4))) == 0)
      {
        throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
      }

      // - store property reference -
      mqtt_prop_array_s_push_blank(&this->prop_refs);
      mqtt_prop_s_set(mqtt_prop_array_s_last(&this->prop_refs),code,a_data - data_begin);

      switch (prop_descr->type)
      {
        case MQTT_DATA_TYPE_BYTE:
        {/*{{{*/
          if (data_end - a_data < 1 + 1)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          a_data += 1 + 1;
        }/*}}}*/
        break;
        case MQTT_DATA_TYPE_TWO_BYTE_INTEGER:
        {/*{{{*/
          if (data_end - a_data < 1 + 2)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          a_data += 1 + 2;
        }/*}}}*/
        break;
        case MQTT_DATA_TYPE_FOUR_BYTE_INTEGER:
        {/*{{{*/
          if (data_end - a_data < 1 + 4)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          a_data += 1 + 4;
        }/*}}}*/
        break;
        case MQTT_DATA_TYPE_VARIABLE_BYTE_INTEGER:
        {/*{{{*/
          uint32_t value;
          if (mqtt_var_byte_dec(a_data + 1,data_end,&a_data,&value))
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }
        }/*}}}*/
        break;
        case MQTT_DATA_TYPE_UTF_8_ENCODED_STRING:
        {/*{{{*/
          if (data_end - a_data < 1 + 2)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          uint16_t length = mqtt_two_byte_dec(a_data + 1);

          if (data_end - a_data < 1 + 2 + length)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          a_data += 1 + 2 + length;
        }/*}}}*/
        break;
        case MQTT_DATA_TYPE_UTF_8_STRING_PAIR:
        {/*{{{*/
          // - retrieve first string -
          if (data_end - a_data < 1 + 2)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          uint16_t length = mqtt_two_byte_dec(a_data + 1);

          if (data_end - a_data < 1 + 2 + length)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          a_data += 1 + 2 + length;

          // - retrieve second string -
          if (data_end - a_data < 2)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          length = mqtt_two_byte_dec(a_data);

          if (data_end - a_data < 2 + length)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          a_data += 2 + length;
        }/*}}}*/
        break;
        case MQTT_DATA_TYPE_BINARY_DATA:
        {/*{{{*/
          if (data_end - a_data < 1 + 2)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          uint16_t length = mqtt_two_byte_dec(a_data + 1);

          if (data_end - a_data < 1 + 2 + length)
          {
            throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
          }

          a_data += 1 + 2 + length;
        }/*}}}*/
        break;
        default:
          throw_error(MQTT_INVALID_CONTROL_PACKET_PROPERTY);
      }
    } while (a_data < data_end);
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_process_packet(mqtt_conn_s *this,uint8_t pkt_type,uint32_t size,const char *data)
{/*{{{*/
  const char *data_end = data + size;

  if (this->ping_mode == MQTT_PING_MODE_IDLE_COMM)
  {
    // - update response time -
    if (clock_s_gettime(CLOCK_MONOTONIC,&this->resp_time))
    {
      throw_error(MQTT_CONN_GET_TIME_ERROR);
    }
  }

  switch (pkt_type >> 4)
  {
    case MQTT_PACKET_TYPE_CONNECT:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    case MQTT_PACKET_TYPE_CONNACK:
    {/*{{{*/
      if ((pkt_type & 0x0f) != 0 || // reserved
          size < 3)                 // minimal size
      {
        throw_error(MQTT_INVALID_CONNACK_PACKET);
      }

      if ((data[0] & 0xfe) != 0 ||  // connect acknowledge flags
          data[1] != 0x00)          // connect reason code
      {
        throw_error(MQTT_INVALID_CONNACK_PACKET);
      }

      data += 2;

      // - properties length -
      uint32_t props_length;
      if (mqtt_var_byte_dec(data,data_end,&data,&props_length)
          || (uint32_t)(data_end - data) < props_length)
      {
        throw_error(MQTT_INVALID_CONNACK_PACKET);
      }

      if (mqtt_conn_s_process_properties(this,pkt_type,props_length,data))
      {
        throw_error(MQTT_CONN_PROCESS_PROPERTIES_ERROR);
      }

      data += props_length;

      // - set connected flag -
      this->connected = 1;

      // - resend all unacknowledged published packets -
      // - both publish and pubrel packets are resend -
      if (this->published.used != 0)
      {
        mqtt_publish_s *p_ptr = this->published.data;
        mqtt_publish_s *p_ptr_end = p_ptr + this->published.used;
        do {
          if (p_ptr->packet_id != 0)
          {
            if (p_ptr->released)
            {
              if (mqtt_conn_s_send_pubrel(this,p_ptr->packet_id,0x00))
              {
                throw_error(MQTT_SEND_PUBREL_ERROR);
              }
            }
            else
            {
              if (mqtt_conn_s_send_publish(this,p_ptr,1))
              {
                throw_error(MQTT_SEND_PUBLISH_ERROR);
              }
            }
          }
        } while(++p_ptr < p_ptr_end);
      }

      // - call callback -
      if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_CONNECTED))
      {
        throw_error(MQTT_CONN_CALLBACK_ERROR);
      }

      if (this->ping_mode == MQTT_PING_MODE_CONTINUOUS)
      {
        // - update response time -
        if (clock_s_gettime(CLOCK_MONOTONIC,&this->resp_time))
        {
          throw_error(MQTT_CONN_GET_TIME_ERROR);
        }
      }

    } /*}}}*/
    break;
    case MQTT_PACKET_TYPE_PUBLISH:
    {
      uint8_t dup    = (pkt_type & 0x04) >> 3;
      uint8_t qos    = (pkt_type & 0x06) >> 1;
      uint8_t retain = pkt_type & 0x01;
      (void)retain;

      uint32_t topic_length;
      if (data_end - data < 2
          || (uint32_t)(data_end - data) < 2 + (topic_length = mqtt_two_byte_dec(data)))
      {
        throw_error(MQTT_INVALID_PUBLISH_PACKET);
      }

      mqtt_buffer_s_set(&this->topic,topic_length,data + 2);
      data += 2 + topic_length;

      this->packet_id = 0;
      if (qos > 0)
      {
        if (data_end - data < 2)
        {
          throw_error(MQTT_INVALID_PUBLISH_PACKET);
        }

        this->packet_id = mqtt_two_byte_dec(data);
        data += 2;
      }

      // - properties -
      uint32_t props_length;
      if (mqtt_var_byte_dec(data,data_end,&data,&props_length)
          || (uint32_t)(data_end - data) < props_length)
      {
        throw_error(MQTT_INVALID_PUBLISH_PACKET);
      }

      if (mqtt_conn_s_process_properties(this,pkt_type,props_length,data))
      {
        throw_error(MQTT_CONN_PROCESS_PROPERTIES_ERROR);
      }

      data += props_length;

      uint32_t payload_length = data_end - data;
      mqtt_buffer_s_set(&this->payload,payload_length,data);

      switch (qos)
      {
        case 0:
          // - call callback -
          if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_RECEIVED))
          {
            throw_error(MQTT_CONN_CALLBACK_ERROR);
          }
          break;
        case 1:
        {
          // - call callback -
          if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_RECEIVED))
          {
            throw_error(MQTT_CONN_CALLBACK_ERROR);
          }

          // - send puback packet -
          CONT_INIT_CLEAR(bc_array_s,buffer);
          bc_array_s_reserve(&buffer,4);

          bc_array_s_push(&buffer,0x40); // PUBACK
          bc_array_s_push(&buffer,0x02); // remaining length

          // - packet identifier -
          mqtt_two_byte_enc(this->packet_id,&buffer);

          cassert(buffer.used == 4);

          if (mqtt_conn_s_send_message(this,&buffer))
          {
            throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
          }
        }
        break;
        case 2:
        {
          // - resize pubrec vector -
          while (this->packet_id >= this->pubrec.used)
          {
            bc_array_s_push(&this->pubrec,0);
          }

          // - not duplicate or not received yet -
          if (dup == 0 || !this->pubrec.data[this->packet_id])
          {
            // - call callback -
            if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_RECEIVED))
            {
              throw_error(MQTT_CONN_CALLBACK_ERROR);
            }

            // - mark packet as received -
            this->pubrec.data[this->packet_id] = 1;
          }

          // - send pubrec packet -
          CONT_INIT_CLEAR(bc_array_s,buffer);
          bc_array_s_reserve(&buffer,4);

          bc_array_s_push(&buffer,0x50); // PUBREC
          bc_array_s_push(&buffer,0x02); // remaining length

          // - packet identifier -
          mqtt_two_byte_enc(this->packet_id,&buffer);

          cassert(buffer.used == 4);

          if (mqtt_conn_s_send_message(this,&buffer))
          {
            throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
          }
        }
        break;
      }
    }
    break;
    case MQTT_PACKET_TYPE_PUBACK:
    {/*{{{*/
      if ((pkt_type & 0x0f) != 0 || // reserved
          size < 2)                 // minimal size
      {
        throw_error(MQTT_INVALID_PUBACK_PACKET);
      }

      // - packet identifier -
      this->packet_id = mqtt_two_byte_dec(data);
      data += 2;

      // - optional reason code,defaults to 0x00 -
      if (data < data_end)
      {
        // - reason code -
        if ((uint8_t)(*data++) >= 0x80)
        {
          throw_error(MQTT_INVALID_PUBACK_PACKET);
        }

        // - optional properties -
        if (data < data_end)
        {
          // - properties -
          uint32_t props_length;
          if (mqtt_var_byte_dec(data,data_end,&data,&props_length)
              || (uint32_t)(data_end - data) < props_length)
          {
            throw_error(MQTT_INVALID_PUBACK_PACKET);
          }

          if (mqtt_conn_s_process_properties(this,pkt_type,props_length,data))
          {
            throw_error(MQTT_CONN_PROCESS_PROPERTIES_ERROR);
          }

          data += props_length;
        }
      }

      mqtt_publish_s *publish;

      // - unknown packet id or invalid qos -
      if (this->packet_id >= this->published.used
          || (publish = mqtt_publish_array_s_at(&this->published,this->packet_id))->packet_id != this->packet_id
          || publish->qos != 1)
      {
        throw_error(MQTT_INVALID_PUBACK_PACKET);
      }

      // - release packet id -
      usi_queue_s_insert(&this->packet_ids,this->packet_id);

      // - reset publish data -
      mqtt_publish_s_clear(publish);
      publish->packet_id = 0;

      // - call callback -
      if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_PUBLISHED))
      {
        throw_error(MQTT_CONN_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
    case MQTT_PACKET_TYPE_PUBREC:
    {/*{{{*/
      if ((pkt_type & 0x0f) != 0 || // reserved
          size < 2)                 // minimal size
      {
        throw_error(MQTT_INVALID_PUBREC_PACKET);
      }

      // - packet identifier -
      this->packet_id = mqtt_two_byte_dec(data);
      data += 2;

      // - optional reason code,defaults to 0x00 -
      if (data < data_end)
      {
        // - reason code -
        if ((uint8_t)(*data++) >= 0x80)
        {
          throw_error(MQTT_INVALID_PUBREC_PACKET);
        }

        // - optional properties -
        if (data < data_end)
        {
          // - properties -
          uint32_t props_length;
          if (mqtt_var_byte_dec(data,data_end,&data,&props_length)
              || (uint32_t)(data_end - data) < props_length)
          {
            throw_error(MQTT_INVALID_PUBREC_PACKET);
          }

          if (mqtt_conn_s_process_properties(this,pkt_type,props_length,data))
          {
            throw_error(MQTT_CONN_PROCESS_PROPERTIES_ERROR);
          }

          data += props_length;
        }
      }

      // - default reason code: Packet identifier not found -
      uint8_t reason_code = 0x92;

      mqtt_publish_s *publish;

      // - packet id is ok and qos matches -
      if (this->packet_id < this->pubrec.used
          && (publish = mqtt_publish_array_s_at(&this->published,this->packet_id))->packet_id != this->packet_id
          && publish->qos == 2)
      {
        // - reason code: Success -
        reason_code = 0x00;

        // - mark publish as released -
        publish->released = 1;

        // - release data buffers -
        string_s_clear(&publish->topic);
        bc_array_s_clear(&publish->payload);
        bc_array_s_clear(&publish->props);
      }

      if (mqtt_conn_s_send_pubrel(this,this->packet_id,reason_code))
      {
        throw_error(MQTT_SEND_PUBREL_ERROR);
      }
    }/*}}}*/
    break;
    case MQTT_PACKET_TYPE_PUBREL:
    {/*{{{*/
      if ((pkt_type & 0x0f) != 0x02 || // reserved
          size < 2)                    // minimal size
      {
        throw_error(MQTT_INVALID_PUBREL_PACKET);
      }

      // - packet identifier -
      this->packet_id = mqtt_two_byte_dec(data);
      data += 2;

      // - optional reason code,defaults to 0x00 -
      if (data < data_end)
      {
        // - reason code -
        if ((uint8_t)(*data++) >= 0x80)
        {
          throw_error(MQTT_INVALID_PUBREL_PACKET);
        }

        // - optional properties -
        if (data < data_end)
        {
          // - properties -
          uint32_t props_length;
          if (mqtt_var_byte_dec(data,data_end,&data,&props_length)
              || (uint32_t)(data_end - data) < props_length)
          {
            throw_error(MQTT_INVALID_PUBREL_PACKET);
          }

          if (mqtt_conn_s_process_properties(this,pkt_type,props_length,data))
          {
            throw_error(MQTT_CONN_PROCESS_PROPERTIES_ERROR);
          }

          data += props_length;
        }
      }

      // - default reason code: Packet identifier not found -
      uint8_t reason_code = 0x92;

      // - packet id is ok -
      if (this->packet_id < this->pubrec.used && this->pubrec.data[this->packet_id])
      {
        // - reason code: Success -
        reason_code = 0x00;

        // - reset pubrec flag -
        this->pubrec.data[this->packet_id] = 0;
      }

      // - send pubcomp packet -
      uint32_t remaining_length = 2 + (reason_code == 0x00 ? 0 : 1);

      CONT_INIT_CLEAR(bc_array_s,buffer);
      bc_array_s_reserve(&buffer,2 + remaining_length);

      bc_array_s_push(&buffer,0x70);  // PUBCOMP
      bc_array_s_push(&buffer,remaining_length);

      // - packet identifier -
      mqtt_two_byte_enc(this->packet_id,&buffer);

      // - reason code -
      if (reason_code != 0x00)
      {
        bc_array_s_push(&buffer,reason_code);
      }

      cassert(buffer.used == 2 + remaining_length);

      if (mqtt_conn_s_send_message(this,&buffer))
      {
        throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
      }
    }/*}}}*/
    break;
    case MQTT_PACKET_TYPE_PUBCOMP:
    {/*{{{*/
      if ((pkt_type & 0x0f) != 0 ||  // reserved
          size < 2)                  // minimal size
      {
        throw_error(MQTT_INVALID_PUBCOMP_PACKET);
      }

      // - packet identifier -
      this->packet_id = mqtt_two_byte_dec(data);
      data += 2;

      // - optional reason code,defaults to 0x00 -
      if (data < data_end)
      {
        // - reason code -
        if ((uint8_t)(*data++) >= 0x80)
        {
          throw_error(MQTT_INVALID_PUBCOMP_PACKET);
        }

        // - optional properties -
        if (data < data_end)
        {
          // - properties -
          uint32_t props_length;
          if (mqtt_var_byte_dec(data,data_end,&data,&props_length)
              || (uint32_t)(data_end - data) < props_length)
          {
            throw_error(MQTT_INVALID_PUBCOMP_PACKET);
          }

          if (mqtt_conn_s_process_properties(this,pkt_type,props_length,data))
          {
            throw_error(MQTT_CONN_PROCESS_PROPERTIES_ERROR);
          }

          data += props_length;
        }
      }

      mqtt_publish_s *publish;

      // - unknown packet id or invalid qos -
      if (this->packet_id >= this->published.used
          || (publish = mqtt_publish_array_s_at(&this->published,this->packet_id))->packet_id != this->packet_id
          || publish->qos != 2)
      {
        throw_error(MQTT_INVALID_PUBCOMP_PACKET);
      }

      // - release packet id -
      usi_queue_s_insert(&this->packet_ids,this->packet_id);

      // - reset publish data -
      mqtt_publish_s_clear(publish);
      publish->packet_id = 0;

      // - call callback -
      if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_PUBLISHED))
      {
        throw_error(MQTT_CONN_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
    case MQTT_PACKET_TYPE_SUBSCRIBE:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    case MQTT_PACKET_TYPE_SUBACK:
    {/*{{{*/
      if ((pkt_type & 0x0f) != 0 || // reserved
          size < 2 + 1 + 1)         // minimal size
      {
        throw_error(MQTT_INVALID_SUBACK_PACKET);
      }

      // - packet identifier -
      this->packet_id = mqtt_two_byte_dec(data);
      data += 2;

      mqtt_subscribe_s *subscribe;

      // - unknown packet id -
      if (this->packet_id >= this->subscribed.used
          || (subscribe = mqtt_subscribe_array_s_at(&this->subscribed,this->packet_id))->packet_id != this->packet_id)
      {
        throw_error(MQTT_INVALID_SUBACK_PACKET);
      }

      // - properties -
      uint32_t props_length;
      if (mqtt_var_byte_dec(data,data_end,&data,&props_length)
          || (uint32_t)(data_end - data) < props_length)
      {
        throw_error(MQTT_INVALID_SUBACK_PACKET);
      }

      if (mqtt_conn_s_process_properties(this,pkt_type,props_length,data))
      {
        throw_error(MQTT_CONN_PROCESS_PROPERTIES_ERROR);
      }

      data += props_length;

      // - check reason code count -
      if ((uint32_t)(data_end - data) != subscribe->filters.used)
      {
        throw_error(MQTT_INVALID_SUBACK_PACKET);
      }

      // - check reason codes -
      do {
        if (*data != subscribe->max_qos)
        {
          throw_error(MQTT_INVALID_SUBACK_PACKET);
        }
      } while (++data < data_end);

      usi_queue_s_insert(&this->packet_ids,this->packet_id);

      // - reset subscribe data -
      mqtt_subscribe_s_clear(subscribe);
      subscribe->packet_id = 0;

      // - call callback -
      if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_SUBSCRIBED))
      {
        throw_error(MQTT_CONN_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
    case MQTT_PACKET_TYPE_UNSUBSCRIBE:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    case MQTT_PACKET_TYPE_UNSUBACK:
    {/*{{{*/
      if ((pkt_type & 0x0f) != 0 ||  // reserved
          size < 2 + 1 + 1)          // minimal size
      {
        throw_error(MQTT_INVALID_UNSUBACK_PACKET);
      }

      // - packet identifier -
      this->packet_id = mqtt_two_byte_dec(data);
      data += 2;

      mqtt_subscribe_s *unsubscribe;

      // - unknown packet id -
      if (this->packet_id >= this->subscribed.used
          || (unsubscribe = mqtt_subscribe_array_s_at(&this->subscribed,this->packet_id))->packet_id != this->packet_id)
      {
        throw_error(MQTT_INVALID_UNSUBACK_PACKET);
      }

      // - properties -
      uint32_t props_length;
      if (mqtt_var_byte_dec(data,data_end,&data,&props_length)
          || (uint32_t)(data_end - data) < props_length)
      {
        throw_error(MQTT_INVALID_UNSUBACK_PACKET);
      }

      if (mqtt_conn_s_process_properties(this,pkt_type,props_length,data))
      {
        throw_error(MQTT_CONN_PROCESS_PROPERTIES_ERROR);
      }

      data += props_length;

      // - check reason code count -
      if ((uint32_t)(data_end - data) != unsubscribe->filters.used)
      {
        throw_error(MQTT_INVALID_UNSUBACK_PACKET);
      }

      // - check reason codes -
      do {
        if ((uint8_t)(*data) >= 0x80)
        {
          throw_error(MQTT_INVALID_UNSUBACK_PACKET);
        }
      } while (++data < data_end);

      // - release packet id -
      usi_queue_s_insert(&this->packet_ids,this->packet_id);

      // - reset subscribe data -
      mqtt_subscribe_s_clear(unsubscribe);
      unsubscribe->packet_id = 0;

      // - call callback -
      if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_UNSUBSCRIBED))
      {
        throw_error(MQTT_CONN_CALLBACK_ERROR);
      }
    }/*}}}*/
    break;
    case MQTT_PACKET_TYPE_PINGREQ:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    case MQTT_PACKET_TYPE_PINGRESP:
    {/*{{{*/
      if (this->ping_mode == MQTT_PING_MODE_CONTINUOUS)
      {
        // - update response time -
        if (clock_s_gettime(CLOCK_MONOTONIC,&this->resp_time))
        {
          throw_error(MQTT_CONN_GET_TIME_ERROR);
        }
      }
    }/*}}}*/
    break;
    case MQTT_PACKET_TYPE_DISCONNECT:
    case MQTT_PACKET_TYPE_AUTH:
      throw_error(MQTT_UNSUPPORTED_CONTROL_PACKET_TYPE);
    default:
      throw_error(MQTT_UNKNOWN_CONTROL_PACKET_TYPE);
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_conn_recv(void *a_mqtt_conn,unsigned a_index,bc_array_s *a_message)
{/*{{{*/
  (void)a_index;

  uint32_t packet_off = 0;

  do {
    const char *packet = a_message->data + packet_off;
    uint32_t buff_size = a_message->used - packet_off;

    const char *remain;
    uint32_t remain_size;
    uint32_t packet_size;

    if (
      // - variable byte integer is complete -
      mqtt_var_byte_dec(packet + 1,packet + buff_size,&remain,&remain_size) == 0 &&

      // - whole control packet was received -
      buff_size >= (packet_size = remain_size + (remain - packet)))
    {
      if (mqtt_conn_s_process_packet(a_mqtt_conn,packet[0],remain_size,remain))
      {
        throw_error(MQTT_CONN_PROCESS_PACKET_ERROR);
      }

      // - update packet offset -
      packet_off += packet_size;
    }
    else
    {
      // - remove processed packets from receive buffer -
      bc_array_s_tail(a_message,a_message->used - packet_off);

      break;
    }
  } while (1);

  return 0;
}/*}}}*/

int mqtt_conn_s_fd_event(void *a_mqtt_conn,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  mqtt_conn_s *this = (mqtt_conn_s *)a_mqtt_conn;

  // - store connecting state -
  int connecting = this->connection.connecting;

  if (tcp_conn_s_fd_event(&this->connection,0,a_epoll_event))
  {
    tcp_conn_s_clear(&this->connection);

    // - mqtt client was connected -
    if (this->connected)
    {
      if (this->disconnecting)
      {
        // - reset connected flag -
        this->connected = 0;

        // - reset disconnecting flag -
        this->disconnecting = 0;

        // - call callback -
        if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_DISCONNECTED))
        {
          throw_error(MQTT_CONN_CALLBACK_ERROR);
        }
      }
      else
      {
        // - reset connected flag -
        this->connected = 0;

        // - reset disconnecting flag -
        this->disconnecting = 0;

        // - call callback -
        if (mqtt_conn_s_callback(this,cl_mqtt_cbreq_DROPPED))
        {
          throw_error(MQTT_CONN_CALLBACK_ERROR);
        }

        // - register epoll timer -
        struct itimerspec itimerspec = MQTT_TCP_RETRY_INTERVAL;
        if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,mqtt_conn_s_check_time_event,this,0))
        {
          throw_error(MQTT_CONN_EPOLL_ERROR);
        }
      }
    }
    else
    {
      // - register epoll timer -
      struct itimerspec itimerspec = MQTT_TCP_RETRY_INTERVAL;
      if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,mqtt_conn_s_check_time_event,this,0))
      {
        throw_error(MQTT_CONN_EPOLL_ERROR);
      }
    }
  }
  else
  {
    if (connecting)
    {
      // - send connect packet -
      uint32_t remaining_length = 10 + // variable header
        this->connect_props.used +     // properties
        2 + this->client_id.size - 1;  // client id

      if (mqtt_var_byte_len(this->connect_props.used,&remaining_length))
      {
        throw_error(MQTT_INVALID_CONNECT_PACKET);
      }

      // - will will be registered by connection -
      if (this->will.topic.size > 1)
      {
        remaining_length += this->will.props.used + // will properties
          2 + this->will.topic.size - 1 +           // will topic length
          2 + this->will.payload.used;              // will payload length

        if (mqtt_var_byte_len(this->will.props.used,&remaining_length))
        {
          throw_error(MQTT_INVALID_CONNECT_PACKET);
        }
      }

      // - user name will be send -
      if (this->user_name.size > 1)
      {
        remaining_length += 2 + this->user_name.size - 1;
      }

      // - password will be send -
      if (this->password.size > 1)
      {
        remaining_length += 2 + this->password.size - 1;
      }

      uint32_t buffer_size = 1 + remaining_length;

      if (mqtt_var_byte_len(remaining_length,&buffer_size))
      {
        throw_error(MQTT_INVALID_CONNECT_PACKET);
      }

      CONT_INIT_CLEAR(bc_array_s,buffer);
      bc_array_s_reserve(&buffer,buffer_size);

      bc_array_s_push(&buffer,0x10);  // CONNECT
      if (mqtt_var_byte_enc(remaining_length,&buffer))
      {
        throw_error(MQTT_INVALID_CONNECT_PACKET);
      }

      static const char HEADER[] = "\x00\x04MQTT\x05";
      bc_array_s_append(&buffer,7,HEADER);

      // - connect flags -
      bc_array_s_push(&buffer,
          (this->user_name.size > 1) << 7 |  // user name flag
          (this->password.size > 1) << 6 |   // password flag
          (this->will.topic.size > 1 && this->will.retain) << 5 |    // will retain
          (this->will.topic.size > 1 ? this->will.qos : 0x00) << 3 | // will qos (2 bits)
          (this->will.topic.size > 1) << 2 | // will flag
          1 << 1 |                           // clean start
          0 << 0);                           // reserved

      // - keep alive -
      mqtt_two_byte_enc(this->ping_timeout / 1000000000ULL,&buffer);

      // - properties length -
      if (mqtt_var_byte_enc(this->connect_props.used,&buffer))
      {
        throw_error(MQTT_INVALID_CONNECT_PACKET);
      }

      // - properties -
      bc_array_s_append(&buffer,this->connect_props.used,this->connect_props.data);

      // - client id -
      mqtt_two_byte_enc(this->client_id.size - 1,&buffer);
      bc_array_s_append(&buffer,this->client_id.size - 1,this->client_id.data);

      if (this->will.topic.size > 1)
      {
        // - will properties length -
        if (mqtt_var_byte_enc(this->will.props.used,&buffer))
        {
          throw_error(MQTT_INVALID_CONNECT_PACKET);
        }

        bc_array_s_append(&buffer,this->will.props.used,this->will.props.data);

        // - will topic -
        mqtt_two_byte_enc(this->will.topic.size - 1,&buffer);
        bc_array_s_append(&buffer,this->will.topic.size - 1,this->will.topic.data);

        // - will payload -
        mqtt_two_byte_enc(this->will.payload.used,&buffer);
        bc_array_s_append(&buffer,this->will.payload.used,this->will.payload.data);
      }

      // - user name -
      if (this->user_name.size > 1)
      {
        mqtt_two_byte_enc(this->user_name.size - 1,&buffer);
        bc_array_s_append(&buffer,this->user_name.size - 1,this->user_name.data);
      }

      // - password -
      if (this->password.size > 1)
      {
        mqtt_two_byte_enc(this->password.size - 1,&buffer);
        bc_array_s_append(&buffer,this->password.size - 1,this->password.data);
      }

      cassert(buffer.used == buffer_size);

      if (mqtt_conn_s_send_message(this,&buffer))
      {
        throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
      }

      // - register epoll timer -
      struct itimerspec itimerspec = MQTT_TCP_CHECK_INTERVAL;
      if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,mqtt_conn_s_check_time_event,this,0))
      {
        throw_error(MQTT_CONN_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_check_time_event(void *a_mqtt_conn,unsigned a_index,epoll_event_s *a_epoll_event)
{/*{{{*/
  (void)a_index;

  // - read timer expiration counter -
  uint64_t timer_exps;
  if (read(a_epoll_event->data.fd,&timer_exps,sizeof(timer_exps)) != sizeof(timer_exps))
  {
    throw_error(MQTT_CONN_TIMER_READ_ERROR);
  }

  mqtt_conn_s *this = (mqtt_conn_s *)a_mqtt_conn;

  if (this->connected)
  {
    if (!this->disconnecting && this->ping_mode != MQTT_PING_MODE_OFF)
    {
      // - retrieve time -
      time_s now;
      if (clock_s_gettime(CLOCK_MONOTONIC,&now))
      {
        throw_error(MQTT_CONN_GET_TIME_ERROR);
      }

      uint64_t time_diff = now >= this->resp_time ? now - this->resp_time : this->resp_time - now;

      // - if ping response not received -
      if (time_diff >= this->ping_timeout)
      {
        tcp_conn_s_clear(&this->connection);

        // - reset connected flag -
        this->connected = 0;

        // - reset disconnecting flag -
        this->disconnecting = 0;

        // - register epoll timer -
        struct itimerspec itimerspec = MQTT_TCP_RETRY_INTERVAL;
        if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,mqtt_conn_s_check_time_event,this,0))
        {
          throw_error(MQTT_CONN_EPOLL_ERROR);
        }
      }
      else
      {
        // - ping period was reached -
        if (time_diff >= this->ping_period && this->ping_time != this->resp_time)
        {
          // - send pingreq packet -
          CONT_INIT_CLEAR(bc_array_s,buffer);
          bc_array_s_reserve(&buffer,2);

          bc_array_s_push(&buffer,0xc0); // PINGREQ
          bc_array_s_push(&buffer,0x00); // remaining length

          cassert(buffer.used == 2);

          if (mqtt_conn_s_send_message(this,&buffer))
          {
            throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
          }

          // - update ping time -
          this->ping_time = this->resp_time;
        }
      }
    }
  }
  else
  {
    if (tcp_conn_s_create_client(&this->connection,
          this->server_ip.data,this->server_port,mqtt_conn_s_conn_recv,NULL,this,0) ||
        epoll_s_fd_callback(&this->connection.epoll_fd,EPOLLIN | EPOLLOUT | EPOLLPRI,mqtt_conn_s_fd_event,this,0))
    {
      // - register epoll timer -
      struct itimerspec itimerspec = MQTT_TCP_RETRY_INTERVAL;
      if (epoll_s_timer_callback(&this->epoll_timer,&itimerspec,0,mqtt_conn_s_check_time_event,this,0))
      {
        throw_error(MQTT_CONN_EPOLL_ERROR);
      }
    }
  }

  return 0;
}/*}}}*/

int mqtt_var_byte_enc(uint32_t a_value,bc_array_s *a_trg)
{/*{{{*/
  if (a_value < MQTT_VAR_BYTE_INT_MAX_TWO)
  {
    if (a_value < MQTT_VAR_BYTE_INT_MAX_ONE)
    {
      bc_array_s_push(a_trg,a_value);
    }
    else
    {
      bc_array_s_push(a_trg,0x80 | (a_value & 0x7f));
      bc_array_s_push(a_trg,a_value >> 7);
    }
  }
  else
  {
    if (a_value < MQTT_VAR_BYTE_INT_MAX_THREE)
    {
      bc_array_s_push(a_trg,0x80 | (a_value & 0x7f));
      bc_array_s_push(a_trg,0x80 | ((a_value >>= 7) & 0x7f));
      bc_array_s_push(a_trg,a_value >> 7);
    }
    else
    {
      if (a_value >= MQTT_VAR_BYTE_INT_MAX_FOUR)
      {
        throw_error(MQTT_INVALID_VARIABLE_BYTE_INTEGER);
      }

      bc_array_s_push(a_trg,0x80 | (a_value & 0x7f));
      bc_array_s_push(a_trg,0x80 | ((a_value >>= 7) & 0x7f));
      bc_array_s_push(a_trg,0x80 | ((a_value >>= 7) & 0x7f));
      bc_array_s_push(a_trg,a_value >> 7);
    }
  }

  return 0;
}/*}}}*/

int mqtt_var_byte_dec(
  const char *a_src,const char *a_src_end,const char **a_end,uint32_t *a_trg)
{/*{{{*/

  // - check buffer size -
  if (a_src >= a_src_end)
  {
    return 1;
  }

  uint32_t value = 0;
  uint32_t shift = 0;

  do {
    value += (*a_src & 0x7f) << shift;

    // - check buffer size and variable byte integer length -
    if (a_src >= a_src_end || (shift += 7) > 28)
    {
      return 1;
    }
  } while ((*a_src++ & 0x80) != 0);

  *a_end = a_src;
  *a_trg = value;

  return 0;
}/*}}}*/

int mqtt_conn_s_next_packet_id(mqtt_conn_s *this,uint16_t *a_packet_id)
{/*{{{*/
  if (this->packet_ids.used == 0)
  {
    if (this->next_packet_id >= UINT16_MAX)
    {
      throw_error(MQTT_CONN_OUT_OF_PACKET_IDENTIFIERS);
    }

    *a_packet_id = this->next_packet_id++;
  }
  else {
    *a_packet_id = usi_queue_s_next(&this->packet_ids);
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_send_publish(mqtt_conn_s *this,mqtt_publish_s *a_publish,int a_dup)
{/*{{{*/
  uint32_t remaining_length = 2 + a_publish->topic.size - 1 + // topic length
    (a_publish->packet_id != 0 ? 2 : 0) +  // packet identifier length
    a_publish->props.used + // properties length
    a_publish->payload.used; // payload length

  if (mqtt_var_byte_len(a_publish->props.used,&remaining_length))
  {
    throw_error(MQTT_INVALID_PUBLISH_PACKET);
  }

  uint32_t buffer_size = 1 + remaining_length;

  if (mqtt_var_byte_len(remaining_length,&buffer_size))
  {
    throw_error(MQTT_INVALID_PUBLISH_PACKET);
  }

  CONT_INIT_CLEAR(bc_array_s,buffer);
  bc_array_s_reserve(&buffer,buffer_size);

  bc_array_s_push(&buffer,0x30 |    // PUBLISH
      (a_dup ? 8 : 0) |             // dup
      a_publish->qos << 1 |         // qos level
      (a_publish->retain ? 1 : 0)); // retain

  if (mqtt_var_byte_enc(remaining_length,&buffer))
  {
    throw_error(MQTT_INVALID_PUBLISH_PACKET);
  }

  // - topic name -
  mqtt_two_byte_enc(a_publish->topic.size - 1,&buffer);
  bc_array_s_append(&buffer,a_publish->topic.size - 1,a_publish->topic.data);

  if (a_publish->packet_id != 0) {
    // - packet identifier -
    mqtt_two_byte_enc(a_publish->packet_id,&buffer);
  }

  // - properties length -
  if (mqtt_var_byte_enc(a_publish->props.used,&buffer))
  {
    throw_error(MQTT_INVALID_PUBLISH_PACKET);
  }

  // - properties -
  bc_array_s_append(&buffer,a_publish->props.used,a_publish->props.data);

  // - payload -
  bc_array_s_append(&buffer,a_publish->payload.used,a_publish->payload.data);

  cassert(buffer.used == buffer_size);

  if (mqtt_conn_s_send_message(this,&buffer))
  {
    throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_send_pubrel(mqtt_conn_s *this,uint16_t a_packet_id,uint8_t a_reason_code)
{/*{{{*/
  uint32_t remaining_length = 2 + (a_reason_code != 0x00 ? 1 : 0);

  CONT_INIT_CLEAR(bc_array_s,buffer);
  bc_array_s_reserve(&buffer,2 + remaining_length);

  bc_array_s_push(&buffer,0x62); // PUBREL
  bc_array_s_push(&buffer,remaining_length);

  // - packet identifier -
  mqtt_two_byte_enc(a_packet_id,&buffer);

  // - reason code -
  if (a_reason_code != 0x00)
  {
    bc_array_s_push(&buffer,a_reason_code);
  }

  cassert(buffer.used == 2 + remaining_length);

  if (mqtt_conn_s_send_message(this,&buffer))
  {
    throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_send_subscribe(mqtt_conn_s *this,mqtt_subscribe_s *a_subscribe)
{/*{{{*/
  uint32_t remaining_length = 2 + a_subscribe->props.used;

  if (mqtt_var_byte_len(a_subscribe->props.used,&remaining_length))
  {
    throw_error(MQTT_INVALID_SUBSCRIBE_PACKET);
  }

  // - retrieve topic filters length -
  if (a_subscribe->filters.used != 0)
  {
    string_s *f_ptr = a_subscribe->filters.data;
    string_s *f_ptr_end = f_ptr + a_subscribe->filters.used;
    do {
      remaining_length += 2 + 1 + f_ptr->size - 1;
    } while(++f_ptr < f_ptr_end);
  }

  uint32_t buffer_size = 1 + remaining_length;

  if (mqtt_var_byte_len(remaining_length,&buffer_size))
  {
    throw_error(MQTT_INVALID_SUBSCRIBE_PACKET);
  }

  CONT_INIT_CLEAR(bc_array_s,buffer);
  bc_array_s_reserve(&buffer,buffer_size);

  bc_array_s_push(&buffer,0x82); // SUBSCRIBE
  if (mqtt_var_byte_enc(remaining_length,&buffer))
  {
    throw_error(MQTT_INVALID_SUBSCRIBE_PACKET);
  }

  // - packet identifier -
  mqtt_two_byte_enc(a_subscribe->packet_id,&buffer);

  // - properties length -
  if (mqtt_var_byte_enc(a_subscribe->props.used,&buffer))
  {
    throw_error(MQTT_INVALID_SUBSCRIBE_PACKET);
  }

  // - properties -
  bc_array_s_append(&buffer,a_subscribe->props.used,a_subscribe->props.data);

  // - process topic filters -
  if (a_subscribe->filters.used != 0)
  {
    string_s *f_ptr = a_subscribe->filters.data;
    string_s *f_ptr_end = f_ptr + a_subscribe->filters.used;
    do {
      // - topic filter -
      mqtt_two_byte_enc(f_ptr->size - 1,&buffer);
      bc_array_s_append(&buffer,f_ptr->size - 1,f_ptr->data);

      // - subscription options -
      bc_array_s_push(&buffer,
          0 << 4 |  // retain handling (send retained messages at the time of the subscribe)
          1 << 3 |  // retain as published
          0 << 2 |  // no local (off)
          a_subscribe->max_qos);
    } while(++f_ptr < f_ptr_end);
  }

  cassert(buffer.used == buffer_size);

  if (mqtt_conn_s_send_message(this,&buffer))
  {
    throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_send_unsubscribe(mqtt_conn_s *this,mqtt_subscribe_s *a_subscribe)
{/*{{{*/
  uint32_t remaining_length = 2 + a_subscribe->props.used;

  if (mqtt_var_byte_len(a_subscribe->props.used,&remaining_length))
  {
    throw_error(MQTT_INVALID_UNSUBSCRIBE_PACKET);
  }

  // Retrieve topic filters length
  if (a_subscribe->filters.used != 0)
  {
    string_s *f_ptr = a_subscribe->filters.data;
    string_s *f_ptr_end = f_ptr + a_subscribe->filters.used;
    do {
      remaining_length += 2 + f_ptr->size - 1;
    } while(++f_ptr < f_ptr_end);
  }

  uint32_t buffer_size = 1 + remaining_length;

  if (mqtt_var_byte_len(remaining_length,&buffer_size))
  {
    throw_error(MQTT_INVALID_UNSUBSCRIBE_PACKET);
  }

  CONT_INIT_CLEAR(bc_array_s,buffer);
  bc_array_s_reserve(&buffer,buffer_size);

  bc_array_s_push(&buffer,0xa2); // UNSUBSCRIBE

  if (mqtt_var_byte_enc(remaining_length,&buffer))
  {
    throw_error(MQTT_INVALID_UNSUBSCRIBE_PACKET);
  }

  // Packet identifier
  mqtt_two_byte_enc(a_subscribe->packet_id,&buffer);

  // Properties length
  if (mqtt_var_byte_enc(a_subscribe->props.used,&buffer))
  {
    throw_error(MQTT_INVALID_UNSUBSCRIBE_PACKET);
  }

  // Properties
  bc_array_s_append(&buffer,a_subscribe->props.used,a_subscribe->props.data);

  // Process topic filters
  if (a_subscribe->filters.used != 0)
  {
    string_s *f_ptr = a_subscribe->filters.data;
    string_s *f_ptr_end = f_ptr + a_subscribe->filters.used;
    do {
      // Topic filter
      mqtt_two_byte_enc(f_ptr->size - 1,&buffer);
      bc_array_s_append(&buffer,f_ptr->size - 1,f_ptr->data);
    } while(++f_ptr < f_ptr_end);
  }

  cassert(buffer.used == buffer_size);

  if (mqtt_conn_s_send_message(this,&buffer))
  {
    throw_error(MQTT_CONN_SEND_MESSAGE_ERROR);
  }

  return 0;
}/*}}}*/

int mqtt_conn_s_publish(mqtt_conn_s *this,
    string_s *a_topic,bc_array_s *a_payload,bc_array_s *a_props,
    uint8_t a_qos,int a_retain,uint16_t *a_packet_id)
{/*{{{*/
  if (this->disconnecting || a_topic->size - 1 > UINT16_MAX || a_qos >= 3)
  {
    throw_error(MQTT_INVALID_PUBLISH_REQUEST);
  }

  uint16_t packet_id = 0;

  if (a_qos > 0)
  {
    if (mqtt_conn_s_next_packet_id(this,&packet_id))
    {
      throw_error(MQTT_CONN_NEXT_PACKET_ID_ERROR);
    }

    while (packet_id >= this->published.used)
    {
      mqtt_publish_array_s_push_blank(&this->published);
      mqtt_publish_array_s_last(&this->published)->packet_id = 0;
    }

    mqtt_publish_s *publish = mqtt_publish_array_s_at(&this->published,packet_id);

    publish->packet_id = packet_id;
    publish->released = 0;
    string_s_swap(&publish->topic,a_topic);
    bc_array_s_swap(&publish->payload,a_payload);
    publish->qos = a_qos;
    publish->retain = a_retain;
    bc_array_s_swap(&publish->props,a_props);

    if (this->connected)
    {
      if (mqtt_conn_s_send_publish(this,mqtt_publish_array_s_at(&this->published,packet_id),0))
      {
        throw_error(MQTT_SEND_PUBLISH_ERROR);
      }
    }
  }
  else
  {
    if (this->connected)
    {
      CONT_INIT_CLEAR(mqtt_publish_s,publish);

      publish.packet_id = packet_id;
      publish.released = 0;
      string_s_swap(&publish.topic,a_topic);
      bc_array_s_swap(&publish.payload,a_payload);
      publish.qos = a_qos;
      publish.retain = a_retain;
      bc_array_s_swap(&publish.props,a_props);

      if (mqtt_conn_s_send_publish(this,&publish,0))
      {
        throw_error(MQTT_SEND_PUBLISH_ERROR);
      }
    }
  }

  *a_packet_id = packet_id;

  return 0;
}/*}}}*/

int mqtt_conn_s_subscribe(mqtt_conn_s *this,
    string_array_s *a_filters,bc_array_s *a_props,
    uint8_t a_max_qos,uint16_t *a_packet_id)
{/*{{{*/
  if (this->disconnecting || a_filters->used == 0 || a_max_qos >= 3)
  {
    throw_error(MQTT_INVALID_SUBSCRIBE_REQUEST);
  }

  uint16_t packet_id;
  if (mqtt_conn_s_next_packet_id(this,&packet_id))
  {
    throw_error(MQTT_CONN_NEXT_PACKET_ID_ERROR);
  }

  while (packet_id >= this->subscribed.used)
  {
    mqtt_subscribe_array_s_push_blank(&this->subscribed);
    mqtt_subscribe_array_s_last(&this->subscribed)->packet_id = 0;
  }

  mqtt_subscribe_s *subscribe = mqtt_subscribe_array_s_at(&this->subscribed,packet_id);
  subscribe->packet_id = packet_id;
  string_array_s_swap(&subscribe->filters,a_filters);
  subscribe->max_qos = a_max_qos;
  bc_array_s_swap(&subscribe->props,a_props);

  if (mqtt_conn_s_send_subscribe(this,subscribe))
  {
    throw_error(MQTT_SEND_SUBSCRIBE_ERROR);
  }

  *a_packet_id = packet_id;

  return 0;
}/*}}}*/

int mqtt_conn_s_unsubscribe(mqtt_conn_s *this,
    string_array_s *a_filters,bc_array_s *a_props,
    uint16_t *a_packet_id)
{/*{{{*/
  if (this->disconnecting || a_filters->used == 0)
  {
    throw_error(MQTT_INVALID_UNSUBSCRIBE_REQUEST);
  }

  uint16_t packet_id;
  if (mqtt_conn_s_next_packet_id(this,&packet_id))
  {
    throw_error(MQTT_CONN_NEXT_PACKET_ID_ERROR);
  }

  while (packet_id >= this->subscribed.used)
  {
    mqtt_subscribe_array_s_push_blank(&this->subscribed);
    mqtt_subscribe_array_s_last(&this->subscribed)->packet_id = 0;
  }

  mqtt_subscribe_s *subscribe = mqtt_subscribe_array_s_at(&this->subscribed,packet_id);
  subscribe->packet_id = packet_id;
  string_array_s_swap(&subscribe->filters,a_filters);
  subscribe->max_qos = 0;
  bc_array_s_swap(&subscribe->props,a_props);

  if (mqtt_conn_s_send_unsubscribe(this,subscribe))
  {
    throw_error(MQTT_SEND_UNSUBSCRIBE_ERROR);
  }

  *a_packet_id = packet_id;

  return 0;
}/*}}}*/

