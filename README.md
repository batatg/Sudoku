# Sudoku
Interactive Sudoku game implemented in Advanced Programming course as a final project.
Using Linear rogramming methods in order to find possible board solutions.
User possible commands:
1. solve X - loading puzzle named X in solve mode
2. edit [X] - loading puzzle named X in edit mode (X is optional)
3. mark_errors X - set the board marking errors option
4. print_board 
5. set X Y Z - set the value of cell <X,Y> to Z
6. valitae - validate the current board usin Integer Linear Programming
7. guess X - Guess a solution to the current board using Linear Programming with threshold X
8. generate X Y - Generates a puzzle by randomly filling X empty cells with legal values, running ILP to solve the board, and then clearing all but Y random cells.
9. undo - Undo a previous move done by the user. 
10. redo - Redo a move previously undone by the user.
11. save X - saves the current board to the file with path X
12. hint X Y - showing the solution of the cell <X,Y> using ILP
13. guess_hint X Y show a guess for the cell <X,Y> using LP
14. num_soltuions - printing number of solutions for the current board
15. autofill - automatically filling "obvious" values
16. reset - undo all moves, reverting the board to its original loaded state.
17. exit
