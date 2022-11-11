
#ifndef CL_LIB_RTSP_PARSER_H
#define CL_LIB_RTSP_PARSER_H

@begin
include "cl_linux.h"
@end

// - error codes -
#define ERROR_RTSP_DIGEST_AUTHENTICATE_ERROR 1

#define ERROR_RTSP_RESP_PARSE_UNRECOGNIZED_TERMINAL 1
#define ERROR_RTSP_RESP_PARSE_INVALID_SYNTAX 2

// === definition of generated structures ======================================

// -- rtsp_digest_s --
@begin
struct
<
string_s:method
string_s:username
string_s:realm
string_s:nonce
string_s:uri
string_s:response
>
rtsp_digest_s;
@end

WUR int rtsp_digest_s_authenticate(rtsp_digest_s *this,
    const string_s *a_pass,bi a_verify);

// -- rtsp_parser_s --
@begin
struct
<
string_s:source_string
ui:input_idx
ui:old_input_idx

ui:command
ui:cseq
ui:public_commands
pointer:session
ui:session_length
pointer:x_session
ui:x_session_length
ui:timeout
bi:ping
string_s:range
ui:datacast
bi:tcp
bi:onvif_replay
pointer:rtsp_url
ui:inter_port_begin
ui:inter_port_end
rtsp_digest_s:digest
bi:digest_authorization
bi:digest_authenticate

lalr_stack_s:lalr_stack
>
rtsp_parser_s;
@end

unsigned rtsp_parser_s_recognize_terminal(rtsp_parser_s *this);
WUR int rtsp_parser_s_parse(rtsp_parser_s *this,string_s *a_src,int a_no_check);

// === rtsp parse actions access ===============================================

extern void (*rtsp_pa_callers[])(rtsp_parser_s *this);

// === rtsp parse actions ======================================================

void pa_rtsp_null(rtsp_parser_s *this);
void pa_rtsp_command(rtsp_parser_s *this);
void pa_rtsp_response(rtsp_parser_s *this);
void pa_rtsp_header_cseq(rtsp_parser_s *this);
void pa_rtsp_header_ct_base(rtsp_parser_s *this);
void pa_rtsp_header_ct_type(rtsp_parser_s *this);
void pa_rtsp_header_ct_length(rtsp_parser_s *this);
void pa_rtsp_header_transport_tcp(rtsp_parser_s *this);
void pa_rtsp_header_transport_udp(rtsp_parser_s *this);
void pa_rtsp_header_ping(rtsp_parser_s *this);
void pa_rtsp_header_range(rtsp_parser_s *this);
void pa_rtsp_header_require(rtsp_parser_s *this);
void pa_rtsp_header_xscookie(rtsp_parser_s *this);
void pa_rtsp_header_digest_authorization(rtsp_parser_s *this);
void pa_rtsp_header_digest_authenticate(rtsp_parser_s *this);
void pa_rtsp_header_ignore(rtsp_parser_s *this);
void pa_rtsp_header_accept(rtsp_parser_s *this);
void pa_rtsp_pubcmd_get(rtsp_parser_s *this);
void pa_rtsp_pubcmd_post(rtsp_parser_s *this);
void pa_rtsp_pubcmd_options(rtsp_parser_s *this);
void pa_rtsp_pubcmd_describe(rtsp_parser_s *this);
void pa_rtsp_pubcmd_announce(rtsp_parser_s *this);
void pa_rtsp_pubcmd_record(rtsp_parser_s *this);
void pa_rtsp_pubcmd_setup(rtsp_parser_s *this);
void pa_rtsp_pubcmd_teardown(rtsp_parser_s *this);
void pa_rtsp_pubcmd_play(rtsp_parser_s *this);
void pa_rtsp_pubcmd_pause(rtsp_parser_s *this);
void pa_rtsp_pubcmd_set_parameter(rtsp_parser_s *this);
void pa_rtsp_pubcmd_get_parameter(rtsp_parser_s *this);
void pa_rtsp_header_session(rtsp_parser_s *this);
void pa_rtsp_key_timeout(rtsp_parser_s *this);
void pa_rtsp_key_npt(rtsp_parser_s *this);
void pa_rtsp_key_datacast(rtsp_parser_s *this);
void pa_rtsp_key_inter_port(rtsp_parser_s *this);
void pa_rtsp_auth_username(rtsp_parser_s *this);
void pa_rtsp_auth_realm(rtsp_parser_s *this);
void pa_rtsp_auth_nonce(rtsp_parser_s *this);
void pa_rtsp_auth_uri(rtsp_parser_s *this);
void pa_rtsp_auth_response(rtsp_parser_s *this);
void pa_rtsp_auth_stale(rtsp_parser_s *this);
void pa_rtsp_auth_qop(rtsp_parser_s *this);

// === inline methods of generated structures ==================================

// -- rtsp_digest_s --
@begin
inlines rtsp_digest_s
@end

// -- rtsp_parser_s --
@begin
inlines rtsp_parser_s
@end

#endif

