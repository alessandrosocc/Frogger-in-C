#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
int main() {    
	  WINDOW *w1,*w2;
    initscr();
    curs_set(0);
    noecho();
    w1= newwin (10,20,1,2);
    box(w1,ACS_VLINE,ACS_HLINE);
    mvwprintw(w1,5,3,"premi un tasto"); /* Scrive a partire dalla linea 5, colonna 3 */

    w2= newwin (10,20,1,2);
    box(w2,ACS_VLINE,ACS_HLINE);
    mvwprintw(w2,30,30,"premi un tasthljo"); /* Scrive a partire dalla linea 5, colonna 3 */
    wrefresh(w2);

    sleep(5);


    refresh();
    endwin();
    return 0; 
}