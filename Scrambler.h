/*
The header file that declares the Scrambler class and Square class,
along with all their related member functions and data members
*/


#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

#ifndef SCRAMBLER_H
#define SCRAMBLER_H


//delcares the Square class
class Square{
    public:

        /*
        declares and defines the constructor for the class
        Takes in two paramters, a character and pointer to an object of the Square type
        */
        Square(char letter, Square* next = nullptr){

            //assigns the data members accordingly
            this->letter = letter; 
            this->next = next;
        }

        Square(){}

        //declares data members of the class
        char letter; 
        Square* next = nullptr;
};

//declares the Scrambler class
class Scrambler{

    //public functions that are used to run the word scrambler game
    public:
        Scrambler(const string& path, const int size); //constructor for the class
        string str() const; //a function that returns the state fo the board
        void try_move(const string& cmd); //a function that takes in a command and moves the board
        string display_solution(); //a function that outputs the solution to the game
        vector<string> get_words() const; //a function that returns the words the user is trying to get
        void restart(); //a function that restarts the game back to the initial scrambled state
        bool is_over() const; //a function that checks whether the game is over

    //private functions and data members that are used to aid in the game's process
    private:
        void jumble_board(); //a fucntion that jumbles the board into it's initial state the user will see
        void store_initial(); //a function stores the initial scrambled state of the board
        void make_move_h(const int row, const char dir); //a function that shifts a row left or right
        void make_move_v(const int col, const char dir); //a function that shifts a coloumn up or down
        int board_size; //stores the board size the user chooses
        Square* head = new Square{'?'}; //creates a junk node to serve as a head to a linked list
        vector<char> initial_pos; //used to store the initial scrambled position of the board
        vector<string> reverse_board_moves; //used to store the moves that will unscrambled the board: the solutions
        vector<string> words_present; //used to store all the scrambled words that are on board


};

#endif