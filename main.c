
#include <stdlib.h>
#include <stdio.h>           
#include "ComStruct.h"
#include <string.h>
#include <time.h>
#include "LinkedLst.h"

   
    

   
                    
/*setting all the game values data*/
int main(void){  
	
     	com_data a;    
	game_data b;                        
	int i;
	b.stages =NULL;    b.mat = NULL;                                 
	a.com_num = 0; a.error =0;   a.mark_errors = 1;
	a.set_param[0] =0;a.set_param[1] = 1;a.set_param[2] =0; for (i=0;i<260;i++)a.path[i]=0;
	b.mode_status =  0      ;       
	b.cols = 0 ; 
	b.rows = 0;      
	b.N = 0;  
	srand(time(NULL));                                                    
	while ( com_manager(&a,&b) == 1) {}                              
	for(i=0;i< b.N  ;i++){free(b.mat[i]);   }  
	free(b.mat);                
	return 0;   
	if (b.stages->prv != NULL){clean_stages_list_from_top(b.stages->prv);} 
	clean_stages_list(b.stages);               
}       