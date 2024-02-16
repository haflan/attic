#include <ncurses.h>

int main(){
    initscr();
    raw();                      //  Does NOT let you exit program with Ctrl-C. use cbreak for exit functionality
    int testvar = 4;
    printw("This is bog standard string output %d", testvar);
    addch('a');                 // Writes chars to the screen.
    move(12, 13);               // Moves the cursor on the screen
    //   ^y  ^x

    mvprintw(15, 20, "Movement");   // Move cursor, then print at cursor.
    mvaddch(12,50, '@');            // Move cursor, then add character.
    getch();
    endwin();                   //  Frees the memory in the screen and closes ncurses. "destructor"

    return 0;
}
