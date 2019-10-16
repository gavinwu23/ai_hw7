/*
Gavin Wu 15 puzzle program
CS 411 homework 6 A*
-number of misplace tiles
- manhatten distance


*/

#include<iostream>
#include <vector>
#include <queue>
#include "Puzzle_node.h"
#include <chrono>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stack>
#include <algorithm>


// TODO:


// maybe optimize it to make it less memory and nodes?
// some puzzles take more than 30 second to solve but i have a time that times out after 30 seconds hmmmm
//



// my own test case:
// 1 0 2 4 5 7 3 8 9 6 11 12 13 10 14 15
// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// 1 2 3 4 5 6 7 8 9 10 11 12 0 13 14 15
// 1 2 3 0 4 5 6 7 8 9 10 11 12 13 14 15
// 1 0 3 4 5 2 6 8 9 10 7 11 13 14 15 12

bool my_compare(const Puzzle_node*a, const Puzzle_node*b); // forward type decl to make compiler happy

//==================================================================================================================================================

// create the goal,intial state
void create_states(  int goal[BOARD_ROW][BOARD_COL], int starting_state_board[BOARD_ROW][BOARD_COL], const std::vector<int> &input_list) {

    // now we make the starting state
    int counter =0;

    // now starting_state_board holds the values
    for(int i=0; i < BOARD_ROW; i++) {
        for (int j=0; j < BOARD_COL; j++) {
            starting_state_board[i][j] = input_list[counter];
            counter++;
        }
    }

    // goal state should be here, but i moved it within the node class to make it easier to acces

}

//===============================================================================================================================
// A* to find the puzzle node
// will generate puzzles to keep looking for the goal state
// uses misplaces number of tiles

// look inside Puzzle_node.cpp to find how i did misplace tiles and manhatten distance
Puzzle_node *a_star(  Puzzle_node *initial_state, const std::vector<std::string> &directions, int &nodes_made, std::string flag) {

    clock_t tStart = clock(); // start the clock for 30 seconds

    // list to hold the new states then calculate misplace tiltes
    std::vector<Puzzle_node*> mis_list;

    // add itital state
    mis_list.push_back(initial_state);

    // perform the bfs algo
    while (true) {

        Puzzle_node *p1 = mis_list[0]; // grab first element
        mis_list.erase(mis_list.begin()); // pop it off

        // check if the puzzle reached goal state
        if(p1->reached_goal_state()) {
            return p1;
        }

        // if it reaches 30 seconds we just stop it
        auto time_so_far = (double)(clock() - tStart)/CLOCKS_PER_SEC;
        if (time_so_far > 30) {
            Puzzle_node *dummy = new Puzzle_node(5);
            return dummy;
        }

        // loop through directions vector to make moves on the board
        for (int i=0; i < directions.size(); ++i) {

            // check if that piece can go in that direction
            if (p1->valid_direction(directions[i])) {

                // make new puzzle based on going into that direction
                Puzzle_node *p2 = new Puzzle_node(*p1);
                nodes_made++;
                //std::cout << p2->depth << std::endl;

                // store the parent node from p2
                p2->parent = p1;

                // make the move
                p2->move_direction(directions[i]);

                // check if were doing misplace tiles OR manhatten distance
                if (flag == "manhatten") {
                    //std::cout << "manhattan being called" << std::endl;
                    p2->calculate_manhattan_distance(); // calculate manhatten distance as well heruistics
                }
                else if (flag == "misplaceTILES") {
                    //std::cout << "misplace tiles being clled" << std::endl;
                    p2->calculate_misplace_tiles();  // calculate misplace tiltes as well as the heruistics
                }

                // add it to vector of lists
                mis_list.push_back(p2);
            }
        } // for

        // now sort the list based on heristics, we want the lowest heristics to be the top
        std::sort(mis_list.begin(), mis_list.end(), my_compare);

    } // while

    Puzzle_node *no_solution = new Puzzle_node (10);
    return no_solution; // empty?

}

//=========================================================================================================================
// bool compare function to pass into std::sort
// we want the lowest heuristics to come first
bool my_compare( const Puzzle_node* a, const Puzzle_node *b) {

    // if left side is bigger than right side, swap, false means we want it to swap
    // if (a->heruistics > b->heruistics) {
    //     return false;
    // }
    return (a->heruistics < b->heruistics);
}
//============================================================================================================================

// intilize the static variable in puzzle node class
// not used in a*
int Puzzle_node::node_count = 0;

//========================================================================================================================
int main()
{

    std::cout << "Enter the board information...16 numbers" << std::endl;
    std::cout << "< ";

    clock_t tStart = clock(); // start the clock

    // get the input
    int number;
    std::vector<int> input_list; // create vector to store the entire board

    // get all the 16 numbers
    for (int i=0; i < BOARD_SIZE; i++) {
        std::cin >> number;
        input_list.push_back(number);
    }

    // intilize the puzzle boards
    int starting_state_board [BOARD_ROW][BOARD_COL];
    int goal_state[BOARD_ROW][BOARD_COL];
    create_states(goal_state, starting_state_board, input_list);

    // create a list of directions, 4 directions
    std::vector<std::string>directions;
    directions.push_back("UP");
    directions.push_back("DOWN");
    directions.push_back("RIGHT");
    directions.push_back("LEFT");

    int nodes = 1; // count how many nodes were created

    // create a puzzle object
    Puzzle_node *start = new Puzzle_node(starting_state_board);

    Puzzle_node *solution = a_star(start,directions,nodes, "misplaceTILES"); // run a* misplace tiles

    std::cout << std::endl << "Number of misplaced tiles" << std::endl;

    // get all the moves which lead to the solution
    std::cout << "Moves: " <<  solution->get_moves() << std::endl;
    std::cout << "Number of Nodes expanded: " << nodes << std::endl;

    // get the total time
    auto time_so_far = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    std::cout << "Time Taken: " << time_so_far << "s" << std::endl;

    // get the amount of memory used
    int mem = RUSAGE_SELF;
    struct rusage resource_usage; // linux get resource usage from man page
    int get_total_mem = getrusage(mem,&resource_usage);
    std::cout<< "Memory Used: " <<resource_usage.ru_maxrss << "kb" << std::endl; // get the max resident size


    //===========================================================
    //=========================================================== manhatten stuff
    int nodes_created = 1;
    Puzzle_node* start1 = new Puzzle_node(starting_state_board);
    Puzzle_node* manhattan_sol = a_star(start1,directions,nodes_created, "manhatten");

    std::cout << std::endl << "Manhattan Distance" << std::endl;

    // get all the moves which lead to the solution
    std::cout << "Moves: " <<  manhattan_sol->get_moves() << std::endl;
    std::cout << "Number of Nodes expanded: " << nodes_created << std::endl;

    // get the total time
    auto time_so_far1 = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    std::cout << "Time Taken: " << time_so_far1 << "s" << std::endl;

    // get the amount of memory used
    int mem1 = RUSAGE_SELF;
    struct rusage resource_usage1; // linux get resource usage from man page
    int get_total_mem1 = getrusage(mem1,&resource_usage1);
    std::cout<< "Memory Used: " <<resource_usage1.ru_maxrss << "kb" << std::endl; // get the max resident size

    return 0;
}
