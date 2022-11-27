#include <ncurses.h>
int main() {    
    initscr();
    curs_set(0);
    noecho();
    WINDOW *w1;
    w1= newwin (20,30,20,20);
    box(stdscr,0,0);
    refresh();
    box(w1,ACS_VLINE,ACS_HLINE);
    refresh();
    mvwprintw(w1,5,3,"premi un tasto"); /* Scrive a partire dalla linea 5, colonna 3 */
    wgetch(w1);
    delwin(w1);
    refresh();
    endwin();
    return 0; 
}