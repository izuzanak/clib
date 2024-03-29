
@begin
include "cl_parser.h"
@end

// - basic constants and definitions -

typedef int bool;
#define true 1
#define false 0

// === methods of generated structures =========================================

// -- parser_fa_s --
@begin
methods parser_fa_s
@end

int parser_fa_s_create(parser_fa_s *this, const string_array_s *a_reg_exps)
{/*{{{*/
  parser_fa_s_clear(this);

  // - ERROR -
  if (a_reg_exps->used == 0)
  {
    throw_error(FINAL_AUTOMATA_REGULAR_EXPRESSIONS_EMPTY);
  }

  CONT_INIT_CLEAR(fa_states_array_s,states_array);
  CONT_INIT_CLEAR(reg_parser_s,reg_parser);

  unsigned re_idx = 0;
  do {

    // - ERROR -
    if (!reg_parser_s_process_reg_exp(&reg_parser,a_reg_exps->data + re_idx,re_idx))
    {
      throw_error(FINAL_AUTOMATA_REGULAR_EXPRESSION_PARSE_ERROR);
    }

    fa_states_array_s_push_blank(&states_array);
    fa_states_s_swap(fa_states_array_s_last(&states_array),&reg_parser.states);

  } while(++re_idx < a_reg_exps->used);

  finite_automata_s_create_new(&this->fa,&states_array);

  return 0;
}/*}}}*/

// -- ui_tree_s --
@begin
methods ui_tree_s
@end

// -- ui_tree_array_s --
@begin
methods ui_tree_array_s
@end

// -- inverted_index_s --
@begin
methods inverted_index_s
@end

void inverted_index_s_update(inverted_index_s *this,fa_states_array_s *states_array)
{/*{{{*/
  if (this->states.used == 0 && states_array->used == 0)
  {
    return;
  }

  int extended_states_array = 0;
  unsigned ii_states_idx = states_array->used;

  // - extend states array by inverted index states -
  if (this->states.used != 0)
  {
    fa_states_array_s_push_blank(states_array);
    fa_states_s_swap(fa_states_array_s_last(states_array),&this->states);

    extended_states_array = 1;
  }

  fa_states_s_clear(&this->states);

  // - queue of new states descriptions -
  CONT_INIT_CLEAR(fa_state_descr_queue_s,queue);
  CONT_INIT_CLEAR(fa_state_reg_state_tree_s,state_descr_tree);

  // - array of moves used by one state -
  CONT_INIT_CLEAR(fa_state_moves_array_s,moves_array);

  // - description of new produced automata state -
  CONT_INIT_CLEAR(fa_state_descr_s,new_state_descr);

  // - insert first record to queue of new states description -
  {
    /*
     * source state does not exist for first state. set of regular expressions
     * contains all of them with index at position zero
     */
    fa_state_descr_queue_s_insert_blank(&queue);
    fa_state_descr_queue_s_last(&queue)->from_state_id = c_idx_not_exist;
    reg_states_s *init_reg_states = &fa_state_descr_queue_s_last(&queue)->reg_states;

    if (extended_states_array)
    {
      // - enque original inverted index states -
      reg_states_s_push_blank(init_reg_states);
      reg_state_s_set(reg_states_s_last(init_reg_states),ii_states_idx,0);
    }

    if (ii_states_idx != 0)
    {
      unsigned idx = 0;
      do
      {
        reg_states_s_push_blank(init_reg_states);
        reg_state_s_set(reg_states_s_last(init_reg_states),idx,0);
      }
      while(++idx < ii_states_idx);
    }
  }

  // - loop while queue of news states is not empty -
  do
  {
    // - retrieve description of next state from queue -
    fa_state_descr_s q_state_descr;
    fa_state_descr_s_init(&q_state_descr);
    fa_state_descr_s_swap(&q_state_descr,fa_state_descr_queue_s_next(&queue));

    // - test if such state is not defined already -
    unsigned state_descr_idx = fa_state_reg_state_tree_s_get_idx(&state_descr_tree,&q_state_descr.reg_states);
    if (state_descr_idx != c_idx_not_exist)
    {
      /*
       * if yes, retrieve source state for this state, retrieve move index in
       * source state that matches move character, and in source state for this
       * state set as target state already defined state
       *
       * continue at loop beginning
       */
      fa_state_s *state = fa_states_s_at(&this->states,q_state_descr.from_state_id);

      fa_state_move_s s_move;
      s_move.idx = q_state_descr.with_char;

      unsigned move_idx = fa_state_moves_s_get_map_idx(&state->moves,&s_move);
      cassert(move_idx != c_idx_not_exist);
      fa_state_moves_s_at(&state->moves,move_idx)->value = state_descr_idx - 1;

      fa_state_descr_s_clear(&q_state_descr);
      continue;
    }

    // - create new state of automata, create reference to its end terminal and set of moves-
    fa_states_s_push_blank(&this->states);
    unsigned *state_final = &fa_states_s_last(&this->states)->final;
    *state_final = c_idx_not_exist;
    fa_state_moves_s *state_moves = &fa_states_s_last(&this->states)->moves;

    // - set moves to this state -
    reg_states_s *p_reg_states = &q_state_descr.reg_states;
    cassert(p_reg_states->used != 0);

    // - if source state exists -
    if (q_state_descr.from_state_id != c_idx_not_exist)
    {
      // - set move from source state to this state -
      fa_state_s *from_state = fa_states_s_at(&this->states,q_state_descr.from_state_id);

      fa_state_move_s s_move;
      s_move.idx = q_state_descr.with_char;

      unsigned move_idx = fa_state_moves_s_get_map_idx(&from_state->moves,&s_move);
      cassert(move_idx != c_idx_not_exist);
      fa_state_moves_s_at(&from_state->moves,move_idx)->value = this->states.used - 1;
    }

    // - generate array of moves from actual regexp positions -
    moves_array.used = 0;
    reg_state_s *p_reg_state_ptr = p_reg_states->data;
    reg_state_s *p_reg_state_ptr_end = p_reg_state_ptr + p_reg_states->used;

    // - loop through regexp list, and generation of possible moves set -
    do
    {
      fa_state_moves_array_s_push_blank(&moves_array);
      fa_state_s *state = fa_states_s_at(fa_states_array_s_at(states_array,
            p_reg_state_ptr->ui_first),p_reg_state_ptr->ui_second);

      fa_state_moves_s_copy(fa_state_moves_array_s_last(&moves_array),&state->moves);

      // - assign regexp to final state -
      if (state->final != c_idx_not_exist)
      {
        if (p_reg_state_ptr->ui_first >= ii_states_idx)
        {
          debug_assert(*state_final == c_idx_not_exist);
          *state_final = state->final;
        }
        else
        {
          if (*state_final == c_idx_not_exist)
          {
            *state_final = this->targets.used;
            ui_tree_array_s_push_blank(&this->targets);
            ui_tree_s_insert(ui_tree_array_s_last(&this->targets),state->final);
          }
          else
          {
            ui_tree_s_unique_insert(ui_tree_array_s_at(&this->targets,*state_final),state->final);
          }
        }
      }

    }
    while(++p_reg_state_ptr < p_reg_state_ptr_end);

    do
    {
      unsigned short move_char = 0xffff;

      // - get next move character from set -
      fa_state_moves_s *moves_ptr = moves_array.data;
      fa_state_moves_s *moves_ptr_end = moves_ptr + moves_array.used;

      do
      {
        if (moves_ptr->used > 0)
        {
          fa_state_move_s *move_ptr = moves_ptr->data;
          fa_state_move_s *move_ptr_end = move_ptr + moves_ptr->used;

          do
          {
            if (move_ptr->idx < move_char)
            {
              move_char = move_ptr->idx;
            }
          }
          while(++move_ptr < move_ptr_end);
        }
      }
      while(++moves_ptr < moves_ptr_end);

      if (move_char == 0xffff)
      {
        break;
      }

      // - add new move from state without identification of target state -
      fa_state_moves_s_push_blank(state_moves);
      fa_state_moves_s_last(state_moves)->idx = move_char;

      // - create description of move target state -
      new_state_descr.from_state_id = this->states.used - 1;
      new_state_descr.with_char = move_char;
      reg_states_s *new_reg_states = &new_state_descr.reg_states;

      new_reg_states->used = 0;

      // - found and process all moves by this character -
      p_reg_state_ptr = p_reg_states->data;
      moves_ptr = moves_array.data;
      moves_ptr_end = moves_ptr + moves_array.used;

      // - loop through all moves -
      do
      {
        if (moves_ptr->used > 0)
        {
          fa_state_move_s *move_ptr = moves_ptr->data;
          fa_state_move_s *move_ptr_end = move_ptr + moves_ptr->used;

          do
          {
            if (move_ptr->idx != 0xffff)
            {
              unsigned short ptr_char = move_ptr->idx;
              bool match = false;
              bool eq_match = false;

              if (ptr_char >= c_no_char_base)
              {
                if (move_char == ptr_char)
                {
                  eq_match = true;
                }
                else if (move_char != (ptr_char - c_no_char_base))
                {
                  match = true;
                }
              }
              else if (ptr_char == reg_char_digit)
              {
                if (move_char == reg_char_digit)
                {
                  eq_match = true;
                }
                else if (move_char >= '0' && move_char <= '9')
                {
                  match = true;
                }
              }
              else if (ptr_char == reg_char_letter)
              {
                if (move_char == reg_char_letter)
                {
                  eq_match = true;
                }
                else if ((move_char >= 'a' && move_char <= 'z') || (move_char >= 'A' && move_char <= 'Z'))
                {
                  match = true;
                }
              }
              else if (ptr_char == reg_char_white)
              {
                if (move_char == reg_char_white)
                {
                  eq_match = true;
                }
                else if (move_char == ' ' || (move_char >= '\b' && move_char <= '\r' ))
                {
                  match = true;
                }
              }
              else if (ptr_char == move_char)
              {
                eq_match = true;
              }

              if (eq_match || match)
              {
                reg_states_s_push_blank(new_reg_states);
                reg_state_s_set(reg_states_s_last(new_reg_states),p_reg_state_ptr->ui_first,move_ptr->value);

                if (eq_match)
                {
                  move_ptr->idx = 0xffff;
                }
              }
            }
          }
          while(++move_ptr < move_ptr_end);
        }
      }
      while(++p_reg_state_ptr,++moves_ptr < moves_ptr_end);

      // - insert new state description to queue of new states descriptions -
      fa_state_descr_queue_s_insert_blank(&queue);
      fa_state_descr_s_swap(fa_state_descr_queue_s_last(&queue),&new_state_descr);
    }
    while(true);

    // - store description of accepted state, for state existence control -
    fa_state_reg_state_tree_s_swap_insert(&state_descr_tree,&q_state_descr.reg_states);

    fa_state_descr_s_clear(&q_state_descr);
  }
  while(queue.used > 0);

  if (extended_states_array)
  {
    // - remove old inverted index states from states array -
    fa_states_array_s_pop(states_array);
  }
}/*}}}*/

void inverted_index_s_remove_targets(inverted_index_s *this,
    const ui_array_s *a_targets)
{/*{{{*/
  debug_assert(this->states.used != 0 && this->targets.used != 0 && a_targets->used != 0);

  // - removed target flag -
  int removed_target = 0;

  ui_tree_s *t_ptr = this->targets.data;
  ui_tree_s *t_ptr_end = t_ptr + this->targets.used;
  do {
    unsigned *at_ptr = a_targets->data;
    unsigned *at_ptr_end = at_ptr + a_targets->used;
    do {

      // - set of targets contains removed target -
      unsigned target_idx = ui_tree_s_get_idx(t_ptr,*at_ptr);
      if (target_idx != c_idx_not_exist)
      {
        ui_tree_s_remove(t_ptr,target_idx);

        // - set removed target flag -
        removed_target = 1;
      }

    } while(++at_ptr < at_ptr_end);
  } while(++t_ptr < t_ptr_end);

  // - removed target flag is set -
  if (removed_target)
  {
    CONT_INIT_CLEAR(ui_array_s,keep_finals);
    ui_array_s_copy_resize(&keep_finals,this->targets.used);
    ui_array_s_fill(&keep_finals,0);

    ui_tree_s *t_ptr = this->targets.data;
    ui_tree_s *t_ptr_end = t_ptr + this->targets.used;
    do {

      // - set of targets is not empty -
      if (t_ptr->root_idx != c_idx_not_exist)
      {
        ui_tree_s_node *ttn_ptr = t_ptr->data;
        ui_tree_s_node *ttn_ptr_end = ttn_ptr + t_ptr->used;
        do
        {
          if (ttn_ptr->valid)
          {
            *ui_array_s_at(&keep_finals,t_ptr - this->targets.data) = 1;
          }
        } while(++ttn_ptr < ttn_ptr_end);
      }
    } while(++t_ptr < t_ptr_end);

    CONT_INIT_CLEAR(ui_array_s,keep_states);
    ui_array_s_copy_resize(&keep_states,this->states.used);
    ui_array_s_fill(&keep_states,0);

    do {

      // - keep states changed flag -
      int keep_states_changed = 0;

      unsigned fs_idx = this->states.used;
      do
      {
        --fs_idx;

        // - state not keeped -
        if (!*ui_array_s_at(&keep_states,fs_idx))
        {
          fa_state_s *fs_ptr = fa_states_s_at(&this->states,fs_idx);

          int keep_state = 0;

          if (fs_ptr->moves.used != 0)
          {
            fa_state_move_s *fsm_ptr = fs_ptr->moves.data;
            fa_state_move_s *fsm_ptr_end = fsm_ptr + fs_ptr->moves.used;
            do
            {
              // - move to keep state -
              if (*ui_array_s_at(&keep_states,fsm_ptr->value))
              {
                keep_state = 1;
                break;
              }
            } while(++fsm_ptr < fsm_ptr_end);
          }

          if (fs_ptr->final != c_idx_not_exist)
          {
            // - final in keep finals -
            if (*ui_array_s_at(&keep_finals,fs_ptr->final))
            {
              keep_state = 1;
            }
            else
            {
              fs_ptr->final = c_idx_not_exist;
            }
          }

          // - move to keep state -
          if (keep_state)
          {
            *ui_array_s_at(&keep_states,fs_idx) = 1;
            keep_states_changed = 1;
          }
        }
      } while(fs_idx > 0);

      // - count of keeped states does not changed -
      if (!keep_states_changed)
      {
        break;
      }
    } while(1);

    // - map finals to new positions -
    unsigned *kf_ptr = keep_finals.data;
    unsigned *kf_ptr_end = kf_ptr + keep_finals.used;
    unsigned kf_idx = 0;
    do {
      *kf_ptr = *kf_ptr ? kf_idx++ : c_idx_not_exist;
    } while(++kf_ptr < kf_ptr_end);

    // - map states to new positions -
    unsigned *ks_ptr = keep_states.data;
    unsigned *ks_ptr_end = ks_ptr + keep_states.used;
    unsigned ks_idx = 0;
    do {
      *ks_ptr = *ks_ptr ? ks_idx++ : c_idx_not_exist;
    } while(++ks_ptr < ks_ptr_end);

    // - rebuild states -
    CONT_INIT_CLEAR(fa_states_s,states);

    unsigned fs_idx = 0;
    do
    {
      if (*ui_array_s_at(&keep_states,fs_idx) != c_idx_not_exist)
      {
        fa_state_s *fs_ptr = fa_states_s_at(&this->states,fs_idx);

        fa_states_s_push_blank(&states);
        fa_state_s *nfs_ptr = fa_states_s_last(&states);

        nfs_ptr->final = fs_ptr->final == c_idx_not_exist ?
          c_idx_not_exist : *ui_array_s_at(&keep_finals,fs_ptr->final);

        if (fs_ptr->moves.used != 0)
        {
          fa_state_move_s *fsm_ptr = fs_ptr->moves.data;
          fa_state_move_s *fsm_ptr_end = fsm_ptr + fs_ptr->moves.used;
          do {
            unsigned value = *ui_array_s_at(&keep_states,fsm_ptr->value);
            if (value != c_idx_not_exist)
            {
              fa_state_moves_s_push_blank(&nfs_ptr->moves);
              fa_state_move_s_set(fa_state_moves_s_last(&nfs_ptr->moves),fsm_ptr->idx,value);
            }
          } while(++fsm_ptr < fsm_ptr_end);
        }
      }
    } while(++fs_idx < this->states.used);

    // - rebuild targets -
    CONT_INIT_CLEAR(ui_tree_array_s,targets);

    unsigned t_idx = 0;
    do
    {
      if (*ui_array_s_at(&keep_finals,t_idx) != c_idx_not_exist)
      {
        ui_tree_array_s_push_blank(&targets);
        ui_tree_s_swap(ui_tree_array_s_last(&targets),ui_tree_array_s_at(&this->targets,t_idx));
      }
    } while(++t_idx < this->targets.used);

    fa_states_s_swap(&states,&this->states);
    ui_tree_array_s_swap(&targets,&this->targets);
  }
}/*}}}*/

void inverted_index_s_dump(inverted_index_s *this,ui_array_s *a_trg)
{/*{{{*/
  CONT_INIT_CLEAR(ui_array_s,state_offsets);
  CONT_INIT_CLEAR(ui_arrays_s,move_offsets);
  CONT_INIT_CLEAR(ui_arrays_s,final_offsets);
  CONT_INIT_CLEAR(ui_tree_s,target_set);

  // - ofsset to targets space -
  ui_array_s_push(a_trg,0);

  // - ofsset to target list space -
  ui_array_s_push(a_trg,0);

  // - store states count -
  ui_array_s_push(a_trg,this->states.used);

  if (this->states.used != 0)
  {
    fa_state_s *fs_ptr = this->states.data;
    fa_state_s *fs_ptr_end = fs_ptr + this->states.used;
    unsigned fs_idx = 0;
    do
    {
      // - process state move offsets -
      if (move_offsets.used > fs_idx)
      {
        ui_array_s *move_offsets_ptr = ui_arrays_s_at(&move_offsets,fs_idx);
        if (move_offsets_ptr->used != 0)
        {
          unsigned *mo_ptr = move_offsets_ptr->data;
          unsigned *mo_ptr_end = mo_ptr + move_offsets_ptr->used;
          do
          {
            // - update state move offset -
            *ui_array_s_at(a_trg,*mo_ptr) = a_trg->used;
          } while(++mo_ptr < mo_ptr_end);
        }
      }

      ui_array_s_push(&state_offsets,a_trg->used);

      if (fs_ptr->final == c_idx_not_exist)
      {
        // - store state final index -
        ui_array_s_push(a_trg,c_idx_not_exist);
      }
      else
      {
        unsigned final_idx = fs_ptr->final;

        // - resize final offsets array -
        while (final_offsets.used <= final_idx)
        {
          ui_arrays_s_push_clear(&final_offsets);
        }

        // - store offset to final index -
        ui_array_s_push(ui_arrays_s_at(&final_offsets,final_idx),a_trg->used);

        // - offset to target space -
        ui_array_s_push(a_trg,0);
      }

      // - store state move count -
      ui_array_s_push(a_trg,fs_ptr->moves.used);

      if (fs_ptr->moves.used != 0)
      {
        fa_state_move_s *fsm_ptr = fs_ptr->moves.data;
        fa_state_move_s *fsm_ptr_end = fsm_ptr + fs_ptr->moves.used;
        do {
          ui_array_s_push(a_trg,fsm_ptr->idx);

          if (fsm_ptr->value <= fs_idx)
          {
            // - store move to already processes state -
            unsigned move_offset = *ui_array_s_at(&state_offsets,fsm_ptr->value);
            ui_array_s_push(a_trg,move_offset);
          }
          else
          {
            unsigned move_fs_idx = fsm_ptr->value;

            // - resize move offsets array -
            while (move_offsets.used <= move_fs_idx)
            {
              ui_arrays_s_push_clear(&move_offsets);
            }

            // - offset to move state index -
            ui_array_s_push(ui_arrays_s_at(&move_offsets,move_fs_idx),a_trg->used);

            // - move to unprocessed state space -
            ui_array_s_push(a_trg,0);
          }

        } while(++fsm_ptr < fsm_ptr_end);
      }
    } while(++fs_idx,++fs_ptr < fs_ptr_end);
  }

  // - store targets offset -
  *ui_array_s_at(a_trg,c_ii_dump_offset_to_targets_offset) = a_trg->used;

  // - store target trees count -
  ui_array_s_push(a_trg,this->targets.used);

  if (this->targets.used != 0)
  {
    ui_tree_s *t_ptr = this->targets.data;
    ui_tree_s *t_ptr_end = t_ptr + this->targets.used;
    unsigned t_idx = 0;
    do
    {
      // - process final offsets -
      if (final_offsets.used > t_idx)
      {
        ui_array_s *final_offsets_ptr = ui_arrays_s_at(&final_offsets,t_idx);
        if (final_offsets_ptr->used != 0)
        {
          unsigned *fo_ptr = final_offsets_ptr->data;
          unsigned *fo_ptr_end = fo_ptr + final_offsets_ptr->used;
          do
          {
            // - update final offset -
            *ui_array_s_at(a_trg,*fo_ptr) = a_trg->used;
          } while(++fo_ptr < fo_ptr_end);
        }
      }

      // - store targets count -
      ui_array_s_push(a_trg,t_ptr->count);

      if (t_ptr->root_idx != c_idx_not_exist)
      {
        unsigned stack[RB_TREE_STACK_SIZE(ui_tree_s,t_ptr)];
        unsigned *stack_ptr = stack;

        unsigned idx = ui_tree_s_get_stack_min_value_idx(t_ptr,t_ptr->root_idx,&stack_ptr);
        do
        {
          unsigned target = *ui_tree_s_at(t_ptr,idx);

          // - store target value -
          ui_array_s_push(a_trg,target);

          // - update targets set -
          ui_tree_s_unique_insert(&target_set,target);

          idx = ui_tree_s_get_stack_next_idx(t_ptr,idx,&stack_ptr,stack);
        } while(idx != c_idx_not_exist);
      }
    } while(++t_idx,++t_ptr < t_ptr_end);
  }

  // - store targets list offset -
  *ui_array_s_at(a_trg,c_ii_dump_offset_to_target_list_offset) = a_trg->used;

  // - store targets count -
  ui_array_s_push(a_trg,target_set.count);

  if (target_set.root_idx != c_idx_not_exist)
  {
    unsigned stack[RB_TREE_STACK_SIZE(ui_tree_s,&target_set)];
    unsigned *stack_ptr = stack;

    unsigned idx = ui_tree_s_get_stack_min_value_idx(&target_set,target_set.root_idx,&stack_ptr);
    do
    {
      // - store target value -
      ui_array_s_push(a_trg,*ui_tree_s_at(&target_set,idx));

      idx = ui_tree_s_get_stack_next_idx(&target_set,idx,&stack_ptr,stack);
    } while(idx != c_idx_not_exist);
  }
}/*}}}*/

int inverted_index_s_load(inverted_index_s *this,const unsigned *a_data)
{/*{{{*/
  inverted_index_s_clear(this);

  CONT_INIT_CLEAR(ui_tree_s,state_offsets);
  CONT_INIT_CLEAR(ui_tree_s,final_offsets);

  // - retrieve offset maps -
  {
    // - skip targets offsets -
    unsigned offset = c_ii_dump_state_count_offset;

    unsigned state_cnt = a_data[offset++];
    if (state_cnt != 0)
    {
      unsigned state_idx = 0;
      do
      {
        // - store state offset -
        ui_tree_s_insert(&state_offsets,offset);

        // - skip target offset -
        ++offset;

        // - skip state moves -
        unsigned move_cnt = a_data[offset++];
        offset += 2*move_cnt;

      } while(++state_idx < state_cnt);
    }

    unsigned targets_cnt = a_data[offset++];
    if (targets_cnt != 0)
    {
      unsigned targets_idx = 0;
      do {
        ui_tree_s_insert(&final_offsets,offset);

        // - skip targets -
        unsigned target_cnt = a_data[offset++];
        offset += target_cnt;
      } while(++targets_idx < targets_cnt);
    }
  }

  // - load inverted index -
  {
    // - skip targets offset -
    unsigned offset = c_ii_dump_state_count_offset;

    // - load states -
    unsigned state_cnt = a_data[offset++];
    if (state_cnt != 0)
    {
      unsigned state_idx = 0;
      do
      {
        fa_states_s_push_blank(&this->states);
        fa_state_s *fs_ptr = fa_states_s_last(&this->states);

        // - retrieve targets index -
        unsigned final_offset = a_data[offset++];
        if (final_offset == c_idx_not_exist)
        {
          fs_ptr->final = c_idx_not_exist;
        }
        else
        {
          unsigned final_idx = ui_tree_s_get_idx(&final_offsets,final_offset);
          debug_assert(final_idx != c_idx_not_exist);
          fs_ptr->final = final_idx - 1;
        }

        // - retrieve moves -
        unsigned move_cnt = a_data[offset++];
        if (move_cnt != 0)
        {
          unsigned move_idx = 0;
          do {
            fa_state_moves_s_push_blank(&fs_ptr->moves);
            fa_state_move_s *fsm_ptr = fa_state_moves_s_last(&fs_ptr->moves);

            fsm_ptr->idx = a_data[offset++];

            unsigned state_offset = a_data[offset++];
            unsigned state_idx = ui_tree_s_get_idx(&state_offsets,state_offset);
            debug_assert(state_idx != c_idx_not_exist);
            fsm_ptr->value = state_idx - 1;
          } while(++move_idx < move_cnt);
        }

      } while(++state_idx < state_cnt);
    }

    debug_assert(a_data[c_ii_dump_offset_to_targets_offset] == offset);

    // - load targets -
    unsigned targets_cnt = a_data[offset++];
    if (targets_cnt != 0)
    {
      unsigned targets_idx = 0;
      do {
        ui_tree_array_s_push_blank(&this->targets);
        ui_tree_s *targets = ui_tree_array_s_last(&this->targets);

        unsigned target_cnt = a_data[offset++];
        if (target_cnt != 0)
        {
          unsigned target_idx = 0;
          do {
            ui_tree_s_insert(targets,a_data[offset++]);
          } while(++target_idx < target_cnt);
        }
      } while(++targets_idx < targets_cnt);
    }

    debug_assert(a_data[c_ii_dump_offset_to_target_list_offset] == offset);
  }

  return 0;
}/*}}}*/

unsigned inverted_index_dump_s_recognize(const unsigned *a_dump,
    const char *input,unsigned *input_idx,unsigned input_length)
{/*{{{*/
  debug_assert(input != NULL && a_dump[c_ii_dump_state_count_offset] != 0);

  // - set first state -
  const unsigned *state_ptr = a_dump + c_ii_dump_first_state_offset;

  // - endless loop searching for terminal symbol -
  do
  {
    // - reset next state offset -
    unsigned next_state_offset = c_idx_not_exist;

    // - process next input character -
    unsigned short in_char;
    if (*input_idx < input_length)
    {
      in_char = (unsigned char)input[*input_idx];
    }
    else
    {
      in_char = '\0';
    }

    // - retrieve next state index -
    unsigned move_cnt = state_ptr[1];
    if (move_cnt != 0)
    {
      // - brute force search -
      const unsigned *move_ptr = state_ptr + 2;
      const unsigned *move_ptr_end = move_ptr + (move_cnt << 1);
      do
      {
        if (move_ptr[0] < c_base_char_cnt)
        {
          if (move_ptr[0] == in_char)
          {
            next_state_offset = move_ptr[1];
          }
        }
        else
        {
          // - process special regular expressions -
          if (move_ptr[0] >= c_no_char_base)
          {
            if (move_ptr[0] != in_char)
            {
              next_state_offset = move_ptr[1];
            }
          }
          else
          {
            switch (move_ptr[0])
            {
            case reg_char_white:
            {
              if (in_char == ' ' || (in_char >= '\b' && in_char <= '\r'))
              {
                next_state_offset = move_ptr[1];
              }
            }
            break;
            case reg_char_digit:
            {
              if (in_char >= '0' && in_char <= '9')
              {
                next_state_offset = move_ptr[1];
              }
            }
            break;
            case reg_char_letter:
            {
              if ((in_char >= 'a' && in_char <= 'z') ||
                  (in_char >= 'A' && in_char <= 'Z'))
              {
                next_state_offset = move_ptr[1];
              }

            }
            break;
            default:
              cassert(0);
            }
          }
        }
      } while((move_ptr += 2) < move_ptr_end);
    }

    // - transition to next state if it exists -
    if (next_state_offset != c_idx_not_exist)
    {
      // - input end -
      if (*input_idx >= input_length)
      {
        return a_dump[next_state_offset + 0];
      }

      (*input_idx)++;

      state_ptr = a_dump + next_state_offset;
    }

    // - if there is no next state -
    else
    {
      return state_ptr[0];
    }
  }
  while(1);
}/*}}}*/

// -- inverted_indexes_s --
@begin
methods inverted_indexes_s
@end

void inverted_indexes_s_merge(inverted_indexes_s *this,inverted_index_s *a_trg)
{/*{{{*/
  inverted_index_s_clear(a_trg);

  if (this->used != 0)
  {
    // - queue of new states descriptions -
    CONT_INIT_CLEAR(fa_state_descr_queue_s,queue);
    CONT_INIT_CLEAR(fa_state_reg_state_tree_s,state_descr_tree);

    // - array of moves used by one state -
    CONT_INIT_CLEAR(fa_state_moves_array_s,moves_array);

    // - description of new produced automata state -
    CONT_INIT_CLEAR(fa_state_descr_s,new_state_descr);

    // - insert first record to queue of new states description -
    {
      /*
       * source state does not exist for first state. set of regular expressions
       * contains all of them with index at position zero
       */
      fa_state_descr_queue_s_insert_blank(&queue);
      fa_state_descr_queue_s_last(&queue)->from_state_id = c_idx_not_exist;
      reg_states_s *init_reg_states = &fa_state_descr_queue_s_last(&queue)->reg_states;
      reg_states_s_copy_resize(init_reg_states,this->used);

      unsigned idx = 0;
      do
      {
        reg_states_s_push_blank(init_reg_states);
        reg_state_s_set(reg_states_s_last(init_reg_states),idx,0);
      }
      while(++idx < this->used);
    }

    // - loop while queue of news states is not empty -
    do
    {
      // - retrieve description of next state from queue -
      fa_state_descr_s q_state_descr;
      fa_state_descr_s_init(&q_state_descr);
      fa_state_descr_s_swap(&q_state_descr,fa_state_descr_queue_s_next(&queue));

      // - test if such state is not defined already -
      unsigned state_descr_idx = fa_state_reg_state_tree_s_get_idx(&state_descr_tree,&q_state_descr.reg_states);
      if (state_descr_idx != c_idx_not_exist)
      {
        /*
         * if yes, retrieve source state for this state, retrieve move index in
         * source state that matches move character, and in source state for this
         * state set as target state already defined state
         *
         * continue at loop beginning
         */
        fa_state_s *state = fa_states_s_at(&a_trg->states,q_state_descr.from_state_id);

        fa_state_move_s s_move;
        s_move.idx = q_state_descr.with_char;

        unsigned move_idx = fa_state_moves_s_get_map_idx(&state->moves,&s_move);
        cassert(move_idx != c_idx_not_exist);
        fa_state_moves_s_at(&state->moves,move_idx)->value = state_descr_idx - 1;

        fa_state_descr_s_clear(&q_state_descr);
        continue;
      }

      // - create new state of automata, create reference to its end terminal and set of moves-
      fa_states_s_push_blank(&a_trg->states);
      unsigned *state_final = &fa_states_s_last(&a_trg->states)->final;
      *state_final = c_idx_not_exist;
      fa_state_moves_s *state_moves = &fa_states_s_last(&a_trg->states)->moves;

      // - set moves to this state -
      reg_states_s *p_reg_states = &q_state_descr.reg_states;
      cassert(p_reg_states->used != 0);

      // - if source state exists -
      if (q_state_descr.from_state_id != c_idx_not_exist)
      {
        // - set move from source state to this state -
        fa_state_s *from_state = fa_states_s_at(&a_trg->states,q_state_descr.from_state_id);

        fa_state_move_s s_move;
        s_move.idx = q_state_descr.with_char;

        unsigned move_idx = fa_state_moves_s_get_map_idx(&from_state->moves,&s_move);
        cassert(move_idx != c_idx_not_exist);
        fa_state_moves_s_at(&from_state->moves,move_idx)->value = a_trg->states.used - 1;
      }

      // - generate array of moves from actual regexp positions -
      moves_array.used = 0;
      reg_state_s *p_reg_state_ptr = p_reg_states->data;
      reg_state_s *p_reg_state_ptr_end = p_reg_state_ptr + p_reg_states->used;

      // - loop through regexp list, and generation of possible moves set -
      do
      {
        fa_state_moves_array_s_push_blank(&moves_array);
        inverted_index_s *inverted_index = inverted_indexes_s_at(this,p_reg_state_ptr->ui_first);
        fa_state_s *state = fa_states_s_at(&inverted_index->states,p_reg_state_ptr->ui_second);

        fa_state_moves_s_copy(fa_state_moves_array_s_last(&moves_array),&state->moves);

        // - assign regexp to final state -
        if (state->final != c_idx_not_exist)
        {
          if (*state_final == c_idx_not_exist)
          {
            *state_final = a_trg->targets.used;
            ui_tree_array_s_push(&a_trg->targets,ui_tree_array_s_at(&inverted_index->targets,state->final));
          }
          else
          {
            ui_tree_s *trg_targets = ui_tree_array_s_at(&a_trg->targets,*state_final);
            ui_tree_s *inverted_index_targets = ui_tree_array_s_at(&inverted_index->targets,state->final);

            // - union of inverted index and target trees -
            ui_tree_s_node *iitn_ptr = inverted_index_targets->data;
            ui_tree_s_node *iitn_ptr_end = iitn_ptr + inverted_index_targets->used;
            do
            {
              if (iitn_ptr->valid)
              {
                ui_tree_s_unique_insert(trg_targets,iitn_ptr->object);
              }
            } while(++iitn_ptr < iitn_ptr_end);
          }
        }

      }
      while(++p_reg_state_ptr < p_reg_state_ptr_end);

      do
      {
        unsigned short move_char = 0xffff;

        // - get next move character from set -
        fa_state_moves_s *moves_ptr = moves_array.data;
        fa_state_moves_s *moves_ptr_end = moves_ptr + moves_array.used;

        do
        {
          if (moves_ptr->used > 0)
          {
            fa_state_move_s *move_ptr = moves_ptr->data;
            fa_state_move_s *move_ptr_end = move_ptr + moves_ptr->used;

            do
            {
              if (move_ptr->idx < move_char)
              {
                move_char = move_ptr->idx;
              }
            }
            while(++move_ptr < move_ptr_end);
          }
        }
        while(++moves_ptr < moves_ptr_end);

        if (move_char == 0xffff)
        {
          break;
        }

        // - add new move from state without identification of target state -
        fa_state_moves_s_push_blank(state_moves);
        fa_state_moves_s_last(state_moves)->idx = move_char;

        // - create description of move target state -
        new_state_descr.from_state_id = a_trg->states.used - 1;
        new_state_descr.with_char = move_char;
        reg_states_s *new_reg_states = &new_state_descr.reg_states;

        new_reg_states->used = 0;

        // - found and process all moves by this character -
        p_reg_state_ptr = p_reg_states->data;
        moves_ptr = moves_array.data;
        moves_ptr_end = moves_ptr + moves_array.used;

        // - loop through all moves -
        do
        {
          if (moves_ptr->used > 0)
          {
            fa_state_move_s *move_ptr = moves_ptr->data;
            fa_state_move_s *move_ptr_end = move_ptr + moves_ptr->used;

            do
            {
              if (move_ptr->idx != 0xffff)
              {
                unsigned short ptr_char = move_ptr->idx;
                bool match = false;
                bool eq_match = false;

                if (ptr_char >= c_no_char_base)
                {
                  if (move_char == ptr_char)
                  {
                    eq_match = true;
                  }
                  else if (move_char != (ptr_char - c_no_char_base))
                  {
                    match = true;
                  }
                }
                else if (ptr_char == reg_char_digit)
                {
                  if (move_char == reg_char_digit)
                  {
                    eq_match = true;
                  }
                  else if (move_char >= '0' && move_char <= '9')
                  {
                    match = true;
                  }
                }
                else if (ptr_char == reg_char_letter)
                {
                  if (move_char == reg_char_letter)
                  {
                    eq_match = true;
                  }
                  else if ((move_char >= 'a' && move_char <= 'z') || (move_char >= 'A' && move_char <= 'Z'))
                  {
                    match = true;
                  }
                }
                else if (ptr_char == reg_char_white)
                {
                  if (move_char == reg_char_white)
                  {
                    eq_match = true;
                  }
                  else if (move_char == ' ' || (move_char >= '\b' && move_char <= '\r' ))
                  {
                    match = true;
                  }
                }
                else if (ptr_char == move_char)
                {
                  eq_match = true;
                }

                if (eq_match || match)
                {
                  reg_states_s_push_blank(new_reg_states);
                  reg_state_s_set(reg_states_s_last(new_reg_states),p_reg_state_ptr->ui_first,move_ptr->value);

                  if (eq_match)
                  {
                    move_ptr->idx = 0xffff;
                  }
                }
              }
            }
            while(++move_ptr < move_ptr_end);
          }
        }
        while(++p_reg_state_ptr,++moves_ptr < moves_ptr_end);

        // - insert new state description to queue of new states descriptions -
        fa_state_descr_queue_s_insert_blank(&queue);
        fa_state_descr_s_swap(fa_state_descr_queue_s_last(&queue),&new_state_descr);
      }
      while(true);

      // - store description of accepted state, for state existence control -
      fa_state_reg_state_tree_s_swap_insert(&state_descr_tree,&q_state_descr.reg_states);

      fa_state_descr_s_clear(&q_state_descr);
    }
    while(queue.used > 0);
  }
}/*}}}*/

