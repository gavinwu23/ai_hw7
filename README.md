Gavin Wu, gwu23, 6579388770
AI homework 6

I wrote this program in c++ 11. To compile and run this program
	type make 
	type ./main

then it will say Enter the board information...16 numbers
which then you enter the 16 numbers and then press enter.
	I'm gonna assume its good input, example no duplicated number like 1 1 1 3 4...etc 

i used a linux enviorment also to compile and run if that makes a difference.

After a few seconds for a working solvable puzzle, it will display 

Number of misplace Tiles
Moves:
Number of nodes Expanded:
Time taken:
memory used:

Manhattan
Moves:
Number of nodes Expanded:
Time taken:
memory used:

one uses calculation of a* with misplace tiles, and other with manhatten distance

most of the test cases from HW4, give out the same number of nodes expanded EXCEPT for the last one.
1 3 4 8 5 2 0 6 9 10 7 11 13 14 15 12
so manhattan distance seems to be better.


Code Explaination:

Inside Puzzle_node.cpp, i made 2 methods. calculate_misplace_tiles and calculate_manhattan_distance.

For the misplace titles, i compared the entire board(2d array) to the goal state and ignore the 0 spot. For each
iteration, if the number do not match to the goal state, i kept a book keeping variable to keep track of it.
Then i used my depth + misplaceVariable to get my final heuristics. 

For manhatten distance, i used the regular distance formula to find the distance. loop through the entire board find the distance for each spot, then
store the total distance in a variable. Then i used my depth + variable to get my final heuristics.

Depth is the distance from the intitial state to that state we got to, so basically kinda depth.


In main, there is a function a_star which does the search algo. I made a vector to hold all my nodes and then sort it everytime, by our final heuristics values.
I thought this was easier because it is straight forward, and a* looks at the lowest final value and goes from there. base code is similar to hw5,4