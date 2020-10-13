#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Solver.h"

/*function to create a stack of given capacity. It initializes size of 
  stack as 0*/ 
struct Stack* createStack(int capacity) { 
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack)); 
    stack->capacity = capacity; 
    stack->top = -1; 
    stack->array = (int*)malloc(stack->capacity * sizeof(int)); 
    return stack; 
} 
  
/* Stack is full when top is equal to the last index*/ 
int isFull(struct Stack* stack) 
{ 
    return stack->top == stack->capacity - 1; 
} 
  
/* Stack is empty when top is equal to -1 */
int isEmpty(struct Stack* stack) 
{ 
    return stack->top == -1; 
} 
  
/* Function to add an item to stack.  It increases top by 1*/ 
void push(struct Stack* stack, int item) 
{ 
    if (isFull(stack)) 
        stack->capacity *= 2;
        stack->array = (int *)realloc(stack->array, stack->capacity * sizeof(int)); 
    stack->array[++stack->top] = item; 
} 
  
/* Function to remove an item from stack.  It decreases top by 1*/ 
int pop(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return 0; 
    return stack->array[stack->top--]; 
} 
  
/* Function to return the top from stack without removing it*/ 
int peek(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return 0; 
    return stack->array[stack->top];}

 

int check_valid(int* lst,int* counter,int N){  /* helper function, checks if in a list of 9 elments there are repetitions of numbers from 1 to 9 mod 10. */
  int i, num;
  for(i=0;i<N;i++){counter[i]=0;}
  for(i=0;i<N;i++){
    num=lst[i]%(N+1);
    if(num!=0){
	counter[num-1]=counter[num-1]+1;
        if(counter[num-1]>1){return 0;}}}
  return 1;
}

int choose_num_det(int* pb, int N){ /*for deterministic backtracking, chooses the next number to try to fill in the board.*/
  int i;
  for(i=0;i<N;i++){if(pb[i]==0){return i;}}
  return -1;
}

int full(int* pb,int N){ /*helper function, checks if a list of 9 elements is only 1's.*/
  int i;
  for(i=0;i<N;i++){if(pb[i]==0){return 0;}}
  return 1;
}

int check_valid_col(int** board, int num, int* col, int* lst,int N){ /* checks if coloum $num is valid*/
  int i;
  for(i=0;i<N;i++){col[i]=board[i][num];}
  return check_valid(col, lst,N);
}

int check_valid_row(int** board, int num, int* lst,int N){/* checks if row $num is valid*/
  return check_valid(board[num],lst,N);
}

int check_valid_box(int**board,int num, int* box, int* lst,int N,int n,int m){/* checks if box $num is valid*/
  int i;
  for(i=0;i<N;i++){box[i]=board[n*(num/n)+i%n][m*(num%n)+i/n];}
  return check_valid(box,lst,N);
}

void box_lst(int n, int m, int num, int* lst, int** board){
  int i;
  for(i=0;i<n*m;i++){lst[i]=board[n*(num/n)+i%n][m*(num%n)+i/n];}
}

int box_index(int n, int m, int pos){
  int x=pos/(n*m), y=pos%(n*m), num=0;
  num=(x/n)*n+y/m;
  return num;
}


int check_valid_full(int** board,int* lst1 ,int* lst2, int pos, int N, int n, int m){/*checks if the board is valid*/
  int i;
  if(pos==-1){
    for(i=0;i<N;i++){
      if(check_valid_col(board,i,lst1,lst2,N)==0 || check_valid_row(board,i,lst2,N)==0 || check_valid_box(board,i,lst1,lst2,N,n,m)==0){return 0;}
    }}
  else{if(check_valid_col(board,pos%N,lst1,lst2,N)==0 || check_valid_row(board,pos/N,lst2,N)==0 || check_valid_box(board,(pos/(n*n*m))*n+(pos%(n*m))/m,lst1,lst2,N,n,m)==0){return 0;}}
  return 1;
}
/*checks if the board is valid accordingly to the usage 
  of count_sol (if it is a board generator or sol counter)*/
int check_valid_full_caller(int** board,int* lst1 ,int* lst2, int pos, int N, int n, int m, int* prem,int gen,int** mat){
  int i,num;
  if(gen>0){
  for(i=0;i<gen;i++){num=prem[i];mat[num/N][num%N]=board[i/N][i%N];}
  return check_valid_full(mat, lst1, lst2, prem[pos], N,n,m);}
  else{return check_valid_full(board, lst1, lst2, pos, N,n,m);}}
 


void clean(int *lst,int N){/* helper function, make a list full of 0's.*/
  int i;
  for(i=0;i<N;i++){lst[i]=0;};}  

int backtrack(int pos, int*** pb, int** board, int before_fixed, int N,struct Stack* stack){  /* preforms backtracking if needed   */
  int curr_num;
  free(stack->array);
  free(stack);
  stack=createStack(N);
  while(1){
    push(stack,pos); 
    if(pos<0){return -1;} /* if backtracked until the start, there are no more solutions*/
    if(board[pos/N][pos%N]>N+1){pos--;before_fixed=1;continue;}  /* if fixed go one pos back*/
    if(before_fixed==1){
      pb[pos/N][pos%N][board[pos/N][pos%N]-1]=1;
      board[pos/N][pos%N]=0;    
}
    if(full(pb[pos/N][pos%N],N)==0){ /* continue backtracking only if there are no options for the current cell */
      return pos;}
    clean(pb[pos/N][pos%N],N);
    pos--;
    if(pos<0){return -1;}
    if(board[pos/N][pos%N]>N+1){pos--;before_fixed=1;continue;}
    curr_num=board[pos/N][pos%N];
    pb[pos/N][pos%N][curr_num-1]=1; /* mark that the current number for the current cell does not solve the board */
    board[pos/N][pos%N]=0;
    before_fixed=0; /*continue backtracking */
}}
void free_all(int* lst1,int* lst2,int* lst3, int*** pb, int N){/* frees all allocated memory */
  int i, j;
  free(lst1);
  free(lst2);
  free(lst3);
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      free(pb[i][j]);}
  free(pb[i]);}
  free(pb);
}

void free_all_big(int* lst1,int* lst2,int* lst3, int*** pb, int N,struct Stack* stack){/* frees all allocated memory */
  int i, j;
  free(lst1);
  free(lst2);
  free(lst3);
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      free(pb[i][j]);}
  free(pb[i]);}
  free(pb);
  free(stack->array);
  free(stack);
}



int choose_num_rand(int** board,int* pb, int* lst1, int* lst2, int* options, int pos, int N,int n,int m,int gen, int* prem, int** mat){/*for random backtracking, chooses the next number to try to fill in the board.*/
  int sum=0, i=0,j=0;
  for(i=0;i<N;i++){
      if(pb[i]==0){
     board[pos/N][pos%N]=i+1;
     /*sum++;options[j]=i;j++;*/
     if(check_valid_full_caller(board,lst1, lst2,pos,N,n,m,prem,gen,mat)){
           sum++;options[j]=i;j++;} /*makes the list we rand a number from*/
     else{pb[i]=1;}
}} 
  board[pos/N][pos%N]=0;
  if(sum==1){return options[0];}
  if(sum==0){return -1;}
  return options[rand()%sum];
}
 

int backtrack_main(int** board, int type,int n, int m,int* prem,int gen){ /*the main function to generate a soduko, returns 1 if solveable, 0 otherwise, gen>0 iff prem is relevent 
 (only if you need a premutation to generate the sudoku). */ 
  int pos=0,i=0, j=0,k=0,decision=0, N=n*m, counter=0, fixed_num=0;
  int* lst1=(int*) malloc(N*sizeof(int)); /* allocated lists for uses in other functions */
  int* lst2=(int*) malloc(N*sizeof(int));
  int* lst3=(int*) malloc(N*sizeof(int));
  int** mat=(int**) malloc(N*sizeof(int*));
  int* possibilities;
  int*** possible_board =(int ***) malloc(N* sizeof(int**));
  struct Stack* stack=createStack(N);
  for(i=0;i<N;i++){
    mat[i]=(int *)malloc(N*sizeof(int));
    for(j=0;j<N;j++){
      mat[i][j]=0;
}}
  for(i=0;i<N;i++){
    possible_board[i]=(int **)malloc(N*sizeof(int*));
    for(j=0;j<N;j++){
      possible_board[i][j]=(int*) malloc(N*sizeof(int));
        for(k=0;k<N;k++){
          possible_board[i][j][k]=0;}}} /*this 3D matrix represents for each cell the numbers we can not put in it, in the current state of the board */
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      if(board[i][j]!=0){
        if(i*N+j<gen+fixed_num){fixed_num++;}
        if(board[i][j]<N+1){
        board[i][j]=board[i][j]+3*(N+1);}}}} /* a way of marking the not fixed numbers that were in the board before the function (relevent for checking if the board is solvable) */
  if(check_valid_full_caller(board,lst1,lst2,-1,N,n,m,prem,gen,mat)==0){free_all_big(lst1, lst2, lst3, possible_board,N,stack);return 0;}
  while(pos<N*N+1){  /* main loop*/
    if(!isEmpty(stack)){pos=pop(stack);}
    if(gen>0 && pos>=gen+fixed_num){return 1;} /*if backtracking for generate, finish when you have filled x cells */
    if(pos==N*N){ /* if youre in the end of the sudoku, backtrack again and save the solution*/ 
      pos--;
      counter++;
      pos  = backtrack(pos,possible_board,board,1,N,stack);	
}
    if(board[pos/N][pos%N]<N+1){ /* dont change the numbers that were on the board*/
    possibilities=possible_board[pos/N][pos%N];
    if(type==0){decision=choose_num_rand(board,possibilities,lst1,lst2,lst3,pos,N,n,m,gen,prem,mat)+1;} /*type represents if the backtracking is random or deterministic*/
    else{decision=choose_num_det(possibilities,N)+1;}
    board[pos/N][pos%N]=decision; 
    if(decision!=0 && check_valid_full_caller(board,lst1,lst2,pos,N,n,m,prem,gen,mat)){pos++;} /*chooses the number to put in the cell, if valid go on.*/
    else{
      board[pos/N][pos%N]=0;
      if(decision!=0){possibilities[decision-1]=1;} /*mark the cell as not valid*/
      pos=backtrack(pos,possible_board,board,0,N,stack); /*check if bactracking is needed, if it is needed do it.*/
      if(pos<0){
        free_all_big(lst1, lst2, lst3, possible_board,N,stack);
        return counter;
}}}
   else{ /* if the cell was filled before*/
     pos++;
}}
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      if(board[i][j]>3*(N+1)){board[i][j]=board[i][j]-3*(N+1);}}} /*unmark the numbers that were on the board */
  free_all_big(lst1, lst2, lst3, possible_board,N,stack);
  return counter;  
}


int count_solutions(int** board,int n, int m){
  int i=0;
  return backtrack_main(board,1, n, m ,&i, i); 
}

