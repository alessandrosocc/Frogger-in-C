
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
int main() {    
	WINDOW *w1;
    initscr();

    start_color();
    init_pair(1,COLOR_BLACK,COLOR_RED);
    
    w1= newwin (10,20,1,2);

    wbkgd(w1,COLOR_PAIR(1));
    wrefresh(w1);    
    sleep(5);
    refresh();
    endwin();
    return 0; 
}