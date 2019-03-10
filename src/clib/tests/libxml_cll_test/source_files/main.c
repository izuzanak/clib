
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libxml_cll_test";

const char *test_names[] =
{/*{{{*/
  "xml_parse",
  "xml_create",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_xml_parse,
  test_xml_create,
};/*}}}*/

// === test execution functions ================================================

#define VAR_S_TO_STRING(NAME) \
{/*{{{*/\
  buffer.used = 0;\
  var_s_to_string(NAME,&buffer);\
  bc_array_s_push(&buffer,'\0');\
}/*}}}*/

void test_xml_parse()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  CONT_INIT(file_s,file);
  cassert(file_s_open(&file,"tests/libxml_cll_test/resources/xml_parse_0.xml","r") == 0);

  CONT_INIT(bc_array_s,data);
  data.used = 0;
  cassert(file_s_read_close(&file,&data) == 0);
  bc_array_s_push(&data,'\0');

  string_s source = {data.used - 1,data.data};

  // - xml_parse -
  var_s xml_var = NULL;
  cassert(xml_parse(&source,&xml_var) == 0);

  // - xml_node_s_update_node_dict -
  xml_node_s_update_node_dict(xml_var);
  VAR_S_TO_STRING(&loc_s_xml_node_value(xml_var)->node_dict);
  puts(buffer.data);

  // - xml_create -
  // - xml_create_nice -
  CONT_INIT(string_s,tab);
  string_s_set_ptr(&tab,"  ");

  CONT_INIT(string_s,indent);
  string_s_set_ptr(&indent,"--");

  xml_create_nice(xml_var,&tab,&indent,&buffer);
  bc_array_s_push(&buffer,'\0');
  puts(buffer.data);

  string_s_clear(&indent);
  string_s_clear(&tab);

  var_s_clear(&xml_var);
  bc_array_s_clear(&data);
  file_s_clear(&file);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

void test_xml_create()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT(bc_array_s,buffer);

  CONT_INIT(string_s,tab);
  string_s_set_ptr(&tab,"==");

  CONT_INIT(string_s,indent);
  string_s_set_ptr(&indent,"--");

  VAR(root,loc_s_xml_node(loc_s_string_ptr("ROOT")));
  xml_create(root,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"<ROOT></ROOT>") == 0);

  unsigned c_idx = 0;
  do {
    buffer.used = 0;
    ui_to_string(&c_idx,&buffer);

    var_s consist = xml_node_s_node(root,loc_s_string_ptr("CONSIST"));
    xml_node_s_attr(consist,loc_s_string_ptr("id"),loc_s_string(buffer.used,buffer.data));

    unsigned v_idx = 0;
    do {
      CONT_INIT(string_s,descr);
      string_s_set_format(&descr,"Description of vehicle %u of consist %u",v_idx,c_idx);

      xml_node_s_node_text(consist,loc_s_string_ptr("VEHICLE"),loc_s_string_swap(&descr));
    } while(++v_idx < 5);
  } while(++c_idx < 10);

  xml_create_nice(root,&tab,&indent,&buffer);
  bc_array_s_push(&buffer,'\0');

  CONT_INIT(file_s,file);
  cassert(file_s_open(&file,"tests/libxml_cll_test/resources/xml_create_0.xml","r") == 0);

  CONT_INIT(bc_array_s,data);
  data.used = 0;
  cassert(file_s_read_close(&file,&data) == 0);

  // - replace last '\n' by '\0' -
  data.data[data.used - 1] = '\0';
  bc_array_s_push(&data,'\0');
  cassert(strcmp(buffer.data,data.data) == 0);

  bc_array_s_clear(&data);
  file_s_clear(&file);
  var_s_clear(&root);
  string_s_clear(&indent);
  string_s_clear(&tab);
  bc_array_s_clear(&buffer);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libxml_init(),libxml_clear());

  return 0;
}/*}}}*/

