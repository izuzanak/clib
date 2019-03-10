
#ifndef __CL_RTSP_SDP_PARSER_H
#define __CL_RTSP_SDP_PARSER_H

@begin
include "cl_linux.h"
@end

// - error codes -
#define ERROR_RTSP_SDP_PARSE_UNRECOGNIZED_TERMINAL 1
#define ERROR_RTSP_SDP_PARSE_INVALID_SYNTAX 2

// === definition of generated structures ======================================

enum
{/*{{{*/
  rtsp_sdp_media_type_UNKNOW = 0,
  rtsp_sdp_media_type_VIDEO,
  rtsp_sdp_media_type_AUDIO,
};/*}}}*/

// -- rtsp_sdp_parser_s --
@begin
struct
<
string_s:source_string
ui:input_idx
ui:old_input_idx

ui:media_type
string_s:video_control
string_s:audio_control

lalr_stack_s:lalr_stack
>
rtsp_sdp_parser_s;
@end

unsigned rtsp_sdp_parser_s_recognize_terminal(rtsp_sdp_parser_s *this);
WUR int rtsp_sdp_parser_s_parse(rtsp_sdp_parser_s *this,string_s *a_src);

// === rtsp parse actions access ==========================================

extern void (*rtsp_sdp_pa_callers[])(rtsp_sdp_parser_s *this);

// === rtsp parse actions =================================================

void pa_rtsp_sdp_null(rtsp_sdp_parser_s *this);
void pa_rtsp_sdp_video(rtsp_sdp_parser_s *this);
void pa_rtsp_sdp_audio(rtsp_sdp_parser_s *this);
void pa_rtsp_sdp_control(rtsp_sdp_parser_s *this);

// === inline methods of generated structures ==================================

// -- rtsp_sdp_parser_s --
@begin
inlines rtsp_sdp_parser_s
@end

#endif

