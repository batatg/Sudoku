/*
 * Commands.h
 *
 *  Created on: 25 במאי 2019
 *      Author: אוהד גזית
 */



#ifndef COMMANDS_H_
#define COMMANDS_H_
#include "LinkedLst.h"
#include "Solver.h"
#include "EzerCom.h"
#include "Liner.h"





typedef struct{
	int com_num;  			
	int error;  		
	char  path[260] ;
	int mark_errors;
	int set_param[3] ;
	
} com_data;
/*holds the game details*/
typedef struct{
	int mode_status;
	int cols;
	int rows;
	int N;
	int** mat;
	lnkd* stages;
} game_data;


void set_command (lnkd* stage,int ** board,int arr[],int before,int N,int n,int m);
void PrintScreen(int** mat, int n, int m, int N,int mark_errors,int mode_status); 
int ssave(int** mat, int n, int m, char* path,int edit);
int guess_command(lnkd* stage,int ** mat, int N,int n, int m,float X);
int validate_command(int **mat,int N,int n,int m,int validate);
int** file_load(/*int ** mat,*/char *path,int* lst1,int status);
int**  edit_command(int** mat,int N);
void hint_command(int ** mat,int m ,int n, int N,int row_index,int col_index);
void guess_hint_command(int** mat, int m, int n, int N,int row_index,int col_index);
void num_solutions_command(int** mat,int n,int m,int N);
int autofill_command(lnkd* stage,int** mat, int n, int m);
lnkd* undo_command(int** mat, lnkd* stage,int N);
lnkd* redo_command(int** mat, lnkd* stage,int N);
lnkd* reset_command(int** mat,lnkd* stage);
int generate(int** board,lnkd* stage,int N,int n, int m,int x, int y);

#endif /* COMMANDS_H_ */
