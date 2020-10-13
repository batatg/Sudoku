/*
 * ComStruct.c
 *  
 *   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ComStruct.h"
#include "Ezer.c"

 
#define SOLVE 1
#define EDIT 2
#define MARK_ERRORS 3
#define PRINT_BOARD 4
#define SET 5
#define VALIDATE 6
#define GUESS 7
#define GENERATE 8
#define UNDO 9
#define REDO 10
#define SAVE 11
#define HINT 12
#define GUESS_HINT 13
#define NUM_SOLUTIONS 14
#define AUTOFILL 15
#define RESET 16
#define EXIT 17
#define WRONG_COMMAND 18
#define EMPTY_LINE 19
#define LONG_COMMAND 20


/*
data structure of the commands;
error = 1 - not enough parameters
error = 2 - too much parameters
error = 3 - invalid parameters
com_num = the command number
set_param = command parameters*/

int com_manager(/*int ** mat,*/com_data * data,game_data * game);
void com_name(com_data * data,int status,char * token);
void param_check(/*int ** mat,*/com_data * data, game_data * game);

/*
managing the commands 
gets the data structure and game status
getting the user command check it and calls to the needed command
*/
int com_manager(/*int ** mat,*/com_data * data,game_data * game){
	char* deli =  " \t\r\n";
	char str[260];
	char * token;
	int status = game->mode_status;
	if( game->mode_status == 0){printf("please enter solve or edit command to start a new puzzle\n");}
	else{printf("please enter a command\n");}
	if (fgets(str,260,stdin) != NULL  && strcmp(str,"")!= 0 ){
		if (strlen(str) > 256) { data-> com_num = LONG_COMMAND; printf("command is too long\n"); return 1;}	
		token = strtok(str,deli);
		com_name(data,status,token); 
		if (data -> com_num == EMPTY_LINE){ return 1;}
		/*param_check(data,game);*/

		
	}
	else
		{
		data->com_num = EXIT; 
	}
	param_check(/*game->mat,*/data,game); if (data->com_num == EXIT){return 0;}
	if ( data-> error == 0 && game->mode_status != 0 && (data->com_num == SET || data->com_num == AUTOFILL || data->com_num == UNDO || 
	data->com_num == REDO || data->com_num == GUESS || data->com_num == GENERATE || data->com_num == RESET || data->com_num == SOLVE || data->com_num == EDIT) ){
		PrintScreen(game->mat,game->rows,game->cols,game->N,data->mark_errors,game->mode_status);}
	if ( ( empty_cells_count(game->mat,game->N) == 0 ) && ( game->mode_status == SOLVE ) ){ 
	if(validate_command(game->mat,game->N,game->rows,game->cols,2) != -2 ){
		printf("the board is solved succesfully\n");
		game->mode_status = 0;}
	else{printf("the board is erroneous\n");}
	}
		
	data->error = 0;
	return 1;
}

/*gets the data structure and game status
  check if the user input is a valid command
  print a message and store the user command in the data structure
*/
void com_name(com_data * data,int status,char * token){
		data->com_num = WRONG_COMMAND;
		if (token != NULL){
		if (strcmp(token,"solve")==0){
			data -> com_num = SOLVE;
			return;
		}
		if (strcmp(token,"edit")==0){
			data->com_num = EDIT;
			return;
		}
		if (strcmp(token,"mark_errors")==0){
			if (status == 0 || status == EDIT) { printf("mark_errors is available in solve mode\n"); return;}
			data->com_num = MARK_ERRORS;
			return;
		}
		if (strcmp(token,"print_board")==0){
			if (status == 0) { printf("print_board is available only in edit and solve mode\n"); return;}
			data->com_num = PRINT_BOARD;
			return;
		}
		if (strcmp(token,"set")==0){
			if (status == 0) { printf("set is available only in edit and solve mode\n"); return;}
			data->com_num = SET;
			return;
		}
		if (strcmp(token,"validate")==0){
			if (status == 0) { printf("validate is available only in edit and solve mode \n"); return;}
			data->com_num = VALIDATE;
			return;
		}
		if (strcmp(token,"guess")==0){
			if (status == 0 || status == EDIT) { printf("guess is available only in solve mode\n"); return;}
			data->com_num = GUESS;
			return;
		}
		if (strcmp(token,"generate")==0){
			if (status == 0 || status == SOLVE) { printf("generate is available only in edit mode\n"); return;}
			data->com_num = GENERATE;
			return;
		}
		if (strcmp(token,"undo")==0){
			if (status == 0) { printf("undo is available only in edit and solve mode\n"); return;}
			data->com_num = UNDO;
			return;
		}
		if (strcmp(token,"redo")==0){
			if (status == 0) { printf("redo is available only in edit and solve mode\n"); return;}
			data->com_num = REDO;
			return;
		}
		if (strcmp(token,"save")==0){
			if (status == 0) { printf("save is available only in edit and solve mode\n"); return;}
			data->com_num = SAVE;
			return;
		}
		if (strcmp(token,"hint")==0){
			if (status == 0 || status == EDIT) { printf("hint is available only in solve mode\n"); return;}
			data->com_num = HINT;
			return;
		}
		if (strcmp(token,"guess_hint")==0){
			if (status == 0 || status == EDIT) { printf("guess_hint is available only in solve mode\n"); return;}
			data->com_num = GUESS_HINT;
			return;
		}
		if (strcmp(token,"num_solutions")==0){
			if (status == 0) { printf("num_solutions is available only in edit and solve mode\n"); return;}
			data->com_num = NUM_SOLUTIONS;
			return;
		}
		if (strcmp(token,"autofill")==0){
			if (status == 0 || status == EDIT) { printf("autofill is available only in solve mode\n"); return;}
			data->com_num = AUTOFILL;
			return;
		}
		if (strcmp(token,"reset")==0){
			if (status == 0) { printf("reset is available only in edit and solve mode\n"); return;}
			data->com_num = RESET;
			return;
		}
		if (strcmp(token,"exit")==0){
			data->com_num = EXIT;
			return;
		}
		if (data->com_num == WRONG_COMMAND) { printf("no such command exist\n");}
		}
		else	
			if (token == NULL)
				data->com_num = EMPTY_LINE;
	
	
}
/* get the board size and checks if the parameters are valid 
for the set command*/
void set_param_check(com_data * data,game_data * game, char * array[]){
	/*int N = game -> N;*/
		int a[3] ;
	/*int i = str_to_int(array,3,data -> set_param);*/
		int i = str_to_int(array,3,a);
	if (i != 0 ){printf("parameter %d is invalid, please enter cell index between 1 to %d and value between 0 to %d\n",i,game -> N,game -> N); data -> error = 3; return ;}
	/*i = valid_nums(2,data -> set_param,1,game->N);*/
		i = valid_nums(2,a,1,game->N);
	if( i != 0 ){printf("parameter %d of cell index is invalid, please enter cell index between 1 to %d and value between 0 to %d\n",i, game -> N,game -> N); data -> error = 3; return ;}
		data -> set_param[0] = a[0];data -> set_param[1] = a[1];data -> set_param[2] = a[2];
	i = valid_nums(1, data->set_param+2,0,game->N);
	if( i != 0 ){printf("parameter %d of cell value is invalid, please enter cell index between 1 to %d and value between 0 to %d\n",i, game -> N,game-> N); data -> error = 3; return ;}
	
	return  ;  
}

/*
gets the data structure and the command of the user
checks if there is enough parameters and saving them
updating the error field in the data structure if needed
*/
void param_check(/*int ** mat,*/com_data * data, game_data * game){
	char* deli = " \t\r\n";int** hlp;
	char * token;
	int* lst1;
	int* lst2;
	char * path1 = "";
	int too_long;
	char * set_array[3]; 
	float f = 0;
	int i/*,j */; int cube = (game->rows)*(game->cols);
	int board_size = ((game->rows)*(game->rows)*(game->cols)*(game->cols));
	data -> error = 0;
	token = strtok (NULL,deli);
	if (token == NULL){
		if(data->com_num == SOLVE || data->com_num == MARK_ERRORS || data->com_num == SET
		 || data->com_num == GUESS || data->com_num ==  GENERATE || data->com_num == SAVE
		 || data->com_num ==  HINT || data->com_num == GUESS_HINT){
			strcpy(data -> path, "");
			data -> error = 1; 	
		}
	}
	else if (data->com_num == PRINT_BOARD || data->com_num == VALIDATE || data->com_num == UNDO
		|| data->com_num == REDO || data->com_num == NUM_SOLUTIONS || data->com_num == AUTOFILL
		|| data->com_num == RESET || data->com_num == EXIT){
		data->error = 2;	
		}
	if (data -> com_num == SOLVE || data -> com_num == SAVE){
		if(data->error == 1){ printf("please enter a file path as parameter\n");return;}
		path1 = token;
		strcpy(data -> path, path1);
		token = strtok (NULL,deli);
		if (token != NULL){ printf("too much parameters, please enter only file path as parameter\n"); data->error = 2;strcpy(data -> path, "");  return;}
		data -> error = 0;
		
		
	}
	if (data -> com_num == SOLVE){/*in solve and parameter is ok*/
		lst1 = (int*)malloc(sizeof(int)*(2));
		hlp = file_load(/*game->mat,*/data->path,lst1,1);
		if (hlp == NULL){return;} 
		if (game->mat != NULL){ free_mat(game->mat,game->N);free(game->mat);}
		game->mat = hlp; 
		game->stages = clean_stages_list_from_top(game->stages);
		game->stages = init_lnkd(1);
		game->rows = lst1[0]; game->cols = lst1[1]; game->N = (game->cols)*(game->rows);
		game->mode_status = SOLVE;
		free(lst1);
		
	}

	
	else
	if ( data -> com_num == EDIT){
		if (token == NULL){   /* edit without address parameter*/
			strcpy(data -> path, ""); data->error = 0; 
			game->mat = edit_command(game->mat,game->N);
			game->cols = 3; 
			game->rows = 3;	
			game->N = 9;
			game->mode_status = EDIT;  
			/*data -> mark_errors = 1;*/
			game->stages = clean_stages_list_from_top(game->stages);
			game->stages = init_lnkd(1);

			return;
		}
		else { 
			strcpy(data -> path, token); 
			data->error = 0;
			token = strtok (NULL,deli);
			if (token != NULL){ printf("too much parameters, please enter file path or 0 parameters\n"); data->error = 2; strcpy(data -> path, ""); return;}
			lst1 = (int*)malloc(sizeof(int)*(2));
			hlp = file_load(/*game->mat,*/data->path,lst1,2);
			if (hlp == NULL){return;} 
			if (game->mat != NULL){ free_mat(game->mat,game->N); free(game->mat);}
			game->mat = hlp;
			game->rows = lst1[0]; game->cols = lst1[1]; game->N = (game->cols)*(game->rows);
			game->mode_status = EDIT;
			game->stages = clean_stages_list_from_top(game->stages);
			game->stages = init_lnkd(1);
			/*data -> mark_errors = 1;*/
			free(lst1);
			return;
		}
	}
	if (data -> com_num == MARK_ERRORS){  
		if(data->error == 1){ printf("please enter 0 or 1 as parameters\n");return;}		
		data -> mark_errors = token[0]-48;
		if (strlen(token)>1){ too_long = 1;} else too_long = 0;
		token = strtok (NULL,deli);
		if (token != NULL){ printf("too much parameters, please enter 0 or 1 as parameters\n"); data->error = 2;  return;}
		if (too_long  == 1 ){
			printf("invalid parameter,please enter 0 or 1 as parameter\n");
			data -> error = 3;
			return;
		}
		if((data -> mark_errors) > 1 || (data -> mark_errors) < 0){ printf("wrong parameter, please enter 0 or 1 as parameter\n"); data -> error = 3; return;}
		return;      
	}
	if ( data -> com_num == PRINT_BOARD){
		if (data->error == 2){printf("no valid parameters for print_board command\n");return;}	
		PrintScreen(game->mat,game->rows,game->cols,game->N,data->mark_errors,game->mode_status);
	}
	if (data -> com_num == SET){ 
		if(data->error == 1){ printf("not enough parameters, please enter cell index and value as parameters\n");return;}		
		set_array[0] = token;
		for (i=0; i<2;i++){
			token = strtok (NULL,deli);
			if (token == NULL){printf("not enough parameters, please enter cell index and value as parameters\n"); data ->error =1;return;}
			set_array[i+1] = token;
		}
		
		token = strtok (NULL,deli);
		if (token != NULL){printf("too much parameters, please enter cell index and value as parameters\n"); data ->error =2;return;}
		set_param_check(data,game,set_array); 
		/*valid_nums(3, data->set_param,1,game->N);*/
		/*if (i != 0){printf("parameter %d is invalid please enter cell index between 1 and %d and value between 0 and %d as parameters",i,game->N,game->N);return;}*/
		if( data -> error == 0) {
			if ( game->mode_status == SOLVE){i = check_erroneous_fixed(game->mat,game->N,data->set_param[1]-1,data->set_param[0]-1);
			if ( i == 1){printf("the cell is fixed and cant be changed in solve mode\n"); return;}}
			i = game->mat[data->set_param[1]-1][data->set_param[0]-1]; 
			set_command(game->stages,game->mat,data->set_param,i,game->N,game->rows,game->cols);
		
				/*find_erroneous(game->mat,game->N,data->set_param[1]-1,data->set_param[0]-1,game->rows,game->cols,i);*/
				/*game->stages = new_stage(game->stages);*/
				/*upt_changes(game->stages,data->set_param[1]-1,data->set_param[0]-1,i,game->mat[data->set_param[1]-1][data->set_param[0]-1]);*/
				/*game->stages = game->stages->next;*/
		game->stages = game->stages->next;}

		return;               
 
	}
	if ( data-> com_num == VALIDATE ){
		if (data->error ==2){ printf("no valid parameters for validate command\n");return;}
		i = validate_command(game->mat,game->N,game->rows,game->cols,1);
	}
	if ( data -> com_num == GUESS){
		if (data -> error == 1){printf("not enough parameters, please enter a threshold parameter\n");return;}
		set_array[0] = token;
		token = strtok (NULL,deli);
		if (token != NULL){printf("too much parameters, please enter a threshold parameter\n");return;}
		i = sscanf(set_array[0],"%f %n", &f, &too_long);
		if (i == EOF) {printf("invalid parameter, please enter a threshold parameter\n");return;}
		i = strlen(set_array[0]);
		if (i != too_long){printf("invalid parameter, please enter a threshold parameter\n");return;}
		if (f > 1 || f < 0 ){printf("invalid parameter, please enter a threshold parameter\n");return;}
		i = guess_command(game->stages,game->mat,game->N,game->rows,game->cols,f);
		/*if (game->stages->plays->next != NULL){
		printf("%d %d\n" ,game->stages->plays->val,game->stages->plays->next->val);
		} else{printf("heyyeyey\n");}*/
		if( i == 1){game->stages= game->stages->next;}
		return;

	}
	if ( data-> com_num == GENERATE){
		if(data->error == 1){ printf("not enough parameters, please enter 2 parameters between 0 and %d \n",board_size);return;}
		set_array[0] = token;
		token = strtok (NULL,deli);
		if (token == NULL ){printf("not enough parameters, please enter 2 parameters between 0 and %d \n",board_size); data ->error =1;return;}
		set_array[1] = token;
		token = strtok (NULL,deli);
		if (token != NULL){printf("too much parameters, please enter 2 parameters between 0 and %d \n",board_size); data ->error =2;return;}
		i = str_to_int(set_array,2,data->set_param);
		if ( i != 0 ){ printf("parameter %d is invalid, please enter 2 parameters between 0 and %d\n",i,board_size); data-> error = 3; return;}
		i = valid_nums(2,data->set_param,0,board_size);
		if ( i != 0 ){ printf("parameter %d is invalid, please enter 2 parameters between 0 and %d\n",i,board_size); data-> error = 3; return;}
		i = empty_cells_count(game->mat,game->N);
		if (i < data->set_param[0]){printf("not enough empty cells, first parameter is invalid for this board\n"); data-> error = 3; return;}
		i = generate(game->mat,game->stages,game->N,game->rows,game->cols,(data->set_param)[0],(data->set_param)[1]);
		game->stages = game->stages->next;
		
		
	}
	if (data->com_num == UNDO){ /*need to enter the func itself*/
		if (data->error ==2){ printf("no valid parameters for undo command\n");return;}
		game->stages = undo_command(game->mat,game->stages,game->N);
	}
	if (data->com_num == REDO){ /*need to enter the func itself*/
		if (data->error ==2){ printf("no valid parameters for redo command\n");return;}
		game->stages = redo_command(game->mat,game->stages,game->N);
	}
	if (data-> com_num == HINT || data-> com_num == GUESS_HINT){
		set_array[0] = token;
		token = strtok (NULL,deli);
		if (token == NULL ){printf("not enough parameters, please enter 2 parameters between 1 and %d as cell index\n",cube); data ->error = 1;return;}
		set_array[1] = token;
		token = strtok (NULL,deli);
		if (token != NULL){printf("too much parameters, please enter 2 parameters between 1 and %d as cell index\n",cube); data ->error =2;return;}
		i = str_to_int(set_array,2,data->set_param);
		if ( i != 0 ){ printf("parameter %d is invalid, please enter 2 parameters between 1 and %d\n",i,cube); data-> error = 3; return;}
		i = valid_nums(2,data->set_param,1,cube);
		if ( i != 0 ){ printf("parameter %d is invalid, please enter 2 parameters between 1 and %d\n",i,cube); data-> error = 3; return;}
		lst1=(int*) malloc((game->N)*sizeof(int)); /* allocated lists for uses in other functions */
  		lst2=(int*) malloc((game->N)*sizeof(int));for (i=0;i<game->N;i++){lst1[i]=0;lst2[i]=0;}
		i = check_valid_full(game->mat,lst1,lst2,-1,game->N,game->rows,game->cols);
		if (i == 0){printf("the board is erroneous, invalid command \n");data-> error = 3; free(lst1);free(lst2);return;}	
		i = check_erroneous_fixed(game->mat,game->N,data->set_param[0]-1,data->set_param[1]-1);
		if (i==1){printf("the cell is fixed, invalid command with this cell index\n");data-> error = 3;free(lst1);free(lst2); return;}
		if ( (game->mat)[data->set_param[1]-1][data->set_param[0]-1]%( (game->N)+2) != 0 ){printf("the cell is not empty, invalid command with this cell index\n");data ->error =3;free(lst1);free(lst2); return;}
		free(lst1);free(lst2);
	}
	if (data -> com_num == HINT){
		hint_command(game->mat,game->cols,game->rows,game->N,data->set_param[1]-1,data->set_param[0]-1);

	}
	if (data -> com_num == GUESS_HINT){
		guess_hint_command(game->mat,game->cols,game->rows,game->N,data->set_param[1]-1,data->set_param[0]-1);

	}

	if (data -> com_num == NUM_SOLUTIONS){
		lst1=(int*) malloc((game->N)*sizeof(int));  
  		lst2=(int*) malloc((game->N)*sizeof(int));for (i=0;i<game->N;i++){lst1[i]=0;lst2[i]=0;}
		if ( data->error == 2 ) {printf("no valid parameters for num_solutions command\n");free(lst1);free(lst2);return;}
		i = check_valid_full(game->mat,lst1,lst2,-1,game->N,game->rows,game->cols);
		if (i == 0){printf("the board is erroneous, invalid command \n");data-> error = 3; free(lst1);free(lst2);return;}	
		num_solutions_command(game->mat,game->rows,game->cols,game->N);
		/*i = count_solutions(game->mat,1,game->rows,game->cols);
		if (i>0){printf("there are %d solutions for this board\n",i);free(lst1);free(lst2);return;}
		printf("the board is unsolvable\n");*/
		free(lst1);free(lst2);
		return;
	}
	if (data -> com_num == AUTOFILL ){
		if ( data->error == 2 ) {printf("no valid parameters for autofill command\n");return;}
		i = autofill_command(game->stages,game->mat,game->rows,game->cols);
		if (i == 1){game->stages= game->stages->next;}
		return;
	}
	if (data -> com_num == RESET ){
		if ( data->error == 2 ) {printf("no valid parameters for reset command\n");return;}
		game->stages = reset_command(game->mat,game->stages);
		return;
	}
	if (data -> com_num == EXIT){
		if ( data->error == 2 ) {printf("no valid parameters for exit command\n");return;}
		printf("exit..\n");
		return;
	}
	if (data -> com_num == SAVE){
		if (game->mode_status == EDIT){
			if( validate_command(game->mat,game->N,game->rows,game->cols,0)	== -1){
				return;
			}				
		}
		i = ssave(game->mat,game -> cols, game -> rows, data -> path,game->mode_status);
		if (i == -1) {printf("failed to save file\n"); return;}
		printf("file saved succesfully\n"); return;
	}

         


	
}

#undef SOLVE
#undef EDIT
#undef MARK_ERRORS
#undef PRINT_BOARD
#undef SET
#undef VALIDATE
#undef GUESS
#undef GENERATE
#undef UNDO
#undef REDO
#undef SAVE
#undef HINT
#undef GUESS_HINT
#undef NUM_SOLUTIONS
#undef AUTOFILL
#undef RESET
#undef EXIT
#undef WRONG_COMMAND
#undef EMPTY_LINE
#undef LONG_COMMAND