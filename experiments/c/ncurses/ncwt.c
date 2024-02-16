#include <ncurses.h>
#include <unistd.h>
// ncurses and getcwd. Baaaasics of a filemanager

int main(){
    initscr();
    raw();


    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
        printw("%s\n", cwd);
    else{
        printw("Working dir not found");
        return 0;
    }
    
    char nuch; 
    int pos = 0;
    while(nuch != '0'){
        nuch = getch();
        move(0,pos);
        addch(nuch);
        pos++;
    }
    
    endwin();                   

    return 0;
}
