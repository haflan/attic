#include <ncurses.h>
// tutorial 3 - using attributes

int main(){
    initscr();
    raw();

    // Attribute:
    attron(A_STANDOUT | A_UNDERLINE);
    mvprintw(0, 40, "READ THIS NOW");
    attroff(A_STANDOUT | A_UNDERLINE);
    
    endwin();                   

    return 0;
}
