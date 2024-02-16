#include <ncurses.h>

int main(){
    initscr();                  //  Creates stdscr (Standard screen). "constructor"
    raw();                      //  Does NOT let you exit program with Ctrl-C. use cbreak for exit functionality
    printw("Hello world!");     //  Works pretty much as printf()
    getch();                    //  Waits for user to enter character (works as a pause)
    endwin();                   //  Frees the memory in the screen and closes ncurses. "destructor"

    return 0;
}
