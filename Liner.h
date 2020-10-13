/*
 * Liner.h
 *
 *  Created on: 25 במאי 2019
 *      Author: אוהד גזית
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Solver.h"
#include "gurobi_c.h"



#ifndef LINER_H_
#define LINER_H_


int lp(int N,int n, int m, int** mat,double*** res,int type);
int find_solution(int N,int n, int m, int** mat);

#endif /* LINER_H_ */
