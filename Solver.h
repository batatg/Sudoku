/*
 * Solver.h
 *
 *  Created on: 25 במאי 2019
 *      Author: אוהד גזית
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#ifndef SOLVER_H_
#define SOLVER_H_

struct Stack{ 
    int top; 
    int capacity; 
    int* array; 
} ;

struct Stack* createStack(int capacity);
int isFull(struct Stack* stack) ;
int isEmpty(struct Stack* stack) ;
void push(struct Stack* stack, int item) ;
int pop(struct Stack* stack) ;
int peek(struct Stack* stack) ;
int check_valid(int* lst,int* counter,int N);
int choose_num_det(int* pb, int N);
int full(int* pb,int N);
int check_valid_col(int** board, int num, int* col, int* lst,int N);
int check_valid_row(int** board, int num, int* lst,int N);
int check_valid_box(int**board,int num, int* box, int* lst,int N,int n,int m);
void box_lst(int n, int m, int num, int* lst, int** board);
int box_index(int n, int m, int pos);
int check_valid_full(int** board,int* lst1 ,int* lst2, int pos, int N, int n, int m);
int check_valid_full_caller(int** board,int* lst1 ,int* lst2, int pos, int N, int n, int m, int* prem,int gen,int** mat);
void clean(int *lst,int N);
int backtrack(int pos, int*** pb, int** board, int before_fixed, int N,struct Stack* stack);
void free_all(int* lst1,int* lst2,int* lst3, int*** pb, int N);
void free_all_big(int* lst1,int* lst2,int* lst3, int*** pb, int N,struct Stack* stack);
int choose_num_rand(int** board,int* pb, int* lst1, int* lst2, int* options, int pos, int N,int n,int m,int gen, int* prem, int** mat);
int backtrack_main(int** board, int type,int n, int m,int* prem,int gen);
int count_solutions(int** board,int n, int m);


#endif /* SOLVER_H_ */
