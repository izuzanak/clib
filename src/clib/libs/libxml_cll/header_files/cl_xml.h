
#ifndef CL_LIB_XML_H
#define CL_LIB_XML_H

@begin
include "cl_var.h"
@end

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libxml_cll_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libxml_cll_EXPORTS
#define libxml_cll_EXPORT __declspec(dllexport)
#else
#define libxml_cll_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_XML_ERROR_PARSING_DATA 1
#define ERROR_XML_NODE_NODE_DICT_TO_VAR_ERROR 2
#define ERROR_XML_NODE_NODE_VALUE_TYPE_ERROR 3

// === definition of generated structures ======================================

// -- string_map_s --
@begin
struct
<
string_s:key
string_s:value
>
string_map_s;
@end

// -- string_map_tree_s --
@begin
rb_tree<string_map_s> string_map_tree_s;
@end

static inline void string_map_tree_s_insert_map(string_map_tree_s *this,
    const char *a_ns,const char *a_abbr);

// -- xml_create_stack_element_s --
@begin
struct
<
pointer:node
ui:index
bi:after_node
>
xml_create_stack_element_s;
@end

// -- xml_create_stack_s --
@begin
array<xml_create_stack_element_s> xml_create_stack_s;
@end

// -- xml_node_s --
@begin
struct
<
var_s:name
var_s:attributes
var_s:node_dict
var_s:nodes
var_s:texts
var_s:conts
bi:escape_texts
>
xml_node_s;
@end

extern unsigned g_type_xml_node;

static inline var_s loc_s_xml_node(var_s a_name);
static inline void loc_s_xml_node_clear(var_s this);
static inline int loc_s_xml_node_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_xml_node_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_xml_node_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_xml_node_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
static inline xml_node_s *loc_s_xml_node_value(var_s this);

static inline void xml_node_s___update_conts_array(var_s *a_array,var_s a_value);
void xml_node_s___add_node_to_node_dict(var_map_tree_s *a_tree,var_s a_name,var_s a_node);
libxml_cll_EXPORT void xml_node_s_update_node_dict(var_s this);
WUR int xml_node_s___node_dict_to_var(var_s this,var_s *a_trg);
WUR libxml_cll_EXPORT int xml_node_s_node_dict_to_var(var_s this,var_s *a_trg);
static inline void xml_node_s_attr(var_s this,var_s a_name,var_s a_value);
static inline var_s xml_node_s_node(var_s this,var_s a_name);
static inline var_s xml_node_s_node_text(var_s this,var_s a_name,var_s a_text);
static inline void xml_node_s_text(var_s this,var_s a_text);

// -- xml_parser_s --
@begin
struct
<
string_tree_s:const_strings
var_array_s:string_vars

string_map_tree_s:ns_abbr_map
string_map_tree_s:abbr_ns_map

var_array_s:ns_stack
var_array_s:attrs_stack

var_arrays_s:node_array_stack
var_arrays_s:text_array_stack
var_arrays_s:cont_array_stack

bc_array_s:buffer
>
xml_parser_s;
@end

static inline void xml_parser_s_start_document(void *user);
static inline void xml_parser_s_end_document(void *user);
void xml_parser_s_start_element(void *user,const xmlChar *name,const xmlChar **a_attrs);
void xml_parser_s_end_element(void *user,const xmlChar *name);
void xml_parser_s_characters(void *user,const xmlChar *ch,int len);

static inline void xml_parser_s_warning(void *user,const char *msg,...);
static inline void xml_parser_s_error(void *user,const char *msg,...);
static inline void xml_parser_s_fatal_error(void *user,const char *msg,...);

var_s xml_parser_s_get_string_var(xml_parser_s *this,unsigned a_length,const char *a_data);

// === definition of global functions ==========================================

WUR libxml_cll_EXPORT int xml_parse_ns(const string_s *a_src,
    var_s *a_trg,string_map_tree_s *a_ns_abbr_map);
static inline int xml_parse(const string_s *a_src,var_s *a_trg);
void xml_create_append_string(unsigned a_length,const char *a_data,bc_array_s *a_trg);
libxml_cll_EXPORT void xml_create(var_s a_node,bc_array_s *a_trg);
libxml_cll_EXPORT void xml_create_nice(var_s a_node,
    const string_s *a_tabulator,const string_s *a_indent,bc_array_s *a_trg);

void libxml_cll_init();
void libxml_cll_clear();

// === constants and definitions ===============================================

extern var_s g_str_empty_var;
extern var_s g_str_attrs_var;
extern var_s g_str_nodes_var;
extern var_s g_str_text_var;

// === inline methods of generated structures ==================================

// -- string_map_s --
@begin
inlines string_map_s
@end

// -- string_map_tree_s --
@begin
inlines string_map_tree_s
@end

static inline int string_map_tree_s___compare_value(const string_map_tree_s *this,const string_map_s *a_first,const string_map_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->key;
  const string_s *second = &a_second->key;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }
  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

static inline void string_map_tree_s_insert_map(string_map_tree_s *this,
    const char *a_ns,const char *a_abbr)
{/*{{{*/
  CONT_INIT_CLEAR(string_map_s,string_map);
  string_s_set(&string_map.key,strlen(a_ns),a_ns);
  string_s_set(&string_map.value,strlen(a_abbr),a_abbr);

  string_map_tree_s_unique_swap_insert(this,&string_map);
}/*}}}*/

// -- xml_create_stack_element_s --
@begin
inlines xml_create_stack_element_s
@end

// -- xml_create_stack_s --
@begin
inlines xml_create_stack_s
@end

// -- xml_node_s --
@begin
inlines xml_node_s
@end

static inline var_s loc_s_xml_node(var_s a_name)
{/*{{{*/
  xml_node_s *xml_node = (xml_node_s *)cmalloc(sizeof(xml_node_s));
  xml_node_s_init(xml_node);

  var_s_copy_loc(&xml_node->name,a_name);
  xml_node->escape_texts = 1;

  var_s var = loc_s___new();
  var->v_type = g_type_xml_node;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = xml_node;

  return var;
}/*}}}*/

static inline void loc_s_xml_node_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == g_type_xml_node);

  xml_node_s *xml_node = (xml_node_s *)this->v_data.ptr;
  xml_node_s_clear(xml_node);
  cfree(xml_node);
}/*}}}*/

static inline int loc_s_xml_node_order(var_s a_first,var_s a_second)
{/*{{{*/
  (void)a_first;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_xml_node_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == g_type_xml_node);

  xml_node_s *xml_node = (xml_node_s *)this->v_data.ptr;

  bc_array_s_append_ptr(a_trg,"xml_node{");
  var_s_to_string(&xml_node->name,a_trg);
  bc_array_s_push(a_trg,'}');
}/*}}}*/
#endif

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_xml_node_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  (void)this;
  (void)a_trg;

  cassert(0);
}/*}}}*/

static inline void loc_s_xml_node_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  (void)this;
  (void)a_json_nice;
  (void)a_trg;

  cassert(0);
}/*}}}*/
#endif

static inline xml_node_s *loc_s_xml_node_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == g_type_xml_node);

  return (xml_node_s *)this->v_data.ptr;
}/*}}}*/

static inline void xml_node_s_attr(var_s this,var_s a_name,var_s a_value)
{/*{{{*/
  xml_node_s *node = loc_s_xml_node_value(this);

  if (node->attributes == NULL)
  {
    var_s_copy_loc(&node->attributes,loc_s_dict());
  }

  var_map_tree_s *tree = loc_s_dict_value(node->attributes);

  var_map_s insert_map = {a_name,a_value};
  unsigned index = var_map_tree_s_unique_insert(tree,&insert_map);

  var_map_s *map = &(tree->data + index)->object;

  if (map->value != a_value)
  {
    var_s_copy_loc(&map->value,a_value);
  }
}/*}}}*/

static inline void xml_node_s___update_conts_array(var_s *a_array,var_s a_value)
{/*{{{*/
  if (*a_array == NULL)
  {
    // - create new conts array -
    var_s array = loc_s_array();
    loc_s_array_push(array,a_value);
    var_s_copy_loc(a_array,array);
  }
  else
  {
    // - push value to conts array -
    loc_s_array_push(*a_array,a_value);
  }
}/*}}}*/

static inline var_s xml_node_s_node(var_s this,var_s a_name)
{/*{{{*/
  xml_node_s *node = loc_s_xml_node_value(this);

  var_s new_node_var = loc_s_xml_node(a_name);
  xml_node_s___update_conts_array(&node->nodes,new_node_var);
  xml_node_s___update_conts_array(&node->conts,new_node_var);

  return new_node_var;
}/*}}}*/

static inline var_s xml_node_s_node_text(var_s this,var_s a_name,var_s a_text)
{/*{{{*/
  xml_node_s *node = loc_s_xml_node_value(this);

  var_s new_node_var = loc_s_xml_node(a_name);
  xml_node_s___update_conts_array(&node->nodes,new_node_var);
  xml_node_s___update_conts_array(&node->conts,new_node_var);

  xml_node_s *new_node = loc_s_xml_node_value(new_node_var);
  xml_node_s___update_conts_array(&new_node->texts,a_text);
  xml_node_s___update_conts_array(&new_node->conts,a_text);

  return new_node_var;
}/*}}}*/

static inline void xml_node_s_text(var_s this,var_s a_text)
{/*{{{*/
  xml_node_s *node = loc_s_xml_node_value(this);
  xml_node_s___update_conts_array(&node->texts,a_text);
  xml_node_s___update_conts_array(&node->conts,a_text);
}/*}}}*/

// -- xml_parser_s --
@begin
inlines xml_parser_s
@end

static inline void xml_parser_s_start_document(void *user)
{/*{{{*/
}/*}}}*/

static inline void xml_parser_s_end_document(void *user)
{/*{{{*/
}/*}}}*/

static inline void xml_parser_s_warning(void *user,const char *msg,...)
{/*{{{*/
}/*}}}*/

static inline void xml_parser_s_error(void *user,const char *msg,...)
{/*{{{*/
}/*}}}*/

static inline void xml_parser_s_fatal_error(void *user,const char *msg,...)
{/*{{{*/
}/*}}}*/

// === inline global functions =================================================

static inline int xml_parse(const string_s *a_src,var_s *a_trg)
{/*{{{*/
  return xml_parse_ns(a_src,a_trg,NULL);
}/*}}}*/

#endif

