
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <stdlib.h>

//maxY dal basso = sempre maxY-3
//(0,0) == alto a sx
//(maxX, maxY) == basso a dx

//WINDOW *<nomeWindow> = newwin(int nlinee, int ncols, int inizio_y, int inizio_x)
void initScreen(int*, int*);
int main() {    
    int maxX=0, maxY=0, nCorsie=3, nFiume = 3;
    initScreen(&maxY,&maxX);
    WINDOW *vite, *tempo, *marciapiede, *autostrada, *prato, *fiume, *tane, *punteggio;
    // box vite
    box(stdscr,0,0);
    refresh();
    vite=newwin(2,20,maxY-3,1);
    box(vite,0,0);
    wrefresh(vite); 
    // box tempo
    tempo=newwin(2,20,maxY-3,maxX-21);
    box(tempo,0,0);
    wrefresh(tempo); 

    // box marciapiede
    marciapiede=newwin(2,maxX-2,maxY-5,1);
    box(marciapiede,0,0);
    wrefresh(marciapiede); 
    // box autostrada
    
    autostrada=newwin(6,maxX-2,maxY-11,1);
    box(autostrada,0,0);
    wrefresh(autostrada); 
    
    // box prato 
    prato=newwin(2,maxX-2,maxY-13,1);
    box(prato,0,0);
    wrefresh(prato); 
    
    // box fiume
    
    fiume=newwin(6,maxX-2,maxY-19,1);
    box(fiume,0,0);
    wrefresh(fiume); 
    
    // box tane
    
    tane=newwin(6,maxX-2,maxY-25,1);
    box(tane,0,0);
    wrefresh(tane); 
    
    // box punteggio
    punteggio=newwin(6,maxX-2,maxY-31,1);
    box(punteggio,0,0);
    wrefresh(punteggio); 
      

   
    sleep(5);
    endwin();
    return 0; 
}

void initScreen(int* maxY, int* maxX){
    initscr(); 
    curs_set(0); // settiamo il cursore per essere invisibile
    noecho();
    getmaxyx(stdscr,*maxY,*maxX);
    return;
}