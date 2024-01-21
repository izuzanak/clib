
#ifndef ID_LIB_IDB_H
#define ID_LIB_IDB_H

@begin
include "cl_linux.h"
include "cl_parser.h"
include "cl_var.h"
include "cl_utf8proc.h"
@end

// - function export definitions -
#if SYSTEM_TYPE == SYSTEM_TYPE_UNIX
#define libidb_idl_EXPORT
#elif SYSTEM_TYPE == SYSTEM_TYPE_WINDOWS
#ifdef libidb_idl_EXPORTS
#define libidb_idl_EXPORT __declspec(dllexport)
#else
#define libidb_idl_EXPORT __declspec(dllimport)
#endif
#endif

// - error codes -
#define ERROR_IDB_DATABASE_CREATE_DIR_OPEN_ERROR 1
#define ERROR_IDB_DATABASE_CREATE_DIR_READ_ERROR 2
#define ERROR_IDB_DATABASE_CREATE_REMOVE_ERROR 3
#define ERROR_IDB_DATABASE_CREATE_RENAME_ERROR 4
#define ERROR_IDB_DATABASE_CREATE_MAP_ERROR 5
#define ERROR_IDB_DATABASE_CREATE_FILENAME_ERROR 6
#define ERROR_IDB_DATABASE_UPDATE_EXTRACTOR_INVALID_DATA 7
#define ERROR_IDB_DATABASE_UPDATE_INDEX_INVALID_DATA 8
#define ERROR_IDB_DATABASE_EXTRACT_REGEXPS_EXTRACTOR_INDEX_MISSING 9
#define ERROR_IDB_DATABASE_EXTRACT_REGEXPS_UTF8_ERROR 10
#define ERROR_IDB_DATABASE_EXTRACT_REGEXPS_UNKNOWN_EXTRACTED_TYPE 11
#define ERROR_IDB_DATABASE_UPDATE_EXTRACTOR_WRITE_ERROR 12
#define ERROR_IDB_DATABASE_UPDATE_EXTRACTOR_RENAME_ERROR 13
#define ERROR_IDB_DATABASE_DUMP_INDEX_WRITE_ERROR 14
#define ERROR_IDB_DATABASE_DUMP_INDEX_RENAME_ERROR 15
#define ERROR_IDB_DATABASE_MAP_INDEX_OPEN_ERROR 16
#define ERROR_IDB_DATABASE_MAP_INDEX_SEEK_ERROR 17
#define ERROR_IDB_DATABASE_MAP_INDEX_MAP_ERROR 18
#define ERROR_IDB_DATABASE_UPDATE_INDEX_EXTRACT_REGEXPS_ERROR 19
#define ERROR_IDB_DATABASE_UPDATE_INDEX_INVALID_REGEXP 20
#define ERROR_IDB_DATABASE_UPDATE_INDEX_DUMP_ERROR 21
#define ERROR_IDB_DATABASE_UPDATE_INDEX_RENAME_ERROR 22
#define ERROR_IDB_DATABASE_MERGE_INDEX_LOAD_ERROR 23
#define ERROR_IDB_DATABASE_MERGE_INDEX_DUMP_ERROR 24
#define ERROR_IDB_DATABASE_MERGE_INDEX_SWAP_ERROR 25
#define ERROR_IDB_DATABASE_MERGE_INDEX_RENAME_ERROR 26
#define ERROR_IDB_DATABASE_MERGE_INDEX_REMOVE_ERROR 27
#define ERROR_IDB_DATABASE_REMOVE_DOCS_LOAD_ERROR 28
#define ERROR_IDB_DATABASE_REMOVE_DOCS_RENAME_ERROR 29
#define ERROR_IDB_DATABASE_REMOVE_DOCS_DUMP_ERROR 30
#define ERROR_IDB_DATABASE_REMOVE_DOCS_SWAP_ERROR 31
#define ERROR_IDB_DATABASE_REMOVE_DOCS_REMOVE_ERROR 32

// - extracted types -
enum
{/*{{{*/
  c_idb_extracted_type_ID = 1,
  c_idb_extracted_type_TEXT,
};/*}}}*/

// === definition of generated structures ======================================

#define IDB_BITS_TARGET_POS(TARGET) ((TARGET)  & 0xfffffe00)
#define IDB_BITS_TARGET_IDX(TARGET) (((TARGET) & 0x000001ff) >> 6)
#define IDB_BITS_TARGET_OFF(TARGET) ((TARGET)  & 0x0000003f)
#define IDB_BITS_SLOT_COUNT 8
#define IDB_BITS_BITCOUNT (IDB_BITS_SLOT_COUNT*64)

// -- idb_bits_s --
@begin
struct
<
ui:pos
ulli:bits0
ulli:bits1
ulli:bits2
ulli:bits3
ulli:bits4
ulli:bits5
ulli:bits6
ulli:bits7
>
idb_bits_s;
@end

// -- idb_bits_tree_s --
@begin
safe_rb_tree<idb_bits_s> idb_bits_tree_s;
@end

void idb_bits_tree_s_to_query_res(idb_bits_tree_s *this,ui_array_s *a_trg);

// -- idb_inverted_index_s --
@begin
struct
<
mmap_s:mmap
>
idb_inverted_index_s;
@end

// -- idb_inverted_index_tree_s --
@begin
safe_rb_tree<idb_inverted_index_s> idb_inverted_index_tree_s;
@end

// -- idb_database_s --
@begin
struct
<
$// - properties -
string_s:name
string_s:dir_data
string_s:dir_keep
string_s:dir_remove
ui:indexes_max
ui:merge_cnt

$// - buffers -
string_array_s:reg_exps
fa_states_array_s:states_array
bc_array_s:utf8_buffer
bc_array_s:buffer

ui_array_s:ii_dump
ui_array_s:remove_tree_idxs
ui_array_s:keep_tree_idxs
bc_array_s:old_path
bc_array_s:new_path

$// - storage -
idb_inverted_index_s:extractor_index
idb_inverted_index_tree_s:inverted_index_tree
idb_bits_tree_s:bits_res
>
idb_database_s;
@end

WUR libidb_idl_EXPORT int idb_database_s_create(idb_database_s *this,const char *a_name,
    const char *a_storage_path,unsigned a_max_indexes,unsigned a_merge_cnt);
WUR libidb_idl_EXPORT int idb_database_s_update_extractor(idb_database_s *this,
    var_s a_data);
WUR libidb_idl_EXPORT int idb_database_s_extract_regexps(idb_database_s *this,
    var_s a_doc,string_array_s *a_reg_exps);

WUR libidb_idl_EXPORT int idb_database_s_dump_index(idb_database_s *this,
    inverted_index_s *a_inverted_index,unsigned *a_ii_index);
WUR libidb_idl_EXPORT int idb_database_s_map_index(
    const char *a_path,idb_inverted_index_s *a_inverted_index);

WUR libidb_idl_EXPORT int idb_database_s_update_index(idb_database_s *this,
    var_s a_doc,unsigned a_doc_index);
WUR libidb_idl_EXPORT int idb_database_s_merge_index(idb_database_s *this);
WUR libidb_idl_EXPORT int idb_database_s_remove_docs(idb_database_s *this,
    ui_array_s *a_doc_indexes);
WUR libidb_idl_EXPORT int idb_database_s_query(idb_database_s *this,
    const string_s *a_query);

// -- idb_database_tree_s --
@begin
safe_rb_tree<idb_database_s> idb_database_tree_s;
@end

// === definition of global functions ==========================================

extern var_tree_s g_id_type_vars;

void libidb_idl_init();
void libidb_idl_clear();

// === inline methods of generated structures ==================================

// -- idb_bits_s --
@begin
inlines idb_bits_s
@end

// -- idb_bits_tree_s --
@begin
inlines idb_bits_tree_s
@end

static inline int idb_bits_tree_s___compare_value(const idb_bits_tree_s *this,const idb_bits_s *a_first,const idb_bits_s *a_second)
{/*{{{*/
  (void)this;

  return a_first->pos < a_second->pos ? -1 : a_first->pos > a_second->pos ? 1 : 0;
}/*}}}*/

// -- idb_inverted_index_s --
@begin
inlines idb_inverted_index_s
@end

// -- idb_inverted_index_tree_s --
@begin
inlines idb_inverted_index_tree_s
@end

static inline int idb_inverted_index_tree_s___compare_value(const idb_inverted_index_tree_s *this,const idb_inverted_index_s *a_first,const idb_inverted_index_s *a_second)
{/*{{{*/
  (void)this;

  unsigned f_state_cnt = a_first->mmap.address != NULL ?
    ((unsigned *)a_first->mmap.address)[c_ii_dump_state_count_offset] : 0;

  unsigned s_state_cnt = a_second->mmap.address != NULL ?
    ((unsigned *)a_second->mmap.address)[c_ii_dump_state_count_offset] : 0;

  return f_state_cnt < s_state_cnt ? -1 : f_state_cnt > s_state_cnt ? 1 : 0;
}/*}}}*/

// -- idb_database_s --
@begin
inlines idb_database_s
@end

// -- idb_database_tree_s --
@begin
inlines idb_database_tree_s
@end

static inline int idb_database_tree_s___compare_value(const idb_database_tree_s *this,const idb_database_s *a_first,const idb_database_s *a_second)
{/*{{{*/
  (void)this;

  const string_s *first = &a_first->name;
  const string_s *second = &a_second->name;

  if (first->size < second->size) { return -1; }
  if (first->size > second->size) { return 1; }
  return memcmp(first->data,second->data,first->size - 1);
}/*}}}*/

#endif

