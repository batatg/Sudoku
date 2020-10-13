/*
 * EzerCom.h
 *
 *  Created on: 25 במאי 2019
 *      Author: אוהד גזית
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "LinkedLst.h"



#ifndef EZERCOM_H_
#define EZERCOM_H_



int bigger(double * k,int N, float X,double* to_rnd);
int weighted_rnd_pick( double * to);
int pick_cell(int N, double * to);
int pick_value_for_guess(double *k,int N, float X);
int sload_mat(int** mat,char* path, int n, int m,int type);
int sload_size(char* path,int** mat);
void find_valid_nums(int **mat,int n,int m,int row_index,int col_index,int *  options);
int find_one_valid(int N, int* opt);
int is_erroneous(int** mat,int N,int row_index,int col_index,int n,int m,int before);
int check_erroneous_fixed(int** mat, int N,int row_index,int col_index);
int find_erroneous_col(int **mat,int N,int row_index,int col_index);
int find_erroneous_row(int** mat,int N, int row_index,int col_index);
int find_erroneous_box(int** mat,int N, int row_index,int col_index,int n,int m);
void find_erroneous(int** mat,int N,int row_index,int col_index,int n,int m,int before);
void fill_lists(int** mat,int N,int n,int m,int* row,int* col,int* cube,int row_index,int col_index);
int hlp_mat_check_row(int** mat,int N, int row_index,int col_index,int calling_command);
int hlp_mat_check_col(int** mat,int N, int row_index,int col_index,int calling_command);
int hlp_mat_check_box(int** mat,int N, int row_index,int col_index,int n, int m,int calling_command);
int mat_file_check(int** mat,int N,int n,int m,int status);
void mat_changes_updater(int** old,int** new ,lnkd* stage,int N);
void clean_3d_mat(double*** mat,int N);
#endif /* EZERCOM_H_ */
