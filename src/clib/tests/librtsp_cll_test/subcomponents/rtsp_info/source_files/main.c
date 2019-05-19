
@begin
include "main.h"
@end

volatile int g_terminate = 0;

// === methods of generated structures =========================================

// -- rtsp_info_s --
@begin
methods rtsp_info_s
@end

int rtsp_info_s_create(rtsp_info_s *this,const char *a_file_name)
{/*{{{*/
  if (file_s_open(&this->file,a_file_name,"rb"))
  {
    throw_error(RTSP_INFO_FILE_OPEN_ERROR);
  }

  return 0;
}/*}}}*/

int rtsp_info_s_print_packet(rtsp_info_s *this)
{/*{{{*/
  this->buffer.used = 0;

  // - read packet header -
  if (stream_s_read_cnt(&this->file,&this->buffer,sizeof(rtsp_pkt_delay_t) + 4))
  {
    throw_error(RTSP_INFO_FILE_READ_ERROR);
  }

  usi pkt_time    = *((usi *)this->buffer.data);
  bc  pkt_char    = this->buffer.data[sizeof(rtsp_pkt_delay_t)];
  uc  pkt_channel = this->buffer.data[sizeof(rtsp_pkt_delay_t) + 1];
  usi pkt_size    = ntohs(*((usi *)(this->buffer.data + sizeof(rtsp_pkt_delay_t) + 2)));

  // - read rest of packet -
  if (stream_s_read_cnt(&this->file,&this->buffer,pkt_size))
  {
    throw_error(RTSP_INFO_FILE_READ_ERROR);
  }

  // - print packet info -
  fprintf(stdout,"pkt_info: %hu, %c, %hhu, %hu\n",pkt_time,pkt_char,pkt_channel,pkt_size);

  //if (
  //  //pkt_channel == 0
  //  //pkt_channel == 2
  //  1
  //  )
  //{
  //  char *rtp_data = this->buffer.data + sizeof(rtsp_pkt_delay_t) + 4;

  //  uc  rtp_M  = (rtp_data[1] & 0x80) >> 7;
  //  uc  rtp_PT = (rtp_data[1] & 0x7f);
  //  usi rtp_SN = ntohs(*((usi *)(rtp_data + 2)));
  //  ui  rtp_TS = ntohl(*((ui *)(rtp_data + 4)));

  //  // - print rtp info -
  //  fprintf(stdout,"rtp_info: M: %hhu, PT: %hhu, SN: %hu, TS: %u\n",rtp_M,rtp_PT,rtp_SN,rtp_TS);

  //  //char h264_byte = rtp_data[12];
  //  //uc h264_F    = (h264_byte & 0x80) >> 7;
  //  //uc h264_NRI  = (h264_byte & 0x60) >> 5;
  //  //uc h264_Type = (h264_byte & 0x1f);

  //  //// - print h264 info -
  //  //fprintf(stdout,"h264_info: F: %hhu, NRI: %hhu, Type: %hhu\n",h264_F,h264_NRI,h264_Type);
  //}

  return 0;
}/*}}}*/

// === global functions ========================================================

void signal_handler(int a_signum)
{/*{{{*/
  (void)a_signum;

  // - terminate on all signals -
  __sync_add_and_fetch(&g_terminate,1);
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  memcheck_init();

  cassert(signal_s_simple_handler(signal_handler) == 0);

  CONT_INIT(rtsp_info_s,info);
  cassert(rtsp_info_s_create(&info,"../recordings/10.2.1.18.stream") == 0);
  //cassert(rtsp_info_s_create(&info,"../recordings/10.2.35.6.stream") == 0);

  do {
    if (rtsp_info_s_print_packet(&info))
    {
      break;
    }
  } while(1);

  rtsp_info_s_clear(&info);

  memcheck_release_assert();

  return 0;
}/*}}}*/

