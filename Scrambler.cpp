#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "Scrambler.h"

/*The constructor defintion for the Scrambler class
Takes in 2 parameters:
1. string constant parameter as a file path to a text file containing all possible words to use
2. a integer constant parameter for the size of the board*/
Scrambler::Scrambler(const string& path, const int size){

    this->board_size = size; //saves the board size to the data member board_size

    ifstream input_file{path}; //creates ifstream using the path parameter

    Square* current = head; //initializes a Square* pointer to the head junk node data member

    //creates a linked list with the required number of nodes needed to represent the game board
    for (int i = 0; i < board_size * board_size; i++){
        current->next = new Square(' '); //initializes each node's character data member to a whitespace
        current = current->next;
    }

    current = head->next; //sets current to first node of linked list

    vector<string> word_vec; //delcares a vector of type string

    //reads in every line of input_file and pushes each line to word_vec
    for (string word; getline(input_file, word);){
            word_vec.push_back(word);
    }

    //the process in which words are selected and placed onto the board through the linked list
    for (int row = 0; row < board_size; row++){
        int space_remain = board_size, column = 0;

        //continues to select random words from word_vec as long as there are at least 3 spaces remaining in a row, 
        //or board_size is 3 and the end of the row hasn't been reached
        while (column < board_size - 3 || (board_size == 3 && column < board_size)){
            string rand_word = word_vec.at(rand() % word_vec.size()); //randomly selects a word from word_vec

            if (space_remain != 3 && board_size - 1 >= rand_word.size() + column){ //checks if there is enough space remaining in the row to insert the word

                this->words_present.push_back(rand_word); //if the word fits, pushes the word to data member words_present

                if (column > 1){ //checks if a word is already in the row. if so, skips one node to have a space between every word
                    space_remain--;
                    column++;
                    current = current->next;
                }
                
                //assigns each node's character member to each letter in the word
                for (char character: rand_word){
                    current->letter = character;
                    current = current->next;
                    space_remain--;
                    column++;
                }

            }

            else if(space_remain <= 3 && board_size != 3){ //if the current row only has 3 columns remaining, skips to the next row
                space_remain = 0;

                for (int i = 0; i < 3; i++){
                    current = current->next;
                }
            }

            else if(board_size == 3 && rand_word.size() == 3){ //for cases board_size is 3, only words 3 letters long will be used

                //repeats the process of inserting the word
                words_present.push_back(rand_word);
                for (char character: rand_word){
                    current->letter = character;
                    current = current->next;
                    column++;
                }
            }

            if (space_remain <= 3 && board_size != 3){ //if the row has run out of space to insert a word, skips to the next row
                
                for (int i = 0; i < space_remain; i++){
                    current = current->next;
                }
            }

        }
    }


    jumble_board(); //jumbles the board

    store_initial(); //stores the initial jumbled state of the board

}


//This function serves to jumble the board from its original, default state
void Scrambler::jumble_board(){

    int rand_int = rand() % 8; //selects a random integer between 0-7
    int nmoves = rand_int + 3; //the number of jumbling will be rand_int + 3

    //to be used to store the move to undo the jumbling
    char reverse_dir; 
    string reverse_move;

    //for loop to jumble the board
    for (int i = 0; i < nmoves; i++){

        int idx = rand() % board_size; //randomly chooses which row or column number to move

        if (idx % 2 == 0){ //if idx is even, a column will be shifted

            char dir = (rand() % 2 == 0) ? 'u' : 'd'; //randomly chooses which direction to shift in
            make_move_v(idx + 1, dir); //makes the move

            //creates the reverse command to undo the move
            reverse_dir = (dir == 'u') ? 'd' : 'u';
            reverse_move = "c" + to_string(idx+1) + reverse_dir;
            this->reverse_board_moves.insert(reverse_board_moves.begin(), reverse_move); //adds the move to the beginnning of the vector
        }

        else{ //same process, but shifting a row instead

            char dir = (rand() % 2 == 0) ? 'r' : 'l';
            make_move_h(idx + 1, dir);
            reverse_dir = (dir == 'r') ? 'l' : 'r';
            reverse_move = "r" + to_string(idx+1) + reverse_dir;
            reverse_board_moves.insert(reverse_board_moves.begin(), reverse_move);
        }
    }
}

//This function stores the initial state of the board
void Scrambler::store_initial(){
    Square* current = head->next;

    //traverses the linked list and pushes each letter of the node to the vector initial_pos
    while (current){
        this->initial_pos.push_back(current->letter);
        current = current->next;
    }
}

/*This function shifts a row
Takes in 2 parameters:
1. integer constant parameter indicating which row to shift
2. character constant parameter indicating the direction to shift in*/
void Scrambler::make_move_h(const int row, const char dir){

    //Square* pointers to be used
    Square* current = head;
    Square* start;
    Square* end;
    Square* previous1;
    Square* previous2;
    Square* temp;

    //traverses to the appropriate node for the start of the row, storing the node before the current node
    for (int i = 0; i < (row - 1) * board_size + 1; i++){
        previous1 = current;
        start = current->next;
        current = current->next;
    }

    current = start;

    //traverses to the appropriate node for the end of the row, storing the node before the current node
    for (int i = 0; i < board_size - 1; i++){
        previous2 = current;
        end = current->next;
        current = current->next;
    }

    switch(dir){

        //for cases the row will be shifted to the left
        case 'l':
        case 'L':

            //adjusts the linking of the nodes to shift the row to the left
            temp = start->next;
            previous1->next = temp;
            start->next = end->next;
            end->next = start;
            break;

        //for cases the row will be shifted to the right
        case 'r':
        case 'R':

            //adjusts the linking of the nodes to shift the row to the right
            temp = end->next;
            previous1->next = end;
            end->next = start;
            previous2->next = temp;
            break;
    }

}

/*This function shifts a column
Takes in 2 parameters:
1. integer constant parameter indicating which column to shift
2. character constant parameter indicating the direction to shift in*/
void Scrambler::make_move_v(const int col, const char dir){

    //Square* pointers to be used
    Square* current = head;
    Square* before_current1;
    Square* before_current2;
    Square* after_current1;
    Square* after_current2;
    Square* temp;

    //traverses to the approporiate node of the start of the column
    for (int i = 0; i < col; i++){
        before_current1 = current;
        current = current->next;
    }

    after_current1 = current->next; //stores which node comes after current

    switch(dir){

        //for cases the column will be shifted upwards
        case 'u':
        case 'U':
            temp = current; //stores the current node's address

            for (int i = 0; i < board_size - 1; i++){ //traverses to the next node on the column, storing the nodes before and after the current node
                for (int j = (i == 0) ? 0 : 1; j < board_size; j++){ 
                    before_current2 = current;
                    current = current->next;
                    after_current2 = current->next;
                }

                //adjusts the linking of the nodes to shift the column upwards
                before_current1->next = current;
                current->next = after_current1;
                before_current1 = before_current2;
                after_current1 = after_current2;
                current = after_current2;
            }

            //final assignment of the nodes
            before_current2->next = temp;
            temp->next = after_current2;
            break;

        //for cases the column will be shifted downwards
        case 'd':
        case 'D':
            temp = before_current1; //stores the before_current1 node's address

            for (int i = 0; i < board_size - 1; i++){ //traverses to the next node in the column, storing the nodes before and after the current node
                for (int j = 0; j < board_size; j++){
                    before_current2 = current;
                    current = current->next;
                    after_current2 = current->next;
                }

                //adjusts the linking of the nodes to shift the column downwards
                before_current2->next = before_current1->next;
                before_current1->next->next = after_current2;
                before_current1->next = current;

            }

            //final assignments of the nodes
            temp->next = current;
            current->next = after_current1;
            break;
    }


}

//This function returns the board as a string
string Scrambler::str() const{

    ostringstream full_board_str; //declares an ostringstream

    //writes the column numbers to full_board_str
    full_board_str << " ";
    for (int i = 1; i <= board_size; i++){
        full_board_str << "   " << i;
    }
    full_board_str << endl;

    int count = 0, row = 1;
    
    Square* current = head->next; //initializes current

    //while loop that traverses the linked list to write each row to full_board_str
    while(current){
        if (count == board_size){ //writes a "|" if the end of the row is reached
            full_board_str << "|\n";
            count = 0;
            row++;
        }

        if(count == 0){ //writes the row separators to full_board_str if 
            full_board_str << "   ";
            for (int i = 0; i < board_size * 4; i++){
                full_board_str << "-";
            }
            full_board_str << endl;
            full_board_str << row << " "; //writes the row number to full_board_str
        }

        //writes each character separater by a "|"
        full_board_str << "| " << current->letter << " ";
        current = current->next; //traverses the linked list
        count++;
    }

    //writes the final bottom line of the board to full_board_str
    full_board_str << "|\n";
    full_board_str << "   ";
    for (int i = 0; i < board_size * 4; i++){
        full_board_str << "-";
    }

    return full_board_str.str(); //returns full_board_str as a string
}

/*This function tries to make a move inputted by the user
Takes in one parameter:
A string constant parameter that represents the move*/
void Scrambler::try_move(const string& cmd){

    stringstream command{cmd}; //creates a string stream using cmd

    char row_Or_col, dir;
    int num_r_O_c;

    command >> row_Or_col; //reads in the first character of the move
    row_Or_col = toupper(row_Or_col); //capitializes the character

    if (row_Or_col == 'R' && cmd.size() == 1){ //checks if the move was to reset
        restart(); //resets the board
    }

    else if(row_Or_col == 'C' || row_Or_col == 'R'){ //checks whether the move is valid
        command >> num_r_O_c >> dir; //reads in the rest of command

        switch(row_Or_col){
            
            //for the case for columns
            case 'C':
                make_move_v(num_r_O_c, dir);
                break;
            
            //for the case for rows
            case 'R':
                make_move_h(num_r_O_c, dir);
                break;
        }
    }
    else{ //for invalid moves
        cout << "Invalid move. Try again\n";
    }
}

//This function resets the board to its initial jumbled state
void Scrambler::restart(){

    Square* current = head->next; //assigns current to the start of the linked list
    int idx = 0; //integer to mark which index to access

    //while loop to traverse the linked list
    while (current){
        current->letter = initial_pos.at(idx); //assigns letter to the element at index idx
        idx++;
        current = current->next;
    }

    cout << "Resetting board...\n\n";
}

//This function returns the step by step solution to the board as a string
string Scrambler::display_solution(){

    ostringstream full_solution; //declares ostringstream
    bool finished = false; //boolean flag variable to check whether the board is solved
    int count = 1;

    restart(); //resets the board
    full_solution << str() << endl; //writes the board to full_solution

    //a while loop that loops until the board is solved
    while (!finished){

        //writes the move with the proper formatting
        full_solution << "*** Move " << count;
        string move = reverse_board_moves.at(count - 1); //access the move from reverse_board_moves
        full_solution << " (" << move.at(0) << "," << move.at(1) << "," << move.at(2) << ")\n";

        try_move(move); //tries and makes the move
        full_solution << str() << endl; //writes the board state to full_solution

        count++;

        finished = is_over(); //checks if the board is solved
    }

    restart(); //resets the board

    return full_solution.str(); //returns full_solution as a string
}

//This function returns a vector that holds all the words unscrambled that are present on the board
vector<string> Scrambler::get_words() const{
    return this->words_present;
}

//This functino checks whether or not the board is solved by checking if all the words on the board are unscrambled
bool Scrambler::is_over() const{

    bool match = false; //flag variable to check if the board has a correct word
    vector<string> current_jumbled; //vector to hold all "words" on the board

    Square* current = head->next; //assigns current to the start of the linked list

    string current_word = "";
    char curr_letter;
    int column = 0; //keeps track of the current column number

    //while loop to traverses the linked list
    while (current){

        if (column == board_size || !current->next){ //check if the end of a row or whether the end of the linked list is reached
            column = 0;

            if (current_word != ""){ //checks whether if current_word is not empty

                if (!current->next){ //checks whether if the current node is the last node of the linked list
                        current_word += current->letter; //adds the letter to current_word
                }
                current_jumbled.push_back(current_word); //pushes current_word to current_jumbled
            }

            current_word = ""; //resets current_word

            //adds the current letter to current_word, as the current node will be skipped 
            curr_letter = current->letter; 
            current_word += curr_letter;
            column++;
        }

        else{

            curr_letter = current->letter;

            if (curr_letter != ' '){ //adds curr_letter to current_word if it is not a whitespace
                current_word += curr_letter;
            }

            else if(curr_letter == ' ' && current_word != ""){ //if it is a whitespace and current_word is not empty, current_word is pushed to current_jumbled
                current_jumbled.push_back(current_word);
                current_word = "";
            }
            column++;
        }

        current = current->next; //traverses to next node
    }

    //ensures that leading and trailing whitespaces are removed from each word in current_jumbled
    for (int i = 0; i < current_jumbled.size(); i++){
        string word = current_jumbled.at(i);

        while (word.at(0) == ' '){
            word.erase(0, 1);
        }

        while(word.at(word.size() - 1) == ' '){
            word.erase(word.size() - 1);
        }

        current_jumbled.at(i) = word;
    }

    //gets all the words present on the board unscrambled
    vector<string> unscrambled = get_words();

    if (current_jumbled.size() != unscrambled.size()){ //immediately returns false if the number of words are not the same between the two vectors
        return false;
    }

    //loops through each element of the two vectors and compares them
    for (int i = 0; i < current_jumbled.size(); i++){
        current_word = current_jumbled.at(i);
        match = false;

        for (string word : unscrambled){ //for each loop through unscrambled
            if (current_word == word){ //if a match is found, skips to the next word in current_jumbled
                match = true; //match is set to true
                break;
            }
        }
        if (match == false){ //if there is at least one word with no match, returns false
            return false;
        }
    }

    return true; //if all previous return statements do not run, it is presumed the board is solved and true is returned
}