
@begin
include "id_idb.h"
@end

// === constants and definitions ===============================================

const char *g_id_type_strings[] =
{/*{{{*/
  "id",
  "text",
};/*}}}*/

var_tree_s g_id_type_vars;

// === methods of generated structures =========================================

// -- idb_inverted_index_s --
@begin
methods idb_inverted_index_s
@end

// -- idb_inverted_index_tree_s --
@begin
methods idb_inverted_index_tree_s
@end

// -- idb_database_s --
@begin
methods idb_database_s
@end

int idb_database_s_create(idb_database_s *this,const char *a_name,
    const char *a_storage_path,unsigned a_max_indexes,unsigned a_merge_cnt)
{/*{{{*/
  string_s_set_ptr(&this->name,a_name);
  string_s_set_format(&this->dir_data,"%s/%s",a_storage_path,a_name);
  string_s_set_format(&this->dir_keep,"%s/%s/keep",a_storage_path,a_name);
  string_s_set_format(&this->dir_remove,"%s/%s/remove",a_storage_path,a_name);
  this->indexes_max = a_max_indexes;
  this->merge_cnt = a_merge_cnt;

  // - create data directory -
  mkdir(this->dir_data.data,0777);

  // - create working directories -
  mkdir(this->dir_keep.data,0777);
  mkdir(this->dir_remove.data,0777);

  CONT_INIT_CLEAR(string_array_s,filenames);

  // - clean keep directory -
  {/*{{{*/
    CONT_INIT_CLEAR(dir_s,dir);
    if (dir_s_open(&dir,this->dir_keep.data))
    {
      throw_error(IDB_DATABASE_CREATE_DIR_OPEN_ERROR);
    }

    filenames.used = 0;
    struct dirent *entry;
    do {
      cassert(dir_s_read(&dir,&entry) == 0);
      if (entry == NULL) { break; }

      if (entry->d_type == DT_REG)
      {
        string_array_s_push_ptr(&filenames,entry->d_name);
      }
    } while(1);

    // - move files to data directory -
    while (filenames.used != 0)
    {
      string_s *filename = string_array_s_pop(&filenames);

      this->old_path.used = 0;
      bc_array_s_append_format(&this->old_path,"%s/%s",this->dir_keep.data,filename->data);

      this->new_path.used = 0;
      bc_array_s_append_format(&this->new_path,"%s/%s",this->dir_data.data,filename->data);

      if (rename(this->old_path.data,this->new_path.data) != 0)
      {
        throw_error(IDB_DATABASE_CREATE_RENAME_ERROR);
      }
    }
  }/*}}}*/

  // - clean remove directory -
  {/*{{{*/
    CONT_INIT_CLEAR(dir_s,dir);
    if (dir_s_open(&dir,this->dir_remove.data))
    {
      throw_error(IDB_DATABASE_CREATE_DIR_OPEN_ERROR);
    }

    filenames.used = 0;
    struct dirent *entry;
    do {
      cassert(dir_s_read(&dir,&entry) == 0);
      if (entry == NULL) { break; }

      if (entry->d_type == DT_REG)
      {
        string_array_s_push_ptr(&filenames,entry->d_name);
      }
    } while(1);

    // - remove files -
    while (filenames.used != 0)
    {
      string_s *filename = string_array_s_pop(&filenames);

      this->old_path.used = 0;
      bc_array_s_append_format(&this->old_path,"%s/%s",this->dir_remove.data,filename->data);

      if (remove(this->old_path.data) != 0)
      {
        throw_error(IDB_DATABASE_CREATE_REMOVE_ERROR);
      }
    }
  }/*}}}*/

  // - read extractor index -
  {/*{{{*/
    this->old_path.used = 0;
    bc_array_s_append_format(&this->old_path,"%s/extract",this->dir_data.data);

    struct stat st;
    if (stat(this->old_path.data,&st) == 0 && S_ISREG(st.st_mode))
    {
      if (idb_database_s_map_index(this->old_path.data,&this->extractor_index))
      {
        throw_error(IDB_DATABASE_CREATE_MAP_ERROR);
      }
    }
  }/*}}}*/

  // - read inverted indexes -
  {
    CONT_INIT_CLEAR(ui_tree_s,file_indexes);

    // - retrieve file indexes -
    CONT_INIT_CLEAR(dir_s,dir);
    if (dir_s_open(&dir,this->dir_data.data))
    {
      throw_error(IDB_DATABASE_CREATE_DIR_OPEN_ERROR);
    }

    struct dirent *entry;
    do {
      cassert(dir_s_read(&dir,&entry) == 0);
      if (entry == NULL) { break; }

      if (entry->d_type == DT_REG)
      {
        const char *filename = entry->d_name;
        size_t filename_length = strlen(filename);

        // - filter inverted indexes -
        if (filename_length > 3 && strncmp(filename,"ii_",3) == 0)
        {
          char *end_ptr;
          unsigned file_idx = strtoll(filename + 3,&end_ptr,10);
          if (end_ptr != filename + filename_length)
          {
            throw_error(IDB_DATABASE_CREATE_FILENAME_ERROR);
          }

          ui_tree_s_insert(&file_indexes,file_idx);
        }
      }
    } while(1);

    // - reconstruct inverted index tree -
    if (file_indexes.root_idx != c_idx_not_exist)
    {
      this->remove_tree_idxs.used = 0;
      unsigned next_tree_idx = 1;

      // - process file indexes -
      unsigned stack[RB_TREE_STACK_SIZE(ui_tree_s,&file_indexes)];
      unsigned *stack_ptr = stack;

      unsigned idx = ui_tree_s_get_stack_min_value_idx(&file_indexes,file_indexes.root_idx,&stack_ptr);
      do
      {
        unsigned file_idx = *ui_tree_s_at(&file_indexes,idx);

        // - skip unavailable tree indexes -
        while (file_idx > next_tree_idx)
        {
          CONT_INIT_CLEAR(idb_inverted_index_s,dummy_inverted_index);
          cassert(idb_inverted_index_tree_s_swap_insert(
                &this->inverted_index_tree,&dummy_inverted_index) == next_tree_idx);

          ui_array_s_push(&this->remove_tree_idxs,next_tree_idx);
          ++next_tree_idx;
        }

        // - retrieve inverted index -
        this->old_path.used = 0;
        bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_data.data,file_idx);

        CONT_INIT_CLEAR(idb_inverted_index_s,inverted_index);
        if (idb_database_s_map_index(this->old_path.data,&inverted_index))
        {
          throw_error(IDB_DATABASE_CREATE_MAP_ERROR);
        }

        cassert(idb_inverted_index_tree_s_swap_insert(
              &this->inverted_index_tree,&inverted_index) == file_idx);
        ++next_tree_idx;

        idx = ui_tree_s_get_stack_next_idx(&file_indexes,idx,&stack_ptr,stack);
      } while(idx != c_idx_not_exist);

      // - remove dummy inverted indexes -
      while (this->remove_tree_idxs.used != 0)
      {
        idb_inverted_index_tree_s_remove(
            &this->inverted_index_tree,ui_array_s_pop(&this->remove_tree_idxs));
      }
    }
  }

  return 0;
}/*}}}*/

int idb_database_s_update_extractor(idb_database_s *this,var_s a_data)
{/*{{{*/
  if (a_data->v_type != c_bi_type_dict)
  {
    throw_error(IDB_DATABASE_UPDATE_EXTRACTOR_INVALID_DATA);
  }

  CONT_INIT_CLEAR(reg_parser_s,reg_parser);
  this->states_array.used = 0;

  var_map_tree_s *data_tree = loc_s_dict_value(a_data);
  if (data_tree->root_idx != c_idx_not_exist)
  {
    var_map_tree_s_node *dt_ptr = data_tree->data;
    var_map_tree_s_node *dt_ptr_end = dt_ptr + data_tree->used;
    do
    {
      if (dt_ptr->valid)
      {
        unsigned type_id = var_tree_s_get_idx(&g_id_type_vars,&dt_ptr->object.value);

        if (type_id == c_idx_not_exist ||
            dt_ptr->object.key->v_type != c_bi_type_string ||
            !reg_parser_s_process_reg_exp(&reg_parser,loc_s_string_value(dt_ptr->object.key),type_id))
        {
          throw_error(IDB_DATABASE_UPDATE_EXTRACTOR_INVALID_DATA);
        }

        fa_states_array_s_push_blank(&this->states_array);
        fa_states_s_swap(fa_states_array_s_last(&this->states_array),&reg_parser.states);
      }
    } while(++dt_ptr < dt_ptr_end);
  }

  CONT_INIT_CLEAR(inverted_index_s,inverted_index);
  inverted_index_s_update(&inverted_index,&this->states_array);

  idb_inverted_index_s_clear(&this->extractor_index);

  {
    // - create inverted index dump -
    this->ii_dump.used = 0;
    inverted_index_s_dump(&inverted_index,&this->ii_dump);

    this->old_path.used = 0;
    bc_array_s_append_format(&this->old_path,"%s/extract",this->dir_remove.data);

    // - create inverted index mmap -
    CONT_INIT_CLEAR(fd_s,fd);
    if ((fd = open(this->old_path.data,O_RDWR | O_CREAT,0644)) == -1 ||
        fd_s_write(&fd,this->ii_dump.data,this->ii_dump.used*sizeof(ui)) ||
        mmap_s_create(&this->extractor_index.mmap,NULL,this->ii_dump.used*sizeof(ui),PROT_READ,MAP_SHARED,fd,0))
    {
      throw_error(IDB_DATABASE_UPDATE_EXTRACTOR_WRITE_ERROR);
    }
  }

  this->new_path.used = 0;
  bc_array_s_append_format(&this->new_path,"%s/extract",this->dir_data.data);

  // - rename inverted index file -
  if (rename(this->old_path.data,this->new_path.data) != 0)
  {
    throw_error(IDB_DATABASE_UPDATE_EXTRACTOR_RENAME_ERROR);
  }

  return 0;
}/*}}}*/

int idb_database_s_extract_regexps(idb_database_s *this,
    var_s a_doc,string_array_s *a_reg_exps)
{/*{{{*/
  if (this->extractor_index.mmap.address == NULL)
  {
    throw_error(IDB_DATABASE_EXTRACT_REGEXPS_EXTRACTOR_INDEX_MISSING);
  }

  // - extract regular expressions -
  CONT_INIT_CLEAR(var_queue_s,dict_queue);
  var_queue_s_insert(&dict_queue,&a_doc);

  this->buffer.used = 0;

  while (dict_queue.used != 0)
  {
    var_map_tree_s *dict_tree = loc_s_dict_value(*var_queue_s_next(&dict_queue));
    if (dict_tree->root_idx != c_idx_not_exist)
    {
      var_map_tree_s_node *dtn_ptr = dict_tree->data;
      var_map_tree_s_node *dtn_ptr_end = dtn_ptr + dict_tree->used;
      do
      {
        if (dtn_ptr->valid)
        {
          var_s key_var = dtn_ptr->object.key;
          var_s value_var = dtn_ptr->object.value;

          switch (value_var->v_type)
          {
          case c_bi_type_dict:
            {/*{{{*/
              var_queue_s_insert(&dict_queue,&value_var);
            }/*}}}*/
            break;
          case c_bi_type_array:
            {/*{{{*/
              var_array_s *value_arr = loc_s_array_value(value_var);
              if (value_arr->used != 0)
              {
                var_s *v_ptr = value_arr->data;
                var_s *v_ptr_end = v_ptr + value_arr->used;
                do
                {
                  if ((*v_ptr)->v_type == c_bi_type_dict)
                  {
                    var_queue_s_insert(&dict_queue,v_ptr);
                  }
                } while(++v_ptr < v_ptr_end);
              }
            }/*}}}*/
            break;
          default:
            {/*{{{*/
              const string_s *key_str = loc_s_string_value(key_var);

              unsigned input_idx = 0;
              unsigned target_offset = inverted_index_dump_s_recognize(
                  this->extractor_index.mmap.address,key_str->data,&input_idx,key_str->size - 1);

              if (target_offset != c_idx_not_exist)
              {
                unsigned *target_ptr = ((unsigned *)this->extractor_index.mmap.address) + target_offset;
                unsigned target_cnt = *target_ptr++;

                if (target_cnt != 0)
                {
                  unsigned *target_ptr_end = target_ptr + target_cnt;
                  do
                  {
                    switch (*target_ptr)
                    {
                    case c_idb_extracted_type_ID:
                      {/*{{{*/
                        switch (value_var->v_type)
                        {
                        case c_bi_type_integer:
                          {
                            string_array_s_push_blank(a_reg_exps);
                            string_s_set_format(string_array_s_last(a_reg_exps),
                                "\"%s\".w*.\"==\".w*.\"%lld\"",
                                key_str->data,loc_s_int_value(value_var));
                          }
                          break;
                        case c_bi_type_string:
                          {
                            string_array_s_push_blank(a_reg_exps);
                            string_s_set_format(string_array_s_last(a_reg_exps),
                                "\"%s\".w*.\"==\".w*.\"%s\"",
                                key_str->data,loc_s_string_value(value_var)->data);
                          }
                          break;
                        }
                      }/*}}}*/
                      break;
                    case c_idb_extracted_type_TEXT:
                      {/*{{{*/
                        if (value_var->v_type == c_bi_type_string)
                        {
                          const string_s *value_str = loc_s_string_value(value_var);
                          if (value_str->size > 1)
                          {
                            const char *ptr = value_str->data;
                            const char *ptr_end = ptr + value_str->size - 1;
                            do
                            {
                              if (isspace(*ptr) || ispunct(*ptr) || *ptr == '\0')
                              {
                                if (this->buffer.used != 0)
                                {
                                  string_array_s_push_blank(a_reg_exps);
                                  string_s_set_format(string_array_s_last(a_reg_exps),
                                      "\"%.*s\"",this->buffer.used,this->buffer.data);

                                  this->buffer.used = 0;
                                }

                                if (ptr >= ptr_end) { break; }
                              }
                              else
                              {
                                bc_array_s_push(&this->buffer,*ptr);
                              }

                              ++ptr;
                            } while(1);
                          }
                        }
                      }/*}}}*/
                      break;
                    default:
                      throw_error(IDB_DATABASE_EXTRACT_REGEXPS_UNKNOWN_EXTRACTED_TYPE);
                    }
                  } while(++target_ptr < target_ptr_end);
                }
              }
            }/*}}}*/
            break;
          }

        }
      } while(++dtn_ptr < dtn_ptr_end);
    }
  }

  return 0;
}/*}}}*/

int idb_database_s_dump_index(idb_database_s *this,
    inverted_index_s *a_inverted_index,unsigned *a_ii_index)
{/*{{{*/
  CONT_INIT_CLEAR(idb_inverted_index_s,inverted_index);

  {
    // - create inverted index dump -
    this->ii_dump.used = 0;
    inverted_index_s_dump(a_inverted_index,&this->ii_dump);

    this->old_path.used = 0;
    bc_array_s_append_format(&this->old_path,"%s/ii_new",this->dir_remove.data);

    // - create inverted index mmap -
    CONT_INIT_CLEAR(fd_s,fd);
    if ((fd = open(this->old_path.data,O_RDWR | O_CREAT,0644)) == -1 ||
        fd_s_write(&fd,this->ii_dump.data,this->ii_dump.used*sizeof(ui)) ||
        mmap_s_create(&inverted_index.mmap,NULL,this->ii_dump.used*sizeof(ui),PROT_READ,MAP_SHARED,fd,0))
    {
      throw_error(IDB_DATABASE_DUMP_INDEX_WRITE_ERROR);
    }
  }

  // - insert inverted index to tree -
  unsigned ii_index = idb_inverted_index_tree_s_swap_insert(
      &this->inverted_index_tree,&inverted_index);

  this->new_path.used = 0;
  bc_array_s_append_format(&this->new_path,"%s/ii_%u",this->dir_remove.data,ii_index);

  // - rename inverted index file -
  if (rename(this->old_path.data,this->new_path.data) != 0)
  {
    throw_error(IDB_DATABASE_DUMP_INDEX_RENAME_ERROR);
  }

  *a_ii_index = ii_index;

  return 0;
}/*}}}*/

int idb_database_s_map_index(const char *a_path,idb_inverted_index_s *a_inverted_index)
{/*{{{*/
  idb_inverted_index_s_clear(a_inverted_index);

  CONT_INIT_CLEAR(fd_s,fd);
  if ((fd = open(a_path,O_RDONLY,0)) == -1)
  {
    throw_error(IDB_DATABASE_MAP_INDEX_OPEN_ERROR);
  }

  off_t fsize;
  if ((fsize = lseek(fd,0,SEEK_END)) == -1 || lseek(fd,0,SEEK_SET) == -1)
  {
    throw_error(IDB_DATABASE_MAP_INDEX_SEEK_ERROR);
  }

  // - create inverted index mmap -
  if (mmap_s_create(&a_inverted_index->mmap,NULL,fsize,PROT_READ,MAP_SHARED,fd,0))
  {
    throw_error(IDB_DATABASE_MAP_INDEX_MAP_ERROR);
  }

  return 0;
}/*}}}*/

int idb_database_s_update_index(idb_database_s *this,var_s a_doc,unsigned a_doc_index)
{/*{{{*/
  if (a_doc->v_type != c_bi_type_dict)
  {
    throw_error(IDB_DATABASE_UPDATE_INDEX_INVALID_DATA);
  }

  // - extract regular expressions -
  this->reg_exps.used = 0;
  if (idb_database_s_extract_regexps(this,a_doc,&this->reg_exps))
  {
    throw_error(IDB_DATABASE_UPDATE_INDEX_EXTRACT_REGEXPS_ERROR);
  }

  // - update inverted index -
  if (this->reg_exps.used != 0)
  {
    CONT_INIT_CLEAR(reg_parser_s,reg_parser);
    this->states_array.used = 0;

    string_s *re_ptr = this->reg_exps.data;
    string_s *re_ptr_end = re_ptr + this->reg_exps.used;
    do
    {
      if (!reg_parser_s_process_reg_exp(&reg_parser,re_ptr,a_doc_index))
      {
        throw_error(IDB_DATABASE_UPDATE_INDEX_INVALID_REGEXP);
      }

      fa_states_array_s_push_blank(&this->states_array);
      fa_states_s_swap(fa_states_array_s_last(&this->states_array),&reg_parser.states);
    } while(++re_ptr < re_ptr_end);


    CONT_INIT_CLEAR(inverted_index_s,inverted_index);
    inverted_index_s_update(&inverted_index,&this->states_array);

    unsigned ii_index;
    if (idb_database_s_dump_index(this,&inverted_index,&ii_index))
    {
      throw_error(IDB_DATABASE_UPDATE_INDEX_DUMP_ERROR);
    }

    // - move index from remove to data -
    this->old_path.used = 0;
    bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_remove.data,ii_index);

    this->new_path.used = 0;
    bc_array_s_append_format(&this->new_path,"%s/ii_%u",this->dir_data.data,ii_index);

    if (rename(this->old_path.data,this->new_path.data) != 0)
    {
      throw_error(IDB_DATABASE_UPDATE_INDEX_RENAME_ERROR);
    }
  }

  return 0;
}/*}}}*/

int idb_database_s_merge_index(idb_database_s *this)
{/*{{{*/
  if (this->inverted_index_tree.count > this->indexes_max)
  {
    CONT_INIT_CLEAR(inverted_indexes_s,merge_indexes);

    do
    {
      merge_indexes.used = 0;
      this->remove_tree_idxs.used = 0;

      unsigned ii_idx = idb_inverted_index_tree_s_get_min_value_idx(&this->inverted_index_tree,this->inverted_index_tree.root_idx);
      unsigned merge_cnt = 0;

      do {
        unsigned next_ii_idx = idb_inverted_index_tree_s_get_next_idx(&this->inverted_index_tree,ii_idx);

        inverted_indexes_s_push_blank(&merge_indexes);
        if (inverted_index_s_load(inverted_indexes_s_last(&merge_indexes),
              idb_inverted_index_tree_s_at(&this->inverted_index_tree,ii_idx)->mmap.address))
        {
          throw_error(IDB_DATABASE_MERGE_INDEX_LOAD_ERROR);
        }

        ui_array_s_push(&this->remove_tree_idxs,ii_idx);

        // - move merged index to keep directory -
        this->old_path.used = 0;
        bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_data.data,ii_idx);

        this->new_path.used = 0;
        bc_array_s_append_format(&this->new_path,"%s/ii_%u",this->dir_keep.data,ii_idx);

        if (rename(this->old_path.data,this->new_path.data) != 0)
        {
          throw_error(IDB_DATABASE_MERGE_INDEX_RENAME_ERROR);
        }

        ii_idx = next_ii_idx;
      } while(++merge_cnt < this->merge_cnt);

      CONT_INIT_CLEAR(inverted_index_s,inverted_index);
      inverted_indexes_s_merge(&merge_indexes,&inverted_index);

      unsigned ii_index;
      if (idb_database_s_dump_index(this,&inverted_index,&ii_index))
      {
        throw_error(IDB_DATABASE_MERGE_INDEX_DUMP_ERROR);
      }

      // - swap keep and remove directories -
      if (renameat2(AT_FDCWD,this->dir_keep.data,AT_FDCWD,this->dir_remove.data,RENAME_EXCHANGE) != 0)
      {
        throw_error(IDB_DATABASE_MERGE_INDEX_SWAP_ERROR);
      }

      // - clean keep directory -
      this->old_path.used = 0;
      bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_keep.data,ii_index);

      this->new_path.used = 0;
      bc_array_s_append_format(&this->new_path,"%s/ii_%u",this->dir_data.data,ii_index);

      if (rename(this->old_path.data,this->new_path.data) != 0)
      {
        throw_error(IDB_DATABASE_MERGE_INDEX_RENAME_ERROR);
      }

      // - clean remove directory -
      while (this->remove_tree_idxs.used != 0)
      {
        unsigned ii_idx = ui_array_s_pop(&this->remove_tree_idxs);

        // - remove inverted index from database -
        idb_inverted_index_tree_s_remove(&this->inverted_index_tree,ii_idx);

        // - remove inverted index file -
        this->old_path.used = 0;
        bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_remove.data,ii_idx);

        if (remove(this->old_path.data) != 0)
        {
          throw_error(IDB_DATABASE_MERGE_INDEX_REMOVE_ERROR);
        }
      }

    } while(this->inverted_index_tree.count > this->indexes_max);
  }

  return 0;
}/*}}}*/

int idb_database_s_remove_docs(idb_database_s *this,ui_array_s *a_doc_indexes)
{/*{{{*/
  if (a_doc_indexes->used == 0)
  {
    return 0;
  }

  if (this->inverted_index_tree.root_idx != c_idx_not_exist)
  {
    this->remove_tree_idxs.used = 0;
    this->keep_tree_idxs.used = 0;

    idb_inverted_index_tree_s_node *iitn_ptr = this->inverted_index_tree.data;
    idb_inverted_index_tree_s_node *iitn_ptr_end = iitn_ptr + this->inverted_index_tree.used;
    do
    {
      if (iitn_ptr->valid)
      {
        unsigned targets_offset = ((unsigned *)iitn_ptr->object.mmap.address)[c_ii_dump_offset_to_target_list_offset];
        unsigned *targets_ptr = ((unsigned *)iitn_ptr->object.mmap.address) + targets_offset;
        unsigned targets_cnt = *targets_ptr++;

        unsigned *di_ptr = a_doc_indexes->data;
        unsigned *di_ptr_end = di_ptr + a_doc_indexes->used;
        do
        {
          if (ui_binary_search(targets_ptr,targets_cnt,*di_ptr) != c_idx_not_exist)
          {
            break;
          }
        } while(++di_ptr < di_ptr_end);

        // - inverted index contains some document index -
        if (di_ptr < di_ptr_end)
        {
          unsigned ii_idx = iitn_ptr - this->inverted_index_tree.data;

          CONT_INIT_CLEAR(inverted_index_s,inverted_index);
          if (inverted_index_s_load(&inverted_index,
                idb_inverted_index_tree_s_at(&this->inverted_index_tree,ii_idx)->mmap.address))
          {
            throw_error(IDB_DATABASE_REMOVE_DOCS_LOAD_ERROR);
          }

          ui_array_s_push(&this->remove_tree_idxs,ii_idx);

          // - move index to keep directory -
          this->old_path.used = 0;
          bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_data.data,ii_idx);

          this->new_path.used = 0;
          bc_array_s_append_format(&this->new_path,"%s/ii_%u",this->dir_keep.data,ii_idx);

          if (rename(this->old_path.data,this->new_path.data) != 0)
          {
            throw_error(IDB_DATABASE_REMOVE_DOCS_RENAME_ERROR);
          }

          inverted_index_s_remove_targets(&inverted_index,a_doc_indexes);

          // - some states of inverted index remained -
          if (inverted_index.states.used != 0)
          {
            unsigned ii_index;
            if (idb_database_s_dump_index(this,&inverted_index,&ii_index))
            {
              throw_error(IDB_DATABASE_REMOVE_DOCS_DUMP_ERROR);
            }

            ui_array_s_push(&this->keep_tree_idxs,ii_index);
          }
        }
      }
    } while(++iitn_ptr < iitn_ptr_end);

    if (this->remove_tree_idxs.used != 0)
    {
      // - swap keep and remove directories -
      if (renameat2(AT_FDCWD,this->dir_keep.data,AT_FDCWD,this->dir_remove.data,RENAME_EXCHANGE) != 0)
      {
        throw_error(IDB_DATABASE_REMOVE_DOCS_SWAP_ERROR);
      }

      // - clean keep directory-
      while (this->keep_tree_idxs.used != 0)
      {
        unsigned ii_idx = ui_array_s_pop(&this->keep_tree_idxs);

        // - move inverted index from keep to data -
        this->old_path.used = 0;
        bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_keep.data,ii_idx);

        this->new_path.used = 0;
        bc_array_s_append_format(&this->new_path,"%s/ii_%u",this->dir_data.data,ii_idx);

        if (rename(this->old_path.data,this->new_path.data) != 0)
        {
          throw_error(IDB_DATABASE_REMOVE_DOCS_RENAME_ERROR);
        }
      }

      // - clean remove directory -
      while (this->remove_tree_idxs.used != 0)
      {
        unsigned ii_idx = ui_array_s_pop(&this->remove_tree_idxs);

        // - remove inverted index from database -
        idb_inverted_index_tree_s_remove(&this->inverted_index_tree,ii_idx);

        // - remove inverted index file -
        this->old_path.used = 0;
        bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_remove.data,ii_idx);

        if (remove(this->old_path.data) != 0)
        {
          throw_error(IDB_DATABASE_REMOVE_DOCS_REMOVE_ERROR);
        }
      }
    }
  }

  return 0;
}/*}}}*/

void idb_database_s_query(idb_database_s *this,const string_s *a_query)
{/*{{{*/

  // - reset query result -
  ui_tree_s_clear(&this->query_res);

  if (a_query->size > 1)
  {
    if (this->inverted_index_tree.root_idx != c_idx_not_exist)
    {
      idb_inverted_index_tree_s_node *iitn_ptr = this->inverted_index_tree.data;
      idb_inverted_index_tree_s_node *iitn_ptr_end = iitn_ptr + this->inverted_index_tree.used;
      do
      {
        if (iitn_ptr->valid)
        {
          idb_inverted_index_s *ii_ptr = &iitn_ptr->object;

          CONT_INIT_CLEAR(ui_tree_s,ip_query_res);
          int ip_query_res_set = 1;

          // - traverse query string -
          unsigned input_idx = 0;
          while (isspace(a_query->data[input_idx])) { ++input_idx; }

          do
          {
            unsigned target_offset = inverted_index_dump_s_recognize(
                ii_ptr->mmap.address,a_query->data,&input_idx,a_query->size - 1);

            if (target_offset == c_idx_not_exist)
            {
              ui_tree_s_clear(&ip_query_res);
              break;
            }

            unsigned *target_ptr = ((unsigned *)ii_ptr->mmap.address) + target_offset;
            unsigned target_cnt = *target_ptr++;
            debug_assert(target_cnt > 0);

            if (ip_query_res_set)
            {
              // - set ip query result to target -
              unsigned *target_ptr_end = target_ptr + target_cnt;
              do {
                ui_tree_s_insert(&ip_query_res,*target_ptr);
              } while(++target_ptr < target_ptr_end);

              ip_query_res_set = 0;
            }
            else
            {
              // - intersection of ip query result and target -
              unsigned ipqrtn_idx = 0;
              do {
                ui_tree_s_node *ipqrtn_ptr = ip_query_res.data + ipqrtn_idx;

                if (ipqrtn_ptr->valid)
                {
                  if (ui_binary_search(target_ptr,target_cnt,ipqrtn_ptr->object) == c_idx_not_exist)
                  {
                    ui_tree_s_remove(&ip_query_res,ipqrtn_idx);
                  }
                }
              } while(++ipqrtn_idx < ip_query_res.used);

              if (ip_query_res.root_idx == c_idx_not_exist)
              {
                break;
              }
            }

            while (isspace(a_query->data[input_idx])) { ++input_idx; }
          } while(input_idx < a_query->size - 1);

          if (ip_query_res.root_idx != c_idx_not_exist)
          {
            // - union of ip query result and query result -
            ui_tree_s_node *ipqrtn_ptr = ip_query_res.data;
            ui_tree_s_node *ipqrtn_ptr_end = ipqrtn_ptr + ip_query_res.used;
            do
            {
              if (ipqrtn_ptr->valid)
              {
                ui_tree_s_unique_insert(&this->query_res,ipqrtn_ptr->object);
              }
            } while(++ipqrtn_ptr < ipqrtn_ptr_end);
          }
        }
      } while(++iitn_ptr < iitn_ptr_end);
    }
  }
}/*}}}*/

// -- idb_database_tree_s --
@begin
methods idb_database_tree_s
@end

// === global functions ========================================================

void libidb_idl_init()
{/*{{{*/
  var_tree_s_init(&g_id_type_vars);

  const char **s_ptr = g_id_type_strings;
  const char **s_ptr_end = (void *)s_ptr + sizeof(g_id_type_strings);
  do {
    VAR_CLEAR(value,loc_s_string_ptr(*s_ptr));
    var_tree_s_insert(&g_id_type_vars,&value);
  } while(++s_ptr < s_ptr_end);
}/*}}}*/

void libidb_idl_clear()
{/*{{{*/
  var_tree_s_clear(&g_id_type_vars);
}/*}}}*/

