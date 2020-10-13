/*
doubly linked list

*/


/*typedef struct changes changes;
typedef struct lnkd lnkd;*/
#include "LinkedLst.h"



lnkd* new_stage(lnkd* stage);
/*create a new lnked list and return it*/
lnkd* init_lnkd(int new){
   lnkd* stage = (lnkd*)malloc(sizeof(lnkd));
   if (new == 0){stage->next = NULL;}
   if (new == 1){stage->next = new_stage(stage);}
   stage->prv = NULL;
   stage->plays = NULL;
   stage->last_play= NULL;
   if (new == 1){return (stage->next);}
   return stage;
}
/*delete changes list*/
void clean_changes_lst(changes* plays){
  if (plays != NULL){
    clean_changes_lst(plays->next);
    free(plays);
  }
}
/*recursion function to delete the stages list nodes from stage node to the next nodes */
lnkd* clean_stages_list(lnkd* stage){
    if (stage != NULL){
    stage->next = clean_stages_list(stage->next);
    clean_changes_lst(stage->plays);
    free(stage);
    return NULL;
  }
    return NULL;
}

/*recursion function to delete the stage node and its previous nodes*/
lnkd* clean_stages_list_from_top(lnkd* stage){
    if (stage != NULL){
    stage->prv = clean_stages_list_from_top(stage->prv);
    clean_changes_lst(stage->plays);
    free(stage);
    return NULL;
  }
    return NULL;
}
/*
prv_val = value of cell i,j before the change that been made,
val = value of cell i,j after the change that been made
updated the changes list of the give stage with the given values*/
void upt_changes(lnkd* stage,int i,int j,int prv_val,int val){
    if (stage->plays ==NULL){
      stage->plays = (changes*)malloc(sizeof(changes));
      stage->last_play = stage->plays;
      stage->last_play->i = i;stage->last_play->j = j;stage->last_play->val = val;
      stage->last_play->prv_val = prv_val;
      }
    else{
   stage->last_play->next = (changes*)malloc(sizeof(changes));
   stage->last_play =stage->last_play->next;
   stage->last_play->i = i;stage->last_play->j = j;stage->last_play->val = val;
    stage->last_play->prv_val = prv_val;
    }
   stage->last_play->next =NULL;
  
}
/*creating a new linked list node - a new stage
  and return it*/
lnkd* new_stage(lnkd* stage){
  stage->next = init_lnkd(0);
  stage->next->prv = stage;
  /*stage = stage->next;*/
  return stage->next;
}

/*print the plays of the stage,undo_redo = 1 - for undo 
undo_redo = 2 - for redo, reset = 1 if usef for reset*/
void changes_print_upt(int** mat,lnkd* stage,int undo_redo,int reset,int N){
  
 changes* stage_plays = stage->plays;
 while (stage_plays != NULL){
   if (undo_redo == 1){
   if (reset == 0 && ( (stage_plays->val)%(N+1) != (stage_plays->prv_val)%(N+1) ) ){
	printf("cell (%d,%d) changed from %d to %d\n",stage_plays->i+1,
   	stage_plays->j+1,(stage_plays->val)%(N+1),(stage_plays->prv_val)%(N+1));}
   	mat[stage_plays->i][stage_plays->j] = stage_plays->prv_val;
   }
   if(undo_redo == 2){

	if ( (stage_plays->prv_val)%(N+1) != (stage_plays->val)%(N+1)  ){
   	printf("cell (%d,%d) changed from %d to %d\n",stage_plays->i+1,
   	stage_plays->j+1,(stage_plays->prv_val)%(N+1),(stage_plays->val)%(N+1));}
  	mat[stage_plays->i][stage_plays->j] = stage_plays->val;
   }
   stage_plays = stage_plays->next;
 }
  
}

