Gavin Wu, gwu23, 6579388770
AI homework 7

I wrote this program in c++ 11. To compile and run this program
	type make 
	type ./main

then it will say Enter the board information...16 numbers
which then you enter the 16 numbers and then press enter.
	I'm gonna assume its good input, example no duplicated number like 1 1 1 3 4...etc 

i used a linux enviorment also to compile and run if that makes a difference.

After a few seconds for a working solvable puzzle, it will display 

Manhattan
Moves:
Number of nodes Expanded:
Time taken:
memory used:

Number of misplace Tiles
Moves:
Number of nodes Expanded:
Time taken:
memory used:


I took out the timer for 30 seconds timeout because i wanted to solve the #2 new test case which is alot.

For the new test cases, test case 1 solves in about 3 seconds? but for new test case 2, it takes a LONG time, like about 20 minutes :(.
The old test cases still works perfectly.


Code Explaination:

I used my IDDFS from homework 5 because IDA* follows the same idea. Instead of checking the depth, i checked the f value, (heruistic value + distance 
to that node) as the cutoff. I made a global vector called low_values which stores the f values that surpasses the threshold, then i use the std::min to find the min
value which will be the new threshold and repeated until a solution is found.

I deleted the time out of 30 seconds because it works fast and i wanted to solve the new test case #2 which it did but it took a long time, about half a million nodes
generated. geez