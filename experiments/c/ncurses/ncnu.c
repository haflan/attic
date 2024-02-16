#include <ncurses.h>


// modes
#define INSERT 0
#define EDIT 1
// line number offset
#define LN_OFFSET 5

float step = 0.125;
int curX = 0;
int curY = 0;
int yMax, xMax;
int mode = INSERT;

/* owwd = owerwrite default */
bool owwd_note = true;
int default_note = 69;  //TODO: add option for displaying frequency, supercollider note etc.
bool owwd_dur = true;
int default_dur = 0.5;


// This will later be used to connect the instrument letters to actual names.
char* instlist[] = {"piano", "violin", "testins"};
char* get_instrument(char** instlist, char insnum) { return instlist[insnum - 'A']; }

//TODO: typedef for note events (containing start_time, duration, note etc)
//TODO: Set minimum and maximum values for some of the numbers.
//TODO: Instead of hard coding the keybindings, make a header file like dwm
//      for starters, and perhaps a runtime config file later
//TODO: Implement a tone buffer that contains all of the existing notes plus the 
//      one about to be placed, so that this can be used to decide the properties
//      of the new note (the buffer will be played when 'p' is pushed).

int insert_events(){
    char inst = 'A';
    int note = default_note;
    float start_time = 0.5;
    float dur = 0.5;
    char num;
    mvprintw(curY, curX + LN_OFFSET, "%c(%d, %1.3f, %1.3f)", inst, note, start_time, dur);
    refresh();
    while (1){
        num = getch();
        switch(num){
            /* Changing instrument */
            case 'z':
                inst++;
                break;
            case 'Z':
                inst--;
                break;
            /* Changing note, duration, position */
            case 'n':
                note--;
                break;
            case 'N':
                note -= 12;
                break;
            case 'e':
                note++;
                break;
            case 'E':
                note += 12;
                break;
            /* Change duration */
            case 'h':
                dur -= step;
                break;
            case 'i':
                dur += step;
                break;
            /* Change position */
            case 's':
                start_time -= step;
                break;
            case 't':
                start_time += step;
                break;
            /* Finishing insert */
            case ',':
                printw(", ");
                getyx(stdscr, curY, curX);
                // TODO: a function that appends the new note event to a .txt file, so it matches what's
                //       shown on the display.
                if(owwd_note) default_note = note;
                return INSERT;
            case 10:    // this is enter
                curX = 0;
                curY++;
                if(owwd_note) default_note = note;
                return INSERT;
            case 27:    // esc
                return EDIT;
            default:
                break;
        }
        mvprintw(curY, curX+LN_OFFSET, "%c(%d, %1.3f, %1.3f)", inst, note, start_time, dur);
         mvprintw(yMax - 5, 1, "- New entry-\n       note: %d\n start time: %1.3f\n   duration: %1.3f", note, start_time, dur);
        /* mvprintw(yMax - 5, 1, "- New entry-\n instrument: %s\n       note: %d\n start time: %1.3f\n   duration: %1.3f", 
                get_instrument(instlist, inst), note, start_time, dur);
        // This includes the name of the instrument     */ 
        refresh();
    }
}
/* Tonebook is the name of the notepad like text display */
void draw_tonepad(int yLen, int yFirstNum){
    // quick and dirty
    for(int i = 0; i < yLen; i++){
        if(i < 10)
            mvprintw(i, 0, "   %d", yFirstNum + i);
        else if(i < 100)
            mvprintw(i, 0, "  %d", yFirstNum + i);
        else if(i < 1000)
            mvprintw(i, 0, " %d", yFirstNum + i);
        else
            mvprintw(i, 0, "%d", yFirstNum + i);
    }
}

int edit_events(){
    char num;
    while (1){
        num = getch();
        switch(num){
            /* Changing instrument */
            case 'z':
                inst++;
                break;
            case 'Z':
                inst--;
                break;
            /* Changing note, duration, position */
            case 'n':
                note--;
                break;
            case 'N':
                note -= 12;
                break;
            case 'e':
                note++;
                break;
            case 'E':
                note += 12;
                break;
            /* Change duration */
            case 'h':
                dur -= step;
                break;
}

int main(){
    initscr();
    noecho();
    cbreak();                      

    // max positions
    getmaxyx(stdscr, yMax, xMax);
    draw_tonepad(yMax - 8, 0);

    // current position
    bool running = 1; 
    while(running){
        while(mode == EDIT)
            mode = edit_events();
        while(mode == INSERT)
            mode = insert_events();
        running = 0;
    }
    endwin();                  

    return 0;
}
