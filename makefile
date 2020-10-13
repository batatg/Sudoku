CC = gcc
OBJS = main.o ComStruct.o LinkedLst.o Commands.o Solver.o EzerCom.o Liner.o
 
EXEC = sudoku-console
COMP_FLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all:$(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $(EXEC)
main.o: main.c ComStruct.h Commands.h LinkedLst.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
ComStruct.o: ComStruct.h ComStruct.c LinkedLst.h Commands.h EzerCom.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c 
LinkedLst.o: LinkedLst.h LinkedLst.c
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c 
Commands.o: Commands.h Commands.c LinkedLst.h EzerCom.h Liner.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c 
Solver.o: Solver.h Solver.c
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c 
EzerCom.o: EzerCom.h EzerCom.c LinkedLst.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c 
Liner.o:  Liner.h Liner.c Solver.h 
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c 
clean:
	rm -f *.o $(EXEC)




