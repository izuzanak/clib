
@begin
include "cl_xml.h"
@end

// === constants and definitions ===============================================

var_s g_str_empty_var = NULL;
var_s g_str_attrs_var = NULL;
var_s g_str_nodes_var = NULL;
var_s g_str_text_var = NULL;

// === methods of generated structures =========================================

// -- string_map_s --
@begin
methods string_map_s
@end

// -- string_map_tree_s --
@begin
methods string_map_tree_s
@end

// -- xml_create_stack_element_s --
@begin
methods xml_create_stack_element_s
@end

// -- xml_create_stack_s --
@begin
methods xml_create_stack_s
@end

// -- xml_node_s --
@begin
methods xml_node_s
@end

unsigned g_type_xml_node = c_idx_not_exist;

void xml_node_s___add_node_to_node_dict(var_map_tree_s *a_tree,var_s a_name,var_s a_node)
{/*{{{*/

  // - insert name and node to dictionary -
  var_map_s insert_map = {a_name,a_node};
  unsigned index = var_map_tree_s_unique_insert(a_tree,&insert_map);

  var_map_s *map = &(a_tree->data + index)->object;

  if (map->value != a_node)
  {
    if (map->value->v_type == c_bi_type_array)
    {
      loc_s_array_push(map->value,a_node);
    }
    else
    {
      var_s array_var = loc_s_array();

      loc_s_array_push(array_var,map->value);
      loc_s_array_push(array_var,a_node);

      var_s_copy_loc(&map->value,array_var);
    }
  }
}/*}}}*/

void xml_node_s_update_node_dict(var_s this)
{/*{{{*/
  CONT_INIT(pointer_array_s,node_stack);

  // - push node to node stack -
  pointer_array_s_push(&node_stack,loc_s_xml_node_value(this));

  do {
    xml_node_s *node = pointer_array_s_pop(&node_stack);

    // - create node dictionary -
    var_s dict_var = loc_s_dict();
    var_map_tree_s *tree = loc_s_dict_value(dict_var);

    // - replace old dictionary -
    var_s_copy_loc(&node->node_dict,dict_var);

    // - contains another nodes -
    if (node->nodes != NULL)
    {
      var_array_s *array = loc_s_array_value(node->nodes);

      // - process all nodes -
      if (array->used != 0)
      {
        var_s *n_ptr = array->data;
        var_s *n_ptr_end = n_ptr + array->used;
        do {
          xml_node_s *node = loc_s_xml_node_value(*n_ptr);

          // - add node to node dictionary -
          xml_node_s___add_node_to_node_dict(tree,node->name,*n_ptr);

          // - push node to node stack -
          pointer_array_s_push(&node_stack,node);

        } while(++n_ptr < n_ptr_end);
      }
    }

  } while(node_stack.used > 0);

  pointer_array_s_clear(&node_stack);
}/*}}}*/

int xml_node_s___node_dict_to_var(var_s this,var_s *a_trg) // NOLINT(misc-no-recursion)
{/*{{{*/
  xml_node_s *node = loc_s_xml_node_value(this);

  // - create node dictionary -
  var_s_copy_loc(a_trg,loc_s_dict());

  // - has nodes flag -
  int has_nodes = 0;

  // - process attributes -
  if (node->attributes != NULL)
  {
    loc_s_dict_set(*a_trg,g_str_attrs_var,node->attributes);
  }

  // - process nodes -
  if (node->node_dict != NULL)
  {
    var_map_tree_s *node_tree = loc_s_dict_value(node->node_dict);
    if (node_tree->count != 0)
    {
      has_nodes = 1;

      var_s nodes_var = loc_s_dict();
      loc_s_dict_set(*a_trg,g_str_nodes_var,nodes_var);

      var_map_tree_s_node *ptr = node_tree->data;
      var_map_tree_s_node *ptr_end = ptr + node_tree->used;
      do
      {
        if (ptr->valid)
        {
          CONT_INIT_CLEAR(var_s,item_var);

          // - process single node -
          if (ptr->object.value->v_type == g_type_xml_node)
          {/*{{{*/
            if (xml_node_s___node_dict_to_var(ptr->object.value,&item_var))
            {
              throw_error(XML_NODE_NODE_DICT_TO_VAR_ERROR);
            }
          }/*}}}*/

          // - process array of nodes -
          else if (ptr->object.value->v_type == c_bi_type_array)
          {/*{{{*/
            var_array_s *array = loc_s_array_value(ptr->object.value);

            var_s_copy_loc(&item_var,loc_s_array());

            if (array->used != 0)
            {
              var_s *ptr = array->data;
              var_s *ptr_end = ptr + array->used;
              do
              {
                CONT_INIT_CLEAR(var_s,node_var);
                if (xml_node_s___node_dict_to_var(*ptr,&node_var))
                {
                  throw_error(XML_NODE_NODE_DICT_TO_VAR_ERROR);
                }

                loc_s_array_push(item_var,node_var);
              } while(++ptr < ptr_end);
            }
          }/*}}}*/
          else
          {
            throw_error(XML_NODE_NODE_VALUE_TYPE_ERROR);
          }

          loc_s_dict_set(nodes_var,ptr->object.key,item_var);
        }
      } while(++ptr < ptr_end);
    }
  }

  // - process texts -
  if (!has_nodes)
  {
    CONT_INIT_CLEAR(var_s,value_var);

    if (node->texts != NULL)
    {
      switch (node->texts->v_type)
      {
      case c_bi_type_string:
        {/*{{{*/
          var_s_copy_loc(&value_var,node->texts);
        }/*}}}*/
        break;
      case c_bi_type_array:
        {/*{{{*/
          CONT_INIT(bc_array_s,buffer);

          var_array_s *array = loc_s_array_value(node->texts);

          if (array->used != 0)
          {
            var_s *ptr = array->data;
            var_s *ptr_end = ptr + array->used;
            do
            {
              if ((*ptr)->v_type == c_bi_type_string)
              {
                const string_s *string = loc_s_string_value(*ptr);
                bc_array_s_append(&buffer,string->size - 1,string->data);
              }
              else
              {
                bc_array_s_clear(&buffer);

                throw_error(XML_NODE_NODE_VALUE_TYPE_ERROR);
              }
            } while(++ptr < ptr_end);
          }

          bc_array_s_push(&buffer,'\0');

          var_s_copy_loc(&value_var,loc_s_string_ptr(""));
          string_s *string = (string_s *)loc_s_string_value(value_var);
          string->size = buffer.used;
          string->data = buffer.data;
        }/*}}}*/
        break;
      default:
        throw_error(XML_NODE_NODE_VALUE_TYPE_ERROR);
      }
    }
    else
    {
      var_s_copy_loc(&value_var,g_str_empty_var);
    }

    loc_s_dict_set(*a_trg,g_str_text_var,value_var);
  }

  return 0;
}/*}}}*/

int xml_node_s_node_dict_to_var(var_s this,var_s *a_trg)
{/*{{{*/
  if (this->v_type != g_type_xml_node)
  {
    throw_error(XML_NODE_NODE_VALUE_TYPE_ERROR);
  }

  var_s_copy_loc(a_trg,loc_s_dict());

  CONT_INIT_CLEAR(var_s,node_var);
  if (xml_node_s___node_dict_to_var(this,&node_var))
  {
    throw_error(XML_NODE_NODE_DICT_TO_VAR_ERROR);
  }

  loc_s_dict_set(*a_trg,loc_s_xml_node_value(this)->name,node_var);

  return 0;
}/*}}}*/

// -- xml_parser_s --
@begin
methods xml_parser_s
@end

void xml_parser_s_start_element(void *user,const xmlChar *name,const xmlChar **a_attrs)
{/*{{{*/
  (void)name;

  xml_parser_s *parser = (xml_parser_s *)user;
  var_array_s *ns_stack = &parser->ns_stack;
  var_array_s *attrs_stack = &parser->attrs_stack;
  var_arrays_s *node_array_stack = &parser->node_array_stack;
  var_arrays_s *text_array_stack = &parser->text_array_stack;
  var_arrays_s *cont_array_stack = &parser->cont_array_stack;

  // -----

  unsigned ns_stack_used = ns_stack->used;

  if (a_attrs != NULL)
  {
    var_s dict_var = loc_s_dict();
    var_map_tree_s *tree = loc_s_dict_value(dict_var);

    const char **attrs = (const char **)a_attrs;
    while (*attrs != NULL)
    {
      const char *attr_name = attrs[0];
      const char *attr_value = attrs[1];

      var_s attr_name_var = NULL;

      // - namespace attribute -
      if (strncmp("xmlns",attr_name,5) == 0)
      {
        string_map_s ns_search = {.key={strlen(attr_value) + 1,(char *)attr_value}};
        unsigned ns_abbr_idx = string_map_tree_s_get_idx(&parser->ns_abbr_map,&ns_search);

        if (attr_name[5] == '\0')
        {
          if (ns_abbr_idx != c_idx_not_exist)
          {
            var_array_s_push_loc(ns_stack,loc_s_string_str(
                  &string_map_tree_s_at(&parser->ns_abbr_map,ns_abbr_idx)->value));
          }
        }
        else if (attr_name[5] == ':')
        {
          const char *namespace = attr_name + 6;
          string_map_tree_s_insert_map(&parser->abbr_ns_map,namespace,attr_value);

          if (ns_abbr_idx != c_idx_not_exist)
          {
            parser->buffer.used = 0;
            bc_array_s_append_format(&parser->buffer,"xmlns:%s",
                string_map_tree_s_at(&parser->ns_abbr_map,ns_abbr_idx)->value.data);
            attr_name_var = xml_parser_s_get_string_var(parser,parser->buffer.used,parser->buffer.data);
          }
        }
      }

      if (attr_name_var == NULL)
      {
        attr_name_var = xml_parser_s_get_string_var(parser,strlen(attr_name),attr_name);
      }

      var_s attr_value_var = xml_parser_s_get_string_var(parser,strlen(attr_value),attr_value);

      var_map_s insert_map = {attr_name_var,attr_value_var};
      unsigned index = var_map_tree_s_unique_insert(tree,&insert_map);

      var_map_s *map = &(tree->data + index)->object;

      if (map->value != attr_value_var)
      {
        var_s_copy_loc(&map->value,attr_value_var);
      }

      attrs += 2;
    }

    var_array_s_push(attrs_stack,&dict_var);
  }
  else
  {
    var_s dict_var = NULL;
    var_array_s_push(attrs_stack,&dict_var);
  }

  if (ns_stack_used == ns_stack->used)
  {
    // - duplicate last namespace -
    var_s ns_var = *var_array_s_last(ns_stack);
    var_array_s_push(ns_stack,&ns_var);
  }

  // - push new node array -
  var_arrays_s_push_blank(node_array_stack);

  // - push new text array -
  var_arrays_s_push_blank(text_array_stack);

  // - push new content array -
  var_arrays_s_push_blank(cont_array_stack);
}/*}}}*/

void xml_parser_s_end_element(void *user,const xmlChar *name)
{/*{{{*/
  xml_parser_s *parser = (xml_parser_s *)user;
  var_array_s *ns_stack = &parser->ns_stack;
  var_array_s *attrs_stack = &parser->attrs_stack;
  var_arrays_s *node_array_stack = &parser->node_array_stack;
  var_arrays_s *text_array_stack = &parser->text_array_stack;
  var_arrays_s *cont_array_stack = &parser->cont_array_stack;

  // -----

  // - retrieve node name -
  var_s name_var = NULL;
  var_s ns_var = *var_array_s_pop(ns_stack);

  const char *name_ptr = strchr((char *)name,':');
  if (name_ptr != NULL)
  {
    string_map_s abbr_search = {.key={(name_ptr - (char *)name) + 1,(char *)name}};
    unsigned abbr_ns_idx = string_map_tree_s_get_idx(&parser->abbr_ns_map,&abbr_search);
    if (abbr_ns_idx != c_idx_not_exist)
    {
      string_map_s ns_search = {.key=string_map_tree_s_at(&parser->abbr_ns_map,abbr_ns_idx)->value};
      unsigned ns_abbr_idx = string_map_tree_s_get_idx(&parser->ns_abbr_map,&ns_search);

      if (ns_abbr_idx != c_idx_not_exist)
      {
        parser->buffer.used = 0;
        bc_array_s_append_format(&parser->buffer,"%s:%s",
            string_map_tree_s_at(&parser->ns_abbr_map,ns_abbr_idx)->value.data,
            name_ptr + 1);
        name_var = xml_parser_s_get_string_var(parser,parser->buffer.used,parser->buffer.data);
      }
    }
  }
  else
  {
    const string_s *ns_str = loc_s_string_value(ns_var);

    if (ns_str->size > 1)
    {
      parser->buffer.used = 0;
      bc_array_s_append_format(&parser->buffer,"%.*s:%s",
          ns_str->size - 1,ns_str->data,(char *)name);
      name_var = xml_parser_s_get_string_var(parser,parser->buffer.used,parser->buffer.data);
    }
  }

  if (name_var == NULL)
  {
    name_var = xml_parser_s_get_string_var(parser,strlen((const char *)name),(const char *)name);
  }

  // - create xml node -
  var_s xml_node_var = loc_s_xml_node(name_var);
  xml_node_s *xml_node = loc_s_xml_node_value(xml_node_var);

  // - set node attributes -
  var_s_copy(&xml_node->attributes,var_array_s_pop(attrs_stack));

  // - retrieve node array -
  var_array_s *node_array = var_arrays_s_pop(node_array_stack);
  if (node_array->used != 0)
  {
    var_s node_array_var = loc_s_array();
    var_array_s_swap(loc_s_array_value(node_array_var),node_array);
    var_s_copy_loc(&xml_node->nodes,node_array_var);
  }

  // - retrieve text array -
  var_array_s *text_array = var_arrays_s_pop(text_array_stack);
  if (text_array->used != 0)
  {
    var_s text_array_var = loc_s_array();
    var_array_s_swap(loc_s_array_value(text_array_var),text_array);
    var_s_copy_loc(&xml_node->texts,text_array_var);
  }

  // - retrieve content array -
  var_array_s *cont_array = var_arrays_s_pop(cont_array_stack);
  if (cont_array->used != 0)
  {
    var_s cont_array_var = loc_s_array();
    var_array_s_swap(loc_s_array_value(cont_array_var),cont_array);
    var_s_copy_loc(&xml_node->conts,cont_array_var);
  }

  // - push node to element and content arrays -
  var_array_s_push(var_arrays_s_last(node_array_stack),&xml_node_var);
  var_array_s_push(var_arrays_s_last(cont_array_stack),&xml_node_var);
}/*}}}*/

void xml_parser_s_characters(void *user,const xmlChar *ch,int len)
{/*{{{*/
  xml_parser_s *parser = (xml_parser_s *)user;
  var_arrays_s *text_array_stack = &parser->text_array_stack;
  var_arrays_s *cont_array_stack = &parser->cont_array_stack;

  // -----

  if (text_array_stack->used != 0 && cont_array_stack->used != 0)
  {
    var_s text_var = xml_parser_s_get_string_var(parser,len,(const char *)ch);

    // - push text to text and content arrays -
    var_array_s_push(var_arrays_s_last(text_array_stack),&text_var);
    var_array_s_push(var_arrays_s_last(cont_array_stack),&text_var);
  }
}/*}}}*/

var_s xml_parser_s_get_string_var(xml_parser_s *this,unsigned a_length,const char *a_data)
{/*{{{*/
  string_tree_s *const_strings = &this->const_strings;
  var_array_s *string_vars = &this->string_vars;

  // -----

  string_s tmp_string = {a_length + 1,(char *)a_data};

  // - get constant position in tree -
  unsigned cs_idx = string_tree_s_unique_insert(const_strings,&tmp_string);

  if (cs_idx >= string_vars->used)
  {
    // - skip rb_tree indexes gap -
    while (cs_idx >= string_vars->used)
    {
      var_array_s_push_blank(string_vars);
    }

    var_s *last_var = var_array_s_last(string_vars);
    var_s_copy_loc(last_var,loc_s_string_str(&const_strings->data[cs_idx].object));

    return *last_var;
  }

  return string_vars->data[cs_idx];
}/*}}}*/

// === global functions ========================================================

int xml_parse_ns(const string_s *a_src,
    var_s *a_trg,string_map_tree_s *a_ns_abbr_map)
{/*{{{*/

  // - create sax handler -
  xmlSAXHandler sax;
  memset(&sax,0,sizeof(sax));

  // - set sax callbacks -
  sax.startDocument = xml_parser_s_start_document;
  sax.endDocument   = xml_parser_s_end_document;
  sax.startElement  = xml_parser_s_start_element;
  sax.endElement    = xml_parser_s_end_element;
  sax.characters    = xml_parser_s_characters;

  sax.warning       = xml_parser_s_warning;
  sax.error         = xml_parser_s_error;
  sax.fatalError    = xml_parser_s_fatal_error;

  // - create sax_parser object -
  CONT_INIT(xml_parser_s,parser);

  // - copy namespace to abbreviation map -
  if (a_ns_abbr_map != NULL)
  {
    string_map_tree_s_copy(&parser.ns_abbr_map,a_ns_abbr_map);
  }

  // - initialize sax parser -
  var_array_s_push_loc(&parser.ns_stack,loc_s_string_ptr(""));
  var_arrays_s_push_blank(&parser.node_array_stack);
  var_arrays_s_push_blank(&parser.cont_array_stack);

  // - parse xml data -
  int res = xmlSAXUserParseMemory(&sax,&parser,a_src->data,a_src->size - 1);

  // - ERROR -
  if (res != 0)
  {
    xml_parser_s_clear(&parser);

    throw_error(XML_ERROR_PARSING_DATA);
  }

  // - retrieve root variable -
  var_s_copy(a_trg,
      var_array_s_pop(var_arrays_s_last(&parser.node_array_stack)));

  xml_parser_s_clear(&parser);

  return 0;
}/*}}}*/

void xml_create_append_string(unsigned a_length,const char *a_data,bc_array_s *a_trg)
{/*{{{*/
  if (a_length != 0)
  {
    const char *ptr = a_data;
    const char *ptr_end = ptr + a_length;
    do {

      switch (*ptr)
      {
        case '<':
          bc_array_s_append(a_trg,4,"&lt;");
          break;
        case '>':
          bc_array_s_append(a_trg,4,"&gt;");
          break;
        case '&':
          bc_array_s_append(a_trg,5,"&amp;");
          break;
        case '\'':
          bc_array_s_append(a_trg,6,"&apos;");
          break;
        case '"':
          bc_array_s_append(a_trg,6,"&quot;");
          break;
        default:
          bc_array_s_push(a_trg,*ptr);
      }

    } while(++ptr < ptr_end);
  }
}/*}}}*/

void xml_create(var_s a_node,bc_array_s *a_trg)
{/*{{{*/

  // - initialize create stack -
  CONT_INIT(xml_create_stack_s,create_stack);

  // - insert root node to create stack -
  xml_create_stack_s_push_blank(&create_stack);
  xml_create_stack_element_s_set(
      xml_create_stack_s_last(&create_stack),loc_s_xml_node_value(a_node),0,1);

  do {

    // - reference to last stack element -
    xml_create_stack_element_s *cs_elm = xml_create_stack_s_last(&create_stack);

    // - retrieve node pointer -
    xml_node_s *node = (xml_node_s *)cs_elm->node;

    // - format node open tag -
    if (cs_elm->index == 0)
    {
      bc_array_s_push(a_trg,'<');

      const string_s *name = loc_s_string_value(node->name);
      bc_array_s_append(a_trg,name->size - 1,name->data);

      // - format node attributes -
      if (node->attributes != NULL)
      {
        var_map_tree_s *tree = loc_s_dict_value(node->attributes);

        if (tree->root_idx != c_idx_not_exist)
        {
          var_map_tree_s_node *tn_ptr = tree->data;
          var_map_tree_s_node *tn_ptr_end = tn_ptr + tree->used;
          do {

            // - tree node is valid -
            if (tn_ptr->valid)
            {
              bc_array_s_push(a_trg,' ');

              const string_s *key_ptr = loc_s_string_value(tn_ptr->object.key);
              bc_array_s_append(a_trg,key_ptr->size - 1,key_ptr->data);

              bc_array_s_push(a_trg,'=');
              bc_array_s_push(a_trg,'"');

              const string_s *value_ptr = loc_s_string_value(tn_ptr->object.value);
              xml_create_append_string(value_ptr->size - 1,value_ptr->data,a_trg);

              bc_array_s_push(a_trg,'"');
            }
          } while(++tn_ptr < tn_ptr_end);
        }
      }

      bc_array_s_push(a_trg,'>');
    }

    // - process node contents -
    if (node->conts != NULL)
    {
      var_array_s *conts_array = loc_s_array_value(node->conts);

      if (cs_elm->index < conts_array->used)
      {
        var_s item_var = conts_array->data[cs_elm->index++];

        if (item_var->v_type == g_type_xml_node)
        {
          xml_create_stack_s_push_blank(&create_stack);
          xml_create_stack_element_s_set(
              xml_create_stack_s_last(&create_stack),loc_s_xml_node_value(item_var),0,1);
        }
        else if (item_var->v_type == c_bi_type_string)
        {
          const string_s *string = loc_s_string_value(item_var);

          if (node->escape_texts)
          {
            xml_create_append_string(string->size - 1,string->data,a_trg);
          }
          else
          {
            bc_array_s_append(a_trg,string->size - 1,string->data);
          }
        }
        else
        {
          cassert(0);
        }

        continue;
      }
    }

    // - format node close tag -
    {
      bc_array_s_push(a_trg,'<');
      bc_array_s_push(a_trg,'/');

      const string_s *name = loc_s_string_value(node->name);
      bc_array_s_append(a_trg,name->size - 1,name->data);

      bc_array_s_push(a_trg,'>');
    }

    xml_create_stack_s_pop(&create_stack);

  } while(create_stack.used > 0);

  // - release create stack -
  xml_create_stack_s_clear(&create_stack);
}/*}}}*/

void xml_create_nice(var_s a_node,const string_s *a_tabulator,
    const string_s *a_indent,bc_array_s *a_trg)
{/*{{{*/
#define XML_CREATE_NICE_PUSH_TAB() \
{/*{{{*/\
  if ((indent_size += a_tabulator->size - 1) > indent_buffer.used)\
  {\
    bc_array_s_append(&indent_buffer,a_tabulator->size - 1,a_tabulator->data);\
  }\
}/*}}}*/

#define XML_CREATE_NICE_POP_TAB() \
{/*{{{*/\
  indent_size -= a_tabulator->size - 1;\
}/*}}}*/

#define XML_CREATE_NICE_INDENT() \
{/*{{{*/\
  bc_array_s_push(a_trg,'\n');\
  xml_create_append_string(indent_size,indent_buffer.data,a_trg);\
}/*}}}*/

  // - reset target buffer -
  a_trg->used = 0;

  // - initialize indent buffer -
  CONT_INIT(bc_array_s,indent_buffer);

  bc_array_s_append(&indent_buffer,a_indent->size - 1,a_indent->data);

  // - initialize actual indent size -
  unsigned indent_size = indent_buffer.used;

  // - initialize create stack -
  CONT_INIT(xml_create_stack_s,create_stack);

  // - insert root node to create stack -
  xml_create_stack_s_push_blank(&create_stack);
  xml_create_stack_element_s_set(xml_create_stack_s_last(&create_stack),
      loc_s_xml_node_value(a_node),0,0);

  do {

    // - reference to last stack element -
    xml_create_stack_element_s *cs_elm = xml_create_stack_s_last(&create_stack);

    // - retrieve node pointer -
    xml_node_s *node = (xml_node_s *)cs_elm->node;

    // - format node open tag -
    if (cs_elm->index == 0)
    {
      if (cs_elm->after_node)
      {
        XML_CREATE_NICE_INDENT();
      }

      bc_array_s_push(a_trg,'<');

      const string_s *name = loc_s_string_value(node->name);
      bc_array_s_append(a_trg,name->size - 1,name->data);

      // - format node attributes -
      if (node->attributes != NULL)
      {
        var_map_tree_s *tree = loc_s_dict_value(node->attributes);

        if (tree->root_idx != c_idx_not_exist)
        {
          var_map_tree_s_node *tn_ptr = tree->data;
          var_map_tree_s_node *tn_ptr_end = tn_ptr + tree->used;
          do {

            // - tree node is valid -
            if (tn_ptr->valid)
            {
              bc_array_s_push(a_trg,' ');

              const string_s *key_ptr = loc_s_string_value(tn_ptr->object.key);
              bc_array_s_append(a_trg,key_ptr->size - 1,key_ptr->data);

              bc_array_s_push(a_trg,'=');
              bc_array_s_push(a_trg,'"');

              const string_s *value_ptr = loc_s_string_value(tn_ptr->object.value);
              xml_create_append_string(value_ptr->size - 1,value_ptr->data,a_trg);

              bc_array_s_push(a_trg,'"');
            }
          } while(++tn_ptr < tn_ptr_end);
        }
      }

      bc_array_s_push(a_trg,'>');

      cs_elm->after_node = 1;
    }

    // - process node contents -
    if (node->conts != NULL)
    {
      var_array_s *conts_array = loc_s_array_value(node->conts);

      if (cs_elm->index < conts_array->used)
      {
        var_s item_var = conts_array->data[cs_elm->index++];

        if (item_var->v_type == g_type_xml_node)
        {
          XML_CREATE_NICE_PUSH_TAB();

          int cs_elm_after_node = cs_elm->after_node;
          cs_elm->after_node = 1;

          xml_create_stack_s_push_blank(&create_stack);
          xml_create_stack_element_s_set(xml_create_stack_s_last(&create_stack),
              loc_s_xml_node_value(item_var),0,cs_elm_after_node);
        }
        else if (item_var->v_type == c_bi_type_string)
        {
          const string_s *string = loc_s_string_value(item_var);

          if (node->escape_texts)
          {
            xml_create_append_string(string->size - 1,string->data,a_trg);
          }
          else
          {
            bc_array_s_append(a_trg,string->size - 1,string->data);
          }

          cs_elm->after_node = 0;
        }
        else
        {
          cassert(0);
        }

        continue;
      }
    }

    // - format node close tag -
    {
      if (cs_elm->after_node && cs_elm->index > 0)
      {
        XML_CREATE_NICE_INDENT();
      }

      bc_array_s_push(a_trg,'<');
      bc_array_s_push(a_trg,'/');

      const string_s *name = loc_s_string_value(node->name);
      bc_array_s_append(a_trg,name->size - 1,name->data);

      bc_array_s_push(a_trg,'>');
    }

    XML_CREATE_NICE_POP_TAB();

    xml_create_stack_s_pop(&create_stack);

  } while(create_stack.used > 0);

  // - release create stack -
  xml_create_stack_s_clear(&create_stack);

  // - release indent buffer -
  bc_array_s_clear(&indent_buffer);
}/*}}}*/

void libxml_cll_init()
{/*{{{*/
  xmlInitParser();

  // - init var strings -
  var_s_copy_loc(&g_str_empty_var,loc_s_string_ptr(""));
  var_s_copy_loc(&g_str_attrs_var,loc_s_string_ptr("attrs"));
  var_s_copy_loc(&g_str_nodes_var,loc_s_string_ptr("nodes"));
  var_s_copy_loc(&g_str_text_var,loc_s_string_ptr("text"));

  // - loc_s_register_type -
  g_type_xml_node = loc_s_register_type(
    loc_s_xml_node_clear,
    loc_s_xml_node_order,
#if OPTION_TO_STRING == ENABLED
    loc_s_xml_node_to_string,
#else
    NULL,
#endif
#if OPTION_TO_JSON == ENABLED
    loc_s_xml_node_to_json,
    loc_s_xml_node_to_json_nice
#else
    NULL,
    NULL
#endif
    );
}/*}}}*/

void libxml_cll_clear()
{/*{{{*/

  // - clear var strings -
  var_s_clear(&g_str_empty_var);
  var_s_clear(&g_str_attrs_var);
  var_s_clear(&g_str_nodes_var);
  var_s_clear(&g_str_text_var);

  xmlCleanupParser();
}/*}}}*/

