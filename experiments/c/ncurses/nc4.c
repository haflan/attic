#include <ncurses.h>

int main(){
    initscr();
    raw();                      //  Does NOT let you exit program with Ctrl-C. use cbreak for exit functionality
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLUE);                // Corresponds to a pair of colors (foreground, background).
    attron(COLOR_PAIR(1));                              // Set the newly created color pair as attribute
    printw("It huuuurts!");                             //
    attroff(COLOR_PAIR(1));


    getch();
    endwin();                   //  Frees the memory in the screen and closes ncurses. "destructor"

    return 0;
}
