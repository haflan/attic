#include <ncurses.h>

int main(){
    initscr();
    noecho();
    cbreak();                      

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW * menuwin = newwin(6, xMax - 12, yMax-8, 5);
    box(menuwin,0, 0);
    refresh();
    wrefresh(menuwin);

    char num;
    int posX=xMax - 30, posY=yMax - 5;
    while(num != '0'){
        num = getch();
        mvaddch(posY, posX, ' ');
        switch(num){
            case 'n':
                posY++;
                break;
            case 'e':
                posY--;
                break;
            case 'h':
                posX--;
                break;
            case 'i':
                posX++;
                break;
            default:
                break;
        }
        mvaddch(posY, posX, 'o');
        refresh();
        wrefresh(menuwin);
    }
    endwin();                  

    return 0;
}
