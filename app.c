/*
 * =====================================================================================
 *
 *       Filename:  app.c
 *
 *    Description:  This file is a test demo for making use of fsm project
 *
 *        Version:  1.0
 *        Created:  Saturday 31 August 2019 02:14:25  IST
 *       Revision:  1.0
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Networking Developer (AS), sachinites@gmail.com
 *        Company:  Brocade Communications(Jul 2012- Mar 2016), Current : Juniper Networks(Apr 2017 - Present)
 *        
 *        This file is part of the FSMProject distribution (https://github.com/sachinites).
 *        Copyright (c) 2017 Abhishek Sagar.
 *        This program is free software: you can redistribute it and/or modify
 *        it under the terms of the GNU General Public License as published by  
 *        the Free Software Foundation, version 3.
 *
 *        This program is distributed in the hope that it will be useful, but 
 *        WITHOUT ANY WARRANTY; without even the implied warranty of 
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 *        General Public License for more details.
 *
 *        You should have received a copy of the GNU General Public License 
 *        along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fsm.h"

unsigned int
bit_flipper_input_reader_fn(char *input_buff,
                         unsigned int size,
                         unsigned int start_pos,
                         char *read_out_buff,   /*Already zeroed out*/
                         unsigned int *read_out_buff_len,
                         unsigned int read_out_buff_max_size){


   if(start_pos > (size -1))
       return 0;

   memcpy(read_out_buff, input_buff + start_pos, 1);
   read_out_buff[1] = '\0';
   *read_out_buff_len = 1;
   return 1; 
}

void
bit_flipper_output_fn_gen(char *input_buff, 
                          unsigned int size){

    char out;

    out = (*input_buff == '1') ? '0' : '1';
    printf("%c", out);
}

fsm_bool_t
bit_flipper_key_match_fn(char *data1, unsigned int size1,
                         char *data2, unsigned int size2){

   
    if(*data1 == *data2) 
        return FSM_TRUE;
     return FSM_FALSE;
}

int
main(int argc, char **argv){

  /*Create a FSM*/
  fsm_t *fsm = create_new_fsm("Bit Flipper");
  
  /*Assign the input buffer to FSM to execute*/
  strncpy(fsm->input_buffer, "0101010101010", strlen("0101010101010"));
  set_fsm_input_buffer_size(fsm, strlen("0101010101010"));
  
  /*Register application specific input reader fn with FSM*/  
  fsm_register_input_reader_fn(fsm, bit_flipper_input_reader_fn);

  /*Optional : Register the state specific matching function with FSM*/
  fsm_register_generic_state_input_matching_fn_cb(fsm, bit_flipper_key_match_fn);

  /*Create FSM State*/
  state_t *state_S0 = create_new_state(fsm, "S0", FSM_TRUE, bit_flipper_key_match_fn);

  /*Set FSM initial state*/
  set_fsm_initial_state(fsm, state_S0);

  /*Insert Transitions into State's Transition Table*/
  char bit = '0';
  create_and_insert_new_tt_entry(&state_S0->state_trans_table,
                                 &bit, 1,
                                 bit_flipper_output_fn_gen, 
                                 state_S0);

  bit = '1';
  create_and_insert_new_tt_entry(&state_S0->state_trans_table,
                                 &bit, 1, 
                                 bit_flipper_output_fn_gen, 
                                 state_S0);
  
  execute_fsm(fsm);
      
  return 0;
}