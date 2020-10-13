/*
 * LinkedLst.h
 *
 *  Created on: 25 במאי 2019
 *      Author: אוהד גזית
 */

#include <stdlib.h>
#include <stdio.h>           


#ifndef LINKEDLST_H_
#define LINKEDLST_H_

/*i,j = cell values
 val = cell value after change
  prv_val = cell value before change*/
typedef struct changes_1 {
  int i;
  int j;
  int val;
  int prv_val;
  struct changes_1* next;
} changes;
/*plays = list of changes done in the stage (function)
 last_play = top node of the changes list*/
typedef struct lnkd_1 {
    changes* plays;
    changes* last_play;
    struct lnkd_1* next;
    struct lnkd_1* prv;    
} lnkd;

lnkd* init_lnkd(int new);
void clean_changes_lst(changes* plays);
lnkd* clean_stages_list(lnkd* stage);
lnkd* clean_stages_list_from_top(lnkd* stage);
void upt_changes(lnkd* stage,int i,int j,int prv_val,int val);
lnkd* new_stage(lnkd* stage);
void changes_print_upt(int** mat,lnkd* stage,int undo_redo,int reset,int N);
void counting(lnkd* stage);


#endif /* LINKEDLST_H_ */
