#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <vector>

#include "Scrambler.h"

using namespace std;

//This function displays the menu options the user can pick
void menu(){
    cout << "Enter a move, or\n"
         << "(S) See solution. Board will be resetted\n"
         << "(R) Reset the board\n"
         << "(W) See what the unscrambled words are\n"
         << "Or enter exit to quit the game\n";
}

/*This function outputs the words unscrambled on the board
Takes in one parameter:
A Scrambler object by reference*/
void get_words(Scrambler& game){
    vector<string> words;
    words = game.get_words(); //uses the Scrambler member function get_words() to retrieve the unscrambled words

    //outputs the words formatted
    for (int i = 0; i < words.size(); i++){
        cout << i + 1 << ". " << words.at(i) << endl;
    }
    cout << endl;
}

int main(){
    
    int board_size;
    string cmd;

    //displays the instructions for the game
    cout << "This is the Word Scrambler puzzle game\n"
         << "Your objective is to unscramble the board by repeatedly shifting a row or column\n"
         << "Moves are inputted as 3 characters:\n"
         << "1. A letter for row or column\n"
         << "2. A number for the row or column number\n"
         << "3. Another letter for direction\n"
         << "For example, \"r1r\" will move row 1 to the right\n"
         << "\nTo start, please enter a board size from 3-9: ";

    cin >> board_size; //reads in board size

    //while loop to ensure a valid number is read
    while (board_size < 3 || board_size > 9){
        cout << "Invalid board size. Please try again (3-9): ";
        cin >> board_size;
    }

    Scrambler board{"dictionary.txt", board_size}; //creates a Scrambler object using dictionary.txt and board_size

    cout << board.str() << endl; //outputs the board
    
    menu(); //displays the menu

    cin >> cmd; //read in a move

    //while loop to continously read in moves until the user exits
    while (cmd != "Exit" && cmd != "exit"){
        
        if (cmd == "S" || cmd == "s"){ //checks whether the user wants the solution

            //outputs solution
            cout << "The solution is: \n"
                 << board.display_solution()
                 << "End of solution\n"
                 << "Resetting board...\n\n"
                 << board.str() << endl;
        }

        else if (cmd == "W"){ //checks whether the user wants to see the words
            get_words(board);
        }
        else if (cmd != "Exit" && cmd != "exit"){ //for all other inputs
            board.try_move(cmd);
            cout << board.str() << endl;
        }
        

        //displays the menu and read input again
        menu();
        cin >> cmd;
        cout << endl;

    }

    //end message
    cout << "GAME OVER\n";
    cin >> cmd;
    return 0;
}
