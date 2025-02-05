
#ifndef OD_LIB_ODB_H
#define OD_LIB_ODB_H

@begin
include "cl_var.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libodb_odl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libodb_odl_EXPORTS
#define libodb_odl_EXPORT __declspec(dllexport)
#else
#define libodb_odl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_OD_ODB_DUMMY_ERROR 1

// === constants and definitions ===============================================

enum
{/*{{{*/
  od_watch_SEND_MODIFICATIONS = 0x01,
};/*}}}*/

// === definition of generated structures ======================================

// -- odb_node_s --
@begin
struct
<
var_s:info
var_s:path
var_s:nodes
>
odb_node_s;
@end

extern unsigned g_type_odb_node;

static inline var_s loc_s_odb_node(unsigned a_length,const char *a_data);
static inline void loc_s_odb_node_clear(var_s this);
static inline int loc_s_odb_node_order(var_s a_first,var_s a_second);
#if OPTION_TO_STRING == ENABLED
static inline void loc_s_odb_node_to_string(var_s this,bc_array_s *a_trg);
#endif
#if OPTION_TO_JSON == ENABLED
static inline void loc_s_odb_node_to_json(var_s this,bc_array_s *a_trg);
static inline void loc_s_odb_node_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg);
#endif
static inline odb_node_s *loc_s_odb_node_value(var_s this);

// -- odb_node_key_s --
@begin
struct
<
var_s:node_var
var_s:key_var
>
odb_node_key_s;
@end

// -- odb_node_key_array_s --
@begin
array<odb_node_key_s> odb_node_key_array_s;
@end

// -- odb_database_s --
@begin
struct
<
var_s:node_var
var_s:data_var

odb_node_key_array_s:path_stack
>
odb_database_s;
@end

void odb_database_s_create(odb_database_s *this);

void odb_database_s_set_value(odb_database_s *this,const char *a_path,var_s a_value_var,int *a_updated);
void odb_database_s_get_value(odb_database_s *this,const char *a_path,var_s *a_value_var);

void odb_database_s_add_node(odb_database_s *this,const char *a_path,var_s *a_info_var);
void odb_database_s_find_node(odb_database_s *this,const char *a_path,var_s *a_info_var);
void odb_database_s_remove_node(odb_database_s *this);

void odb_database_s_nodes_path(odb_database_s *this,var_array_s *a_nodes);
void odb_database_s_nodes_tree(var_s a_node_var,var_array_s *a_nodes);

// === definition of global functions ==========================================

void libodb_odl_init();
void libodb_odl_clear();

// === inline methods of generated structures ==================================

// -- odb_node_s --
@begin
inlines odb_node_s
@end

static inline var_s loc_s_odb_node(unsigned a_length,const char *a_data)
{/*{{{*/
  odb_node_s *odb_node = (odb_node_s *)cmalloc(sizeof(odb_node_s));
  odb_node_s_init(odb_node);

  var_s_copy_loc(&odb_node->path,loc_s_string(a_length,a_data));
  var_s_copy_loc(&odb_node->nodes,loc_s_dict());

  var_s var = loc_s___new();
  var->v_type = g_type_odb_node;
  atomic_s_set(&var->v_ref_cnt,0);
  var->v_data.ptr = odb_node;

  return var;
}/*}}}*/

static inline void loc_s_odb_node_clear(var_s this)
{/*{{{*/
  debug_assert(this->v_type == g_type_odb_node);

  odb_node_s *odb_node = (odb_node_s *)this->v_data.ptr;
  odb_node_s_clear(odb_node);
  cfree(odb_node);
}/*}}}*/

static inline int loc_s_odb_node_order(var_s a_first,var_s a_second)
{/*{{{*/
  (void)a_first;
  (void)a_second;

  cassert(0);
  return 0;
}/*}}}*/

#if OPTION_TO_STRING == ENABLED
static inline void loc_s_odb_node_to_string(var_s this,bc_array_s *a_trg)
{/*{{{*/
  debug_assert(this->v_type == g_type_odb_node);

  odb_node_s *odb_node = (odb_node_s *)this->v_data.ptr;

  bc_array_s_append_ptr(a_trg,"odb_node{info:");
  var_s_to_string(&odb_node->info,a_trg);
  bc_array_s_append_ptr(a_trg,",path:");
  var_s_to_string(&odb_node->path,a_trg);
  bc_array_s_append_ptr(a_trg,",nodes:");
  var_s_to_string(&odb_node->nodes,a_trg);
  bc_array_s_push(a_trg,'}');
}/*}}}*/
#endif

#if OPTION_TO_JSON == ENABLED
static inline void loc_s_odb_node_to_json(var_s this,bc_array_s *a_trg)
{/*{{{*/
  (void)this;
  (void)a_trg;

  cassert(0);
}/*}}}*/

static inline void loc_s_odb_node_to_json_nice(var_s this,json_nice_s *a_json_nice,bc_array_s *a_trg)
{/*{{{*/
  (void)this;
  (void)a_json_nice;
  (void)a_trg;

  cassert(0);
}/*}}}*/
#endif

static inline odb_node_s *loc_s_odb_node_value(var_s this)
{/*{{{*/
  debug_assert(this->v_type == g_type_odb_node);

  return (odb_node_s *)this->v_data.ptr;
}/*}}}*/

// -- odb_node_key_s --
@begin
inlines odb_node_key_s
@end

// -- odb_node_key_array_s --
@begin
inlines odb_node_key_array_s
@end

// -- odb_database_s --
@begin
inlines odb_database_s
@end

#endif

