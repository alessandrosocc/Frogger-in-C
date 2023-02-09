#include <ncurses.h>
int main() {
    int kcode; 
    initscr();
    noecho();
    curs_set(0);
    /* Inizializza schermo di output */
    /* Imposta modalità della tastiera */
    /* Nasconde il cursore */
    mvprintw(2, 1,"Premi un tasto qualunque (q per uscire)");
    refresh();
    while(true){
        kcode = getch();
        deleteln(); /* Cancella la precedente riga */   
        mvprintw(2, 1,"Codice del tasto premuto (q per uscire): %d", kcode); refresh();
    if(kcode==113) 
        break; /* 113 = codice ASCII del tasto 'q' */ }
    endwin();
return 0;
}