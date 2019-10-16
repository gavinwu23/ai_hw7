/*
Gavin Wu 15 puzzle program
CS 411 homework 7 idda*
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


// global vector to hold all the heruistics that go over the threshold
std::vector<int> low_values;


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

//============================================================================================================================

// iterative deepening depth firt search
// we will use a iterative DFS to make it simple

Puzzle_node* ID_a_star ( Puzzle_node* initial_state, const std::vector<std::string> &directions, int &nodes_made, int max_value, std::string flag) {

    // clear vector of heruistics
    low_values.clear();

    // create a stack to help us perform the dfs
    std::stack <Puzzle_node*> frontier;

    // create a vector to keep track of already same state
    std::vector<std::string> visisted_state;

    // keep track of visisted or not
    bool vis = false;

    clock_t tStart = clock(); // start the clock for 30 seconds

    // add the intial state to the stack
    frontier.push(initial_state);
    // keep going until the stack is empty
    while (!frontier.empty()) {

        vis = false; // rest to false

        // grab the top  element and pop it off
        Puzzle_node* p1 = frontier.top();
        frontier.pop();

        // get the string representation of the board
        std::string c_state = p1->convert_to_string();
        // check if we already reached this state, if we did then skip it other else store that state
        for (std::string s : visisted_state) {
            if (s == c_state ) {
                vis = true;
            }
        }

        // found a matching state so we just skip it and move on
        if (vis == true) {
            continue;
        }
        else { // store the board
            visisted_state.push_back(c_state);
        }

        // check if the goal state is reached
        if(p1->reached_goal_state()) {
            return p1;
        }

        // if it reaches 30 seconds we just stop it
        // auto time_so_far = (double)(clock() - tStart)/CLOCKS_PER_SEC;
        // if (time_so_far > 60) {
        //     Puzzle_node *dummy = new Puzzle_node(5);
        //     return dummy;
        // }

        // check if the cutoff for heruistics is over.
        // if it is, we ignore it and then store that heristics value
        if (p1->heruistics > max_value) {
            // if it is bigger, store it in vector then find min of that vec to set new max value
            low_values.push_back(p1->heruistics);
            continue;
        }

        // loop through the direcion list
        for (int i=0; i < directions.size(); ++i) {

            // check for valid direction
            if (p1->valid_direction(directions[i])) {

                //make new puzzle..by coping it first
                Puzzle_node* p2 = new Puzzle_node(*p1);
                //std::cout << p2->depth << std::endl;
                // make move
                p2->move_direction(directions[i]);
                nodes_made++;

                p2->parent = p1;

                // check if were doing misplace tiles OR manhatten distance
                if (flag == "manhatten") {
                    p2->calculate_manhattan_distance(); // calculate manhatten distance as well heruistics
                }
                else if (flag == "misplaceTILES") {
                    //std::cout << "misplace tiles being clled" << std::endl;
                    p2->calculate_misplace_tiles();  // calculate misplace tiltes as well as the heruistics
                }

                frontier.push(p2);

            } // if valid dir

        }// for loop

    } // while

    return nullptr;

} // function



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

    int start_depth = 0;
    Puzzle_node *solution = ID_a_star(start,directions,nodes, start_depth, "manhatten"); // run iddfs

    std::cout << "Calculating manhattan distance..." << std::endl;

    // continue to loop depth by depth
    while (!solution) {
        auto min_value = *std::min_element(low_values.begin(),low_values.end()); // grab the lowest value that we found
        start_depth = min_value;
        solution = ID_a_star(start,directions,nodes, start_depth, "manhatten");
    }

    std::cout << std::endl << "Manhattan Distance:" << std::endl;

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
    std::cout<< "Memory Used: " <<resource_usage.ru_maxrss << "kb" << std::endl << std::endl; // get the max resident size


    //===========================================================
    //=========================================================== manhatten stuff
    int nodes_created = 1;
    Puzzle_node* start1 = new Puzzle_node(starting_state_board);

    //Puzzle_node* manhattan_sol = ID_a_star(start1,directions,nodes_created, "manhatten");
    int start_depth1 = 0;
    Puzzle_node *manhattan_sol = ID_a_star(start1,directions,nodes_created, start_depth1, "misplaceTILES"); // run iddfs

    std::cout << "Calculating misplace tiles..." << std::endl;

    // continue to loop depth by depth
    while (!manhattan_sol) {
        auto min_value1 = *std::min_element(low_values.begin(),low_values.end()); // grab the lowest value that we found
        start_depth1 = min_value1;
        manhattan_sol = ID_a_star(start1,directions,nodes_created, start_depth1, "misplaceTILES");
    }

    std::cout << std::endl << "Number of misplace tiles" << std::endl;

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
    std::cout<< "Memory Used: " <<resource_usage1.ru_maxrss << "kb" << std::endl << std::endl; // get the max resident size

    return 0;
}
