#include "Puzzle_node.h"

// intial constructor to set up the board
Puzzle_node::Puzzle_node(int intitial_board[BOARD_ROW][BOARD_COL]) {

    // copy over the intitial_board to the current_puzzle_board
    for (int i=0; i < BOARD_ROW; i++) {
        for (int j=0; j < BOARD_COL; j++) {
            current_puzzle_board[i][j] = intitial_board[i][j];
        }
    }

    // now to find the blank spot, aka the 0 position and store it
    for(int i=0; i < BOARD_ROW; i++) {
        for(int j=0; j < BOARD_COL; j++) {
            if (current_puzzle_board[i][j] == 0) {
                x_position = i;
                y_position = j;
            }
        }
    }

    // create the goal state
    int incrementer = 1;
    for (int i=0; i < BOARD_ROW; i++) {
        for (int j=0; j< BOARD_COL; j++) {
            goal_state[i][j] = incrementer;
            incrementer++;
        }
    }

    // set the last spot to 0
    goal_state[BOARD_ROW-1][BOARD_COL-1] = 0;

    // this is considered as the root node so keep it nullptr
    parent = nullptr;

    node_count++;

} // constructor

//=================================================================================================================================

// copy constructor for when we create new nodes based on the bfs search
// then make it move connnecting it like a tree structure
Puzzle_node::Puzzle_node(const Puzzle_node &new_state) {

    // copy over the array
    // copy over the intitial_board to the current_puzzle_board
    for (int i=0; i < BOARD_ROW; i++) {
        for (int j=0; j < BOARD_COL; j++) {
            current_puzzle_board[i][j] = new_state.current_puzzle_board[i][j];
        }
    }

    // copy over x and y position of blank spots
    x_position = new_state.x_position;
    y_position = new_state.y_position;

    // copy over the current string of moves which lead to this point
    moves = new_state.moves;

    // copy over the same goal state
    for (int i=0; i < BOARD_ROW; i++) {
        for (int j=0; j < BOARD_COL; j++) {
            goal_state[i][j] = new_state.goal_state[i][j];
        }
    }

    depth = new_state.depth; // increase the depth since we made a new node
    depth++;

    // now we set to the current parent node
    // parent = new_state;
    node_count++;
}

//==================================================================================================================================
// we use this when the puzzleboard doesn't have a solution
Puzzle_node::Puzzle_node ( int x) {
    moves = "Solution not found";
}

//================================================================================================================================

// error checking methods to see if we got to the 0th spot correctly
int Puzzle_node::get_x_position( ) {
    return x_position;
}

int Puzzle_node::get_y_position () {
    return y_position;
}

//====================================================================================================================================

// method to check if the blank spot can move in that direction
// most cases there would be 4 directions, however
// sometimes only 2 or 3
// take string as paramater which indicates the directions

bool Puzzle_node::valid_direction(std::string direction) {

    // up direction...as long as x values is not 0 then it can go up
    if (direction == "UP") {
        if (x_position != 0) {
            return true;
        }
    }

    // donw direction..as longn as x not BOARD_ROW-1 (3) then it can go down
    else if (direction == "DOWN") {
        if (x_position != BOARD_ROW-1) {
            return true;
        }
    }

    // right direction. as long as y value not 3
    else if (direction == "RIGHT") {
        if (y_position != BOARD_COL-1) {
            return true;
        }
    }

    // finally left direction..as long as y not 0
    else if (direction == "LEFT") {
        if (y_position != 0) {
            return true;
        }
    }

    return false; // none matches
}

//========================================================================================================

// method that will change the position of the blank spot
// changing the puzzle board, takes string input as RIGHT<DOWN<LEFT<UP
// also need to update moves as it moves boards
// assuming we already checked if its a valid move we just swap as normal
void Puzzle_node::move_direction(std::string direction) {

    // going up
    if (direction == "UP") {
        // store the number above blankspot first
        int temp = current_puzzle_board[x_position-1][y_position];
        // perform the swap
        current_puzzle_board[x_position-1][y_position] = 0; // make the top blank
        current_puzzle_board[x_position][y_position] = temp; // make the original spot the number

        // update the new position of the blank aka 0
        x_position = x_position - 1;

        // update moves
        moves = moves + "U";

        // update the current string in which it will move in
        moved_in_this_dir = "U";
    }

    // going down
    else if (direction == "DOWN") {
        // store the number below blank spot
        int temp = current_puzzle_board[x_position+1][y_position];
        // swap
        current_puzzle_board[x_position+1][y_position] = 0; // make bottom blank
        current_puzzle_board[x_position][y_position] = temp; // make original spot temp

        // update new x y
        x_position = x_position + 1;

        // update moves
        moves = moves + "D";

        moved_in_this_dir = "D";
    }

    // going right
    else if (direction == "RIGHT") {
        // store temp right way
        int temp = current_puzzle_board[x_position][y_position+1];
        // swap
        current_puzzle_board[x_position][y_position+1] = 0; // make the to the right 0
        current_puzzle_board[x_position][y_position] = temp; // make the current spot = to the value

        // update y
        y_position = y_position + 1;

        // update the moves
        moves = moves + "R";

        moved_in_this_dir = "R";
    }

    // finally going left
    else if (direction == "LEFT") {
        // store temp of left value
        int temp = current_puzzle_board[x_position][y_position-1];
        // do swap
        current_puzzle_board[x_position][y_position-1] = 0; // set left value to 0
        current_puzzle_board[x_position][y_position] = temp; // make the current spot = to value

        // update y
        y_position = y_position - 1;

        // update moves
        moves = moves + "L";

        moved_in_this_dir = "L";
    }
}


//======================================================================================================

// just print the baord for debugging purposes
void Puzzle_node::print_puzzle_board( ) {
    // print the board
    for(int i =0; i < BOARD_ROW; i++) {
        for (int j=0; j < BOARD_COL; j++) {
            std::cout << current_puzzle_board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

//======================================================================================================

// if we reached the goal state or not
bool Puzzle_node::reached_goal_state( ) {
    // compare the curren state with the goal state
    // if one spot doesnt match..auto fail instant
    for (int i=0; i < BOARD_ROW; i++) {
        for (int j=0; j < BOARD_COL; j++) {
            if (current_puzzle_board[i][j] != goal_state[i][j]) {
                return false; // found mismatch
            }
        }
    }

    // reach here means everything checked out
    return true;
}

//============================================================================================================

// return the moves of the board that lead to the goal state
std::string Puzzle_node::get_moves( ) {
    return moves;
}

//================================================================================================================

// this method will convert the 2d int array to a string to check if we already reached the state
std::string Puzzle_node::convert_to_string( ) {
    std::string answer;
    for (int i=0; i < BOARD_ROW; i++) {
        for (int j=0; j < BOARD_COL; j++) {
            answer = answer + std::to_string(current_puzzle_board[i][j]);
        }
    }
    return answer;
}

//===================================================================================================================
// method which will calculate the number of misplace titles
void Puzzle_node::calculate_misplace_tiles ( ) {
    // loop through the 2d board
    for (int i=0; i < BOARD_ROW; i++) {
        for (int j = 0; j < BOARD_COL; j++) {

            // we ignore the 0 spot when finding the righ spot
            if (current_puzzle_board[i][j] != 0 ) {

                // now check the values, dont match increase mismatch values
                if (current_puzzle_board[i][j] != goal_state[i][j]) {
                    misplace_tiles = misplace_tiles + 1;
                }
            }

        } // inner for

    } // outer for

    // now calculate the heruistics
    heruistics = depth + misplace_tiles;

} // method

//===================================================================================================================
// method to calculate the manhattan distance
void Puzzle_node::calculate_manhattan_distance ( ){

    // loop thr 2d array
    for (int i=0; i < BOARD_ROW; i++){
        for (int j=0; j < BOARD_COL; j++) {

            // dont count the 0 spot
            if (current_puzzle_board[i][j] != 0) {
                int x_pos = (current_puzzle_board[i][j] - 1) /  4;
                int y_pos = (current_puzzle_board[i][j] - 1) % 4;
                manhatten_dis = manhatten_dis + abs(x_pos - i) + abs(y_pos - j); // manhatten distance formula
            } // if
        } // inner for;
    } // outer for

    // now calculate the heruistics
    heruistics = depth + manhatten_dis;


}
