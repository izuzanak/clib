
@begin
include "main.h"
@end

// === constants and definitions ===============================================

const char *test_name = "libidb_idl_test";

const char *test_names[] =
{/*{{{*/
  "update_extractor",
  "update_index",
  "remove_docs",
  "reopen_database",
};/*}}}*/

test_function_t test_functions[] =
{/*{{{*/
  test_update_extractor,
  test_update_index,
  test_remove_docs,
  test_reopen_database,
};/*}}}*/

// === test execution functions ================================================

void test_update_extractor()
{/*{{{*/
  VAR_CLEAR(data_var,loc_s_dict_locs(
    loc_s_string_ptr("\"one\""),  loc_s_string_ptr("id"),
    loc_s_string_ptr("\"two\""),  loc_s_string_ptr("id"),
    loc_s_string_ptr("\"three\""),loc_s_string_ptr("text"),
    loc_s_string_ptr("\"four\""), loc_s_string_ptr("text")
    ));

#define UPDATE_EXTRACTOR_DB_PATH "tests/libidb_idl_test/update_extractor"

  // - clear test directory -
  cassert(system("rm -rf " UPDATE_EXTRACTOR_DB_PATH "/test" ) == 0); // NOLINT(cert-env33-c)

  CONT_INIT_CLEAR(idb_database_s,idb);
  cassert(idb_database_s_create(&idb,"test",UPDATE_EXTRACTOR_DB_PATH,50,25) == 0);
  cassert(idb_database_s_update_extractor(&idb,data_var) == 0);
}/*}}}*/

void test_update_index()
{/*{{{*/
#define UPDATE_INDEX_DB_PATH "tests/libidb_idl_test/update_index"

  // - clear test directory -
  cassert(system("rm -rf " UPDATE_INDEX_DB_PATH "/test" ) == 0); // NOLINT(cert-env33-c)

  CONT_INIT_CLEAR(idb_database_s,idb);
  cassert(idb_database_s_create(&idb,"test",UPDATE_INDEX_DB_PATH,50,25) == 0);

  {
    VAR_CLEAR(extract_var,loc_s_dict_locs(
      loc_s_string_ptr("\"id_0\""),  loc_s_string_ptr("id"),
      loc_s_string_ptr("\"id_1\""),  loc_s_string_ptr("id"),
      loc_s_string_ptr("\"descr\""), loc_s_string_ptr("text"),
      loc_s_string_ptr("\"popis\""), loc_s_string_ptr("text")
      ));

    cassert(idb_database_s_update_extractor(&idb,extract_var) == 0);
  }

  unsigned idx = 0;
  unsigned idx_end = 100;
  do {
    VAR_CLEAR(doc_var,loc_s_dict_locs(
      loc_s_string_ptr("id_0"),loc_s_string_format("doc%u",idx),
      loc_s_string_ptr("inner"),loc_s_array_locs(
        loc_s_int(1),
        loc_s_int(2),
        loc_s_dict_locs(
          loc_s_string_ptr("id_1"),loc_s_int(121 + idx),
          loc_s_string_ptr("descr"),loc_s_string_format("Content of document %u",idx),
          loc_s_string_ptr("popis"),loc_s_string_format("Unrelated document description")
        )
      )
    ));

    //if (idx % 10000 == 0)
    //{
    //  fprintf(stderr,"progress: %3.1d%%\n",idx*100/idx_end);
    //}

    cassert(idb_database_s_update_index(&idb,doc_var,idx) == 0);
    cassert(idb_database_s_merge_index(&idb) == 0);
  } while(++idx < idx_end);

  idx = 0;
  do {
    string_s query = STRING_S("document 43");
    idb_database_s_query(&idb,&query);
  } while(++idx < 50);

  CONT_INIT_CLEAR(ui_tree_s,reference);
  ui_tree_s_insert(&reference,43);
  cassert(ui_tree_s_compare(&idb.query_res,&reference));
}/*}}}*/

void test_remove_docs()
{/*{{{*/
#define REMOVE_DOCS_DB_PATH "tests/libidb_idl_test/remove_docs"

  // - clear test directory -
  cassert(system("rm -rf " REMOVE_DOCS_DB_PATH "/test" ) == 0); // NOLINT(cert-env33-c)

  CONT_INIT_CLEAR(idb_database_s,idb);
  cassert(idb_database_s_create(&idb,"test",REMOVE_DOCS_DB_PATH,50,25) == 0);

  {
    VAR_CLEAR(extract_var,loc_s_dict_locs(
      loc_s_string_ptr("\"id_0\""),  loc_s_string_ptr("id"),
      loc_s_string_ptr("\"id_1\""),  loc_s_string_ptr("id"),
      loc_s_string_ptr("\"descr\""),loc_s_string_ptr("text"),
      loc_s_string_ptr("\"popis\""), loc_s_string_ptr("text")
      ));

    cassert(idb_database_s_update_extractor(&idb,extract_var) == 0);
  }

  unsigned idx = 0;
  do {
    VAR_CLEAR(doc_var,loc_s_dict_locs(
      loc_s_string_ptr("id_0"),loc_s_string_format("doc%u",idx),
      loc_s_string_ptr("inner"),loc_s_array_locs(
        loc_s_int(1),
        loc_s_int(2),
        loc_s_dict_locs(
          loc_s_string_ptr("id_1"),loc_s_int(121 + idx),
          loc_s_string_ptr("descr"),loc_s_string_format("Content of document doc%u",idx),
          loc_s_string_ptr("popis"),loc_s_string_format("Unrelated document description")
        )
      )
    ));

    cassert(idb_database_s_update_index(&idb,doc_var,idx) == 0);
    cassert(idb_database_s_merge_index(&idb) == 0);
  } while(++idx < 100);

  string_s query = STRING_S("document id_0 == doc44");

  CONT_INIT_CLEAR(ui_tree_s,reference);
  ui_tree_s_insert(&reference,44);

  idb_database_s_query(&idb,&query);
  cassert(ui_tree_s_compare(&idb.query_res,&reference));

  CONT_INIT_CLEAR(ui_array_s,remove_docs);
  ui_array_s_push(&remove_docs,42);
  ui_array_s_push(&remove_docs,43);
  ui_array_s_push(&remove_docs,44);

  cassert(idb_database_s_remove_docs(&idb,&remove_docs) == 0);
  idb_database_s_query(&idb,&query);
  cassert(!ui_tree_s_compare(&idb.query_res,&reference));
}/*}}}*/

void test_reopen_database()
{/*{{{*/
#define REOPEN_DATABASE_DB_PATH "tests/libidb_idl_test/reopen_database"

  // - clear test directory -
  cassert(system("rm -rf " REOPEN_DATABASE_DB_PATH "/test" ) == 0); // NOLINT(cert-env33-c)

  CONT_INIT_CLEAR(idb_database_s,idb);
  cassert(idb_database_s_create(&idb,"test",REOPEN_DATABASE_DB_PATH,50,25) == 0);

  {
    VAR_CLEAR(extract_var,loc_s_dict_locs(
      loc_s_string_ptr("\"id_0\""),  loc_s_string_ptr("id"),
      loc_s_string_ptr("\"id_1\""),  loc_s_string_ptr("id"),
      loc_s_string_ptr("\"descr\""),loc_s_string_ptr("text"),
      loc_s_string_ptr("\"popis\""), loc_s_string_ptr("text")
      ));

    cassert(idb_database_s_update_extractor(&idb,extract_var) == 0);
  }

  unsigned idx = 0;
  do {
    VAR_CLEAR(doc_var,loc_s_dict_locs(
      loc_s_string_ptr("id_0"),loc_s_string_format("doc%u",idx),
      loc_s_string_ptr("inner"),loc_s_array_locs(
        loc_s_int(1),
        loc_s_int(2),
        loc_s_dict_locs(
          loc_s_string_ptr("id_1"),loc_s_int(121 + idx),
          loc_s_string_ptr("descr"),loc_s_string_format("Content of document doc%u",idx),
          loc_s_string_ptr("popis"),loc_s_string_format("Unrelated document description")
        )
      )
    ));

    cassert(idb_database_s_update_index(&idb,doc_var,idx) == 0);
    cassert(idb_database_s_merge_index(&idb) == 0);
  } while(++idx < 100);

  string_s query = STRING_S("id_0 == doc45");

  CONT_INIT_CLEAR(ui_tree_s,reference);
  ui_tree_s_insert(&reference,45);

  idb_database_s_query(&idb,&query);
  cassert(ui_tree_s_compare(&idb.query_res,&reference));

  // - reopen databas -
  idb_database_s_clear(&idb);
  cassert(idb_database_s_create(&idb,"test",REOPEN_DATABASE_DB_PATH,50,25) == 0);

  idb_database_s_query(&idb,&query);
  cassert(ui_tree_s_compare(&idb.query_res,&reference));
}/*}}}*/

// === program entry function ==================================================

int main(int argc,char **argv)
{/*{{{*/
  (void)argc;
  (void)argv;

  EXECUTE_TESTS(libidb_idl_init(),libidb_idl_clear());

  return 0;
}/*}}}*/

