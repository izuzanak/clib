
@begin
include "id_idb.h"
@end

// === constants and definitions ===============================================

const char *g_id_type_strings[] =
{/*{{{*/
  "id",
  "text",
  "cpid",
};/*}}}*/

var_tree_s g_id_type_vars;

// === methods of generated structures =========================================

// -- idb_bits_s --
@begin
methods idb_bits_s
@end

// -- idb_bits_tree_s --
@begin
methods idb_bits_tree_s
@end

void idb_bits_tree_s_to_query_res(idb_bits_tree_s *this,ui_array_s *a_trg)
{/*{{{*/
  a_trg->used = 0;

  if (this->root_idx != c_idx_not_exist)
  {
    unsigned stack[RB_TREE_STACK_SIZE(idb_bits_tree_s,this)];
    unsigned *stack_ptr = stack;

    unsigned br_idx = idb_bits_tree_s_get_stack_min_value_idx(this,this->root_idx,&stack_ptr);
    do {
      idb_bits_s *idb_bits = idb_bits_tree_s_at(this,br_idx);
      unsigned value = idb_bits->pos;

      ulli *bits_ptr = &idb_bits->bits0;
      ulli *bits_ptr_end = bits_ptr + IDB_BITS_SLOT_COUNT;
      do {
        ulli bit_mask = 1;
        do {
          if (*bits_ptr & bit_mask)
          {
            ui_array_s_push(a_trg,value);
          }
        } while(++value,bit_mask <<= 1);
      } while(++bits_ptr < bits_ptr_end);

      br_idx = idb_bits_tree_s_get_stack_next_idx(this,br_idx,&stack_ptr,stack);
    } while(br_idx != c_idx_not_exist);
  }
}/*}}}*/

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

                        // - normalize key string -
                        this->utf8_buffer.used = 0;

                        if (key_str->size > 1)
                        {
                          const char *ks_ptr = key_str->data;
                          const char *ks_ptr_end = ks_ptr + key_str->size - 1;
                          do {
                            if (
                                *ks_ptr == '-' ||
                                *ks_ptr == '_' ||
                                *ks_ptr == '.' ||
                                *ks_ptr == ',')
                            {
                              bc_array_s_push(&this->utf8_buffer,' ');
                            }
                            else
                            {
                              bc_array_s_push(&this->utf8_buffer,*ks_ptr);
                            }
                          } while(++ks_ptr < ks_ptr_end);
                        }

                        switch (value_var->v_type)
                        {
                        case c_bi_type_integer:
                          {
                            string_array_s_push_blank(a_reg_exps);
                            string_s_set_format(string_array_s_last(a_reg_exps),
                                "\"%.*s\".w*.\"==\".w*.\"%lld\"",
                                this->utf8_buffer.used,this->utf8_buffer.data,
                                loc_s_int_value(value_var));
                          }
                          break;
                        case c_bi_type_string:
                          {
                            string_array_s_push_blank(a_reg_exps);
                            string_s_set_format(string_array_s_last(a_reg_exps),
                                "\"%.*s\".w*.\"==\".w*.\"%s\"",
                                this->utf8_buffer.used,this->utf8_buffer.data,
                                loc_s_string_value(value_var)->data);
                          }
                          break;
                        }
                      }/*}}}*/
                      break;
                    case c_idb_extracted_type_TEXT:
                    case c_idb_extracted_type_CPID:
                      {/*{{{*/
                        if (value_var->v_type == c_bi_type_string)
                        {
                          const string_s *value_str = loc_s_string_value(value_var);

                          // - normalize text string -
                          this->utf8_buffer.used = 0;
                          if (utf8proc_s_map(value_str->size,value_str->data,
                                UTF8PROC_STABLE | UTF8PROC_STRIPMARK | UTF8PROC_DECOMPOSE |
                                UTF8PROC_CASEFOLD,&this->utf8_buffer))
                          {
                            throw_error(IDB_DATABASE_EXTRACT_REGEXPS_UTF8_ERROR);
                          }

                          if (this->utf8_buffer.used > 1)
                          {
                            const char *ptr = this->utf8_buffer.data;
                            const char *ptr_end = ptr + this->utf8_buffer.used - 1;
                            do
                            {
                              if (isspace(*ptr) ||
                                  *ptr == '-' ||
                                  *ptr == '_' ||
                                  *ptr == '.' ||
                                  *ptr == ',' ||
                                  *ptr == '\0')
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
                              else if (ispunct(*ptr)) {}
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

                    // - cpid text extra processing -
                    if (*target_ptr == c_idb_extracted_type_CPID)
                    {
                      string_s *cpid_numeric = string_array_s_last(a_reg_exps);

                      // - check cpid numeric size -
                      if (cpid_numeric->size - 1 == 15)
                      {
                        char *cpid_numeric_data = cpid_numeric->data + 1;

                        string_array_s_push_blank(a_reg_exps);
                        string_s_set_format(string_array_s_last(a_reg_exps),
                            "\"%.*s\"",12,cpid_numeric_data);

                        string_array_s_push_blank(a_reg_exps);
                        string_s_set_format(string_array_s_last(a_reg_exps),
                            "\"%.*s\"",3,cpid_numeric_data + 5);

                        string_array_s_push_blank(a_reg_exps);
                        string_s_set_format(string_array_s_last(a_reg_exps),
                            "\"%.*s\"",6,cpid_numeric_data + 5);
                      }
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

int idb_database_s_update_indexes(idb_database_s *this,
    var_array_s *a_docs,const ui_array_s *a_doc_ids)
{/*{{{*/
  if (a_docs->used != a_doc_ids->used)
  {
    throw_error(IDB_DATABASE_UPDATE_INDEXES_INVALID_DATA);
  }

  if (a_docs->used == 0)
  {
    return 0;
  }

  CONT_INIT_CLEAR(reg_parser_s,reg_parser);
  this->states_array.used = 0;

  unsigned idx = 0;
  do {
    var_s doc = *var_array_s_at(a_docs,idx);
    unsigned doc_id = *ui_array_s_at(a_doc_ids,idx);

    if (doc == NULL || doc->v_type != c_bi_type_dict)
    {
      throw_error(IDB_DATABASE_UPDATE_INDEXES_INVALID_DATA);
    }

    // - extract regular expressions -
    this->reg_exps.used = 0;
    if (idb_database_s_extract_regexps(this,doc,&this->reg_exps))
    {
      throw_error(IDB_DATABASE_UPDATE_INDEXES_EXTRACT_REGEXPS_ERROR);
    }

    // - update inverted index -
    if (this->reg_exps.used != 0)
    {
      string_s *re_ptr = this->reg_exps.data;
      string_s *re_ptr_end = re_ptr + this->reg_exps.used;
      do
      {
        if (!reg_parser_s_process_reg_exp(&reg_parser,re_ptr,doc_id))
        {
          throw_error(IDB_DATABASE_UPDATE_INDEXES_INVALID_REGEXP);
        }

        fa_states_array_s_push_blank(&this->states_array);
        fa_states_s_swap(fa_states_array_s_last(&this->states_array),&reg_parser.states);
      } while(++re_ptr < re_ptr_end);
    }
  } while(++idx < a_docs->used);

  // - update inverted index -
  if (this->states_array.used != 0)
  {
    CONT_INIT_CLEAR(inverted_index_s,inverted_index);
    inverted_index_s_update(&inverted_index,&this->states_array);

    unsigned ii_index;
    if (idb_database_s_dump_index(this,&inverted_index,&ii_index))
    {
      throw_error(IDB_DATABASE_UPDATE_INDEXES_DUMP_ERROR);
    }

    // - move index from remove to data -
    this->old_path.used = 0;
    bc_array_s_append_format(&this->old_path,"%s/ii_%u",this->dir_remove.data,ii_index);

    this->new_path.used = 0;
    bc_array_s_append_format(&this->new_path,"%s/ii_%u",this->dir_data.data,ii_index);

    if (rename(this->old_path.data,this->new_path.data) != 0)
    {
      throw_error(IDB_DATABASE_UPDATE_INDEXES_RENAME_ERROR);
    }
  }

  return 0;
}/*}}}*/

int idb_database_s_merge_indexes(idb_database_s *this)
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

int idb_database_s_remove_docs(idb_database_s *this,const ui_array_s *a_doc_ids)
{/*{{{*/
  if (a_doc_ids->used == 0)
  {
    return 0;
  }

  if (this->inverted_index_tree.root_idx != c_idx_not_exist)
  {
    this->remove_tree_idxs.used = 0;
    this->keep_tree_idxs.used = 0;

    unsigned iitn_idx = 0;
    do
    {
      idb_inverted_index_tree_s_node *iitn_ptr = this->inverted_index_tree.data + iitn_idx;

      if (iitn_ptr->valid)
      {
        unsigned targets_offset = ((unsigned *)iitn_ptr->object.mmap.address)[c_ii_dump_offset_to_target_list_offset];
        unsigned *targets_ptr = ((unsigned *)iitn_ptr->object.mmap.address) + targets_offset;
        unsigned targets_cnt = *targets_ptr++;

        unsigned *di_ptr = a_doc_ids->data;
        unsigned *di_ptr_end = di_ptr + a_doc_ids->used;
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

          inverted_index_s_remove_targets(&inverted_index,a_doc_ids);

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
    } while(++iitn_idx < this->inverted_index_tree.used);

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

int idb_database_s_query(idb_database_s *this,const string_s *a_query)
{/*{{{*/

  // - reset query result -
  idb_bits_tree_s_clear(&this->bits_res);

  if (a_query->size > 1)
  {
    if (this->inverted_index_tree.root_idx != c_idx_not_exist)
    {
      // - normalize query string -
      this->utf8_buffer.used = 0;
      if (utf8proc_s_map(a_query->size - 1,a_query->data,
            UTF8PROC_STABLE | UTF8PROC_STRIPMARK | UTF8PROC_DECOMPOSE |
            UTF8PROC_CASEFOLD,&this->utf8_buffer))
      {
        throw_error(IDB_DATABASE_EXTRACT_REGEXPS_UTF8_ERROR);
      }

      // - modify query string -
      if (this->utf8_buffer.used != 0)
      {
        char *s_ptr = this->utf8_buffer.data;
        char *s_ptr_end = s_ptr + this->utf8_buffer.used;
        char *t_ptr = this->utf8_buffer.data;
        do
        {
          if (
            *s_ptr == '-' ||
            *s_ptr == '_' ||
            *s_ptr == '.' ||
            *s_ptr == ',')
          {
            *t_ptr++ = ' ';
          }
          else if (*s_ptr == '=')
          {
            *t_ptr++ = '=';
          }
          else if (ispunct(*s_ptr)) {}
          else
          {
            *t_ptr++ = *s_ptr;
          }
        } while(++s_ptr < s_ptr_end);

        // - adjust buffer size -
        this->utf8_buffer.used = t_ptr - this->utf8_buffer.data;
      }

      idb_inverted_index_tree_s_node *iitn_ptr = this->inverted_index_tree.data;
      idb_inverted_index_tree_s_node *iitn_ptr_end = iitn_ptr + this->inverted_index_tree.used;
      do
      {
        if (iitn_ptr->valid)
        {
          idb_inverted_index_s *ii_ptr = &iitn_ptr->object;

          CONT_INIT_CLEAR(idb_bits_tree_s,ip_bits_res);
          int ip_bits_res_set = 1;

          // - traverse query string -
          unsigned input_idx = 0;
          do
          {
            while (isspace(this->utf8_buffer.data[input_idx])) { ++input_idx; }

            unsigned target_offset = inverted_index_dump_s_recognize(
                ii_ptr->mmap.address,this->utf8_buffer.data,&input_idx,this->utf8_buffer.used);

            if (target_offset == c_idx_not_exist)
            {
              idb_bits_tree_s_clear(&ip_bits_res);
              break;
            }

            unsigned *target_ptr = ((unsigned *)ii_ptr->mmap.address) + target_offset;
            unsigned target_cnt = *target_ptr++;
            debug_assert(target_cnt > 0);

            if (ip_bits_res_set)
            {
              // - set ip query result to target -
              unsigned *target_ptr_end = target_ptr + target_cnt;
              do {
                unsigned target = *target_ptr;
                unsigned target_pos = IDB_BITS_TARGET_POS(target);

                // - find or create bitset -
                idb_bits_s search_bits = {target_pos,0,0,0,0,0,0,0,0};
                unsigned bits_index = idb_bits_tree_s_get_idx(&ip_bits_res,&search_bits);
                if (bits_index == c_idx_not_exist)
                {
                  bits_index = idb_bits_tree_s_insert(&ip_bits_res,&search_bits);
                }

                ulli *bits_ptr = &idb_bits_tree_s_at(&ip_bits_res,bits_index)->bits0;

                // - process all targets of bitset -
                unsigned target_end = target_pos + IDB_BITS_BITCOUNT;
                do {
                  bits_ptr[IDB_BITS_TARGET_IDX(target)] |= 1ULL << IDB_BITS_TARGET_OFF(target);
                } while(++target_ptr < target_ptr_end && (target = *target_ptr) < target_end);

              } while(target_ptr < target_ptr_end);

              ip_bits_res_set = 0;
            }
            else
            {
              // - intersection of ip bits result and target -
              unsigned ipbrtn_idx = 0;
              do {
                idb_bits_tree_s_node *ipbrtn_ptr = ip_bits_res.data + ipbrtn_idx;
                if (ipbrtn_ptr->valid)
                {
                  // - remove bitset flag -
                  int remove_bits = 1;
                  unsigned value = ipbrtn_ptr->object.pos;

                  // - check all slots -
                  ulli *bits_ptr = &ipbrtn_ptr->object.bits0;
                  ulli *bits_ptr_end = bits_ptr + IDB_BITS_SLOT_COUNT;
                  do
                  {
                    // - check all bits of slot -
                    ulli bit_mask = 1;
                    do
                    {
                      if (*bits_ptr & bit_mask)
                      {
                        // - if bit is set check presence of value in target -
                        if (ui_binary_search(target_ptr,target_cnt,value) == c_idx_not_exist)
                        {
                          *bits_ptr &= ~bit_mask;
                        }
                        else
                        {
                          remove_bits = 0;
                        }
                      }
                    } while(++value,bit_mask <<= 1);
                  } while(++bits_ptr < bits_ptr_end);

                  // - remove bitset flag is set -
                  if (remove_bits)
                  {
                    idb_bits_tree_s_remove(&ip_bits_res,ipbrtn_idx);
                  }
                }
              } while(++ipbrtn_idx < ip_bits_res.used);

              if (ip_bits_res.root_idx == c_idx_not_exist)
              {
                break;
              }
            }
          } while(input_idx < this->utf8_buffer.used);

          if (ip_bits_res.root_idx != c_idx_not_exist)
          {
            // - union of ip query result and query result -
            unsigned ipbrtn_idx = 0;
            do {
              idb_bits_tree_s_node *ipbrtn_ptr = ip_bits_res.data + ipbrtn_idx;
              if (ipbrtn_ptr->valid)
              {
                unsigned bits_index = idb_bits_tree_s_get_idx(&this->bits_res,&ipbrtn_ptr->object);
                if (bits_index == c_idx_not_exist)
                {
                  // - insert new bitset -
                  idb_bits_tree_s_insert(&this->bits_res,&ipbrtn_ptr->object);
                }
                else
                {
                  // - update bitset target from source -
                  ulli *s_bits_ptr = &ipbrtn_ptr->object.bits0;
                  ulli *s_bits_ptr_end = s_bits_ptr + IDB_BITS_SLOT_COUNT;
                  ulli *t_bits_ptr = &idb_bits_tree_s_at(&this->bits_res,bits_index)->bits0;
                  do {
                    *t_bits_ptr |= *s_bits_ptr;
                  } while(++t_bits_ptr,++s_bits_ptr < s_bits_ptr_end);
                }
              }
            } while(++ipbrtn_idx < ip_bits_res.used);
          }
        }
      } while(++iitn_ptr < iitn_ptr_end);
    }
  }

  return 0;
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

