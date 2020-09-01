
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libodb_odl_test";

const char *test_names[] =
{/*{{{*/
  "set_get_value",
  "add_remove_node",
  "find_remove_node",
  "nodes_path_tree",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_set_get_value,
  test_add_remove_node,
  test_find_remove_node,
  test_nodes_path_tree,
};/*}}}*/

// === test execution functions ================================================

void test_set_get_value()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

  CONT_INIT_CLEAR(odb_database_s,db);
  odb_database_s_create(&db);

  int updated;
  odb_database_s_set_value(&db,"first/second/third/value",loc_s_string_ptr("Hello database"),&updated);

  CONT_INIT_CLEAR(var_s,value);
  odb_database_s_get_value(&db,"wrong/path",&value);

  buffer.used = 0;
  var_s_to_string(&value,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"<null>") == 0);

  odb_database_s_get_value(&db,"first/second/third/value",&value);
  buffer.used = 0;
  var_s_to_string(&value,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"Hello database") == 0);

  odb_database_s_get_value(&db,"first",&value);
  buffer.used = 0;
  var_s_to_string(&value,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[second:[third:[value:Hello database]]]") == 0);
#endif
}/*}}}*/

void test_add_remove_node()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

  CONT_INIT_CLEAR(odb_database_s,db);
  odb_database_s_create(&db);

  CONT_INIT_CLEAR(var_s,info);
  odb_database_s_add_node(&db,"first/second/third/value_0",&info);
  odb_database_s_add_node(&db,"first/second/third/value_1",&info);

  buffer.used = 0;
  var_s_to_string(&info,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"odb_node{info:<null>,path:first/second/third/value_1,nodes:[]}") == 0);

  var_s_copy_loc(&loc_s_odb_node_value(info)->info,loc_s_string_ptr("watch"));

  buffer.used = 0;
  var_s_to_string(&info,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"odb_node{info:watch,path:first/second/third/value_1,nodes:[]}") == 0);

  odb_database_s_add_node(&db,"first/second/third/value_0",&info);
  odb_database_s_remove_node(&db);
  odb_database_s_add_node(&db,"first/second/third/value_1",&info);
  odb_database_s_remove_node(&db);

  buffer.used = 0;
  var_s_to_string(&db.node_var,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"odb_node{info:<null>,path:,nodes:[]}") == 0);
#endif
}/*}}}*/

void test_find_remove_node()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

  CONT_INIT_CLEAR(odb_database_s,db);
  odb_database_s_create(&db);

  CONT_INIT_CLEAR(var_s,info);
  odb_database_s_add_node(&db,"first/second/third/value_0",&info);
  odb_database_s_add_node(&db,"first/second/third/value_1",&info);

  var_s_copy_loc(&info,NULL);

  odb_database_s_find_node(&db,"first/second/third/value_1",&info);

  buffer.used = 0;
  var_s_to_string(&info,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"odb_node{info:<null>,path:first/second/third/value_1,nodes:[]}") == 0);

  odb_database_s_find_node(&db,"first/second/third/value_2",&info);

  buffer.used = 0;
  var_s_to_string(&info,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"<null>") == 0);
#endif
}/*}}}*/

void test_nodes_path_tree()
{/*{{{*/
#if OPTION_TO_STRING == ENABLED
  CONT_INIT_CLEAR(bc_array_s,buffer);

  CONT_INIT_CLEAR(odb_database_s,db);
  odb_database_s_create(&db);

  CONT_INIT_CLEAR(var_s,node);

  odb_database_s_add_node(&db,"first/second/third/value",&node);
  var_s_copy_loc(&loc_s_odb_node_value(node)->info,loc_s_string_ptr("info_0"));

  odb_database_s_add_node(&db,"first/second/third",&node);
  var_s_copy_loc(&loc_s_odb_node_value(node)->info,loc_s_string_ptr("info_1"));

  odb_database_s_add_node(&db,"first/second",&node);
  var_s_copy_loc(&loc_s_odb_node_value(node)->info,loc_s_string_ptr("info_2"));

  CONT_INIT_CLEAR(var_array_s,nodes);

  nodes.used = 0;
  odb_database_s_find_node(&db,"first/second/third",&node);
  odb_database_s_nodes_path(&db,&nodes);

  buffer.used = 0;
  var_array_s_to_string(&nodes,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[odb_node{info:info_2,path:first/second,nodes:[third:odb_node{info:info_1,path:first/second/third,nodes:[value:odb_node{info:info_0,path:first/second/third/value,nodes:[]}]}]}]") == 0);

  nodes.used = 0;
  odb_database_s_nodes_tree(node,&nodes);

  buffer.used = 0;
  var_array_s_to_string(&nodes,&buffer);
  bc_array_s_push(&buffer,'\0');
  cassert(strcmp(buffer.data,"[odb_node{info:info_1,path:first/second/third,nodes:[value:odb_node{info:info_0,path:first/second/third/value,nodes:[]}]},odb_node{info:info_0,path:first/second/third/value,nodes:[]}]") == 0);
#endif
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libodb_odl_init(),libodb_odl_clear());

  return 0;
}/*}}}*/

