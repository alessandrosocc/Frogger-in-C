#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <signal.h> //per kill
#include "HighWay.h"

#define CORRENTI 5
#define DIMCORRENTI 2


#define PUNTEGGIO 3
#define TANE 6
#define FIUME 10
#define PRATO 2
#define AUTOSTRADA 10
#define MARCIAPIEDE 2
#define TEMPO 3
#define VITE 3


//LE WINDOW NON SONO PASSATE PER VALORE!!!!!!!!!! NON PER RIFERIMENTO NON SONO PUNTATORI
//maxY dal basso = sempre maxY-3
//(0,0) == alto a sx
//(maxX, maxY) == basso a dx
//WINDOW *vite, *tempo, *marciapiede, *autostrada, *prato, *fiume, *tane, *punteggio;


void printHighWay(int p[]);
void windowGeneration();
void frog(int p[]);
void initScreen(int*, int*);
int offsetAutostrada = 0;


//WINDOW *<nomeWindow> = newwin(intsetlocale(LC_CTYPE, ""); nlinee, int ncols, int inizio_y, int inizio_x)

int main() {
    srand(time(NULL));
    int p[2];
    pipe(p);
    
    int maxX=0, maxY=0;
    initScreen(&maxY,&maxX);
    windowGeneration();

    // generazione dei processi

    pid_t auto1 = fork();
    if (auto1 < 0){
        perror("errore nella generazione della macchina 1");
    }
    else if (auto1 == 0){
        car(p);
        
    }
    else{
        printHighWay(p);   
    }    
    sleep(5);

    endwin();
    return 0; 
}

void initScreen(int* maxY, int* maxX){
    initscr(); 
    curs_set(0); // settiamo il cursore per essere invisibile
    noecho();
    getmaxyx(stdscr,*maxY,*maxX);
}

void windowGeneration(){
    int maxX=0, maxY=0,nCorsie=3, nFiume = 3, yBox = 0, xBox=0, incYBox = 0, corsie = 1, correnti = 1, righe = 1;
    getmaxyx(stdscr,maxY, maxX);
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_RED);
    init_pair(3,COLOR_WHITE,COLOR_YELLOW);
    init_pair(4,COLOR_WHITE,COLOR_CYAN);



    box(stdscr,0,0);

    int offsetSum=1;
    
    //punteggio
    for (size_t i = 1; i<= PUNTEGGIO; i++){
        mvhline(i, 1, COLOR_PAIR(3), maxX-2);
    }
    
    offsetSum+=3;
    
    //tane
    for (size_t i = offsetSum; i<= offsetSum+TANE; i++){
       mvhline(i, 1, COLOR_PAIR(3), maxX-2);
    }
    offsetSum+=TANE;

    //fiume
    for (size_t i = offsetSum; i<= FIUME+offsetSum; i++){
        mvhline(i, 1, COLOR_PAIR(3), maxX-2);
    }
    offsetSum+=FIUME;

    //prato
    for (size_t i = offsetSum; i<= offsetSum+PRATO; i++){
        mvhline(i, 1, COLOR_PAIR(1), maxX-2);
    }
    offsetSum+=PRATO;

    // autostrada
    for (size_t i = offsetSum; i<= offsetSum+AUTOSTRADA; i++){
        mvhline(i, 1, COLOR_PAIR(4), maxX-2);
    }
    offsetAutostrada=offsetSum-1; //non so perchè ma vuole quel -1 altrimenti non è ben formattato
    offsetSum+=AUTOSTRADA;
    

    //marciapiede
    for (size_t i = offsetSum; i<= MARCIAPIEDE+offsetSum; i++){
        mvhline(i, 1, COLOR_PAIR(2), maxX-2);
    }
    
    offsetSum+=MARCIAPIEDE;

    //tempo
    for (size_t i = offsetSum; i<= TEMPO+offsetSum; i++){
        mvhline(i, 1, COLOR_PAIR(3 ), maxX-2);
    }
    
    offsetSum+=TEMPO;

    //vite
    for (size_t i = offsetSum; i<= VITE+offsetSum; i++){
        mvhline(i, 1, COLOR_PAIR(3 ), maxX-2);
    }
    //refresh();
    offsetSum+=VITE;
}
// void windowGeneration(){ //WINDOW *vite, WINDOW *tempo, WINDOW *marciapiede, WINDOW* autostrada,WINDOW *prato,WINDOW *fiume,WINDOW *tane,WINDOW *punteggio
//     int maxX=0, maxY=0,nCorsie=3, nFiume = 3, yBox = 0, xBox=0, incYBox = 0, corsie = 1, correnti = 1, righe = 1;
//     getmaxyx(stdscr,maxY, maxX);
//     start_color();
//     init_pair(1,COLOR_WHITE,COLOR_GREEN);
//     init_pair(2,COLOR_WHITE,COLOR_RED);
//     // box vite
    
//     box(stdscr,0,0);
//     refresh();
//     // box punteggio    
//     punteggio=newwin(6,maxX-2,1,1);
//     box(punteggio,0,0);
//     wrefresh(punteggio); 
//     refresh();

//     // box tane
//     getmaxyx(punteggio, yBox, xBox);
//     incYBox += (yBox+1);
//     tane=newwin(6,maxX-2,incYBox,1);
//     box(tane,0,0);
//     wrefresh(tane);
    
//     // box fiume
//     righe = (CORRENTI * DIMCORRENTI);
//     getmaxyx(tane, yBox,xBox);
//     incYBox += (yBox );
//     fiume=newwin(righe,maxX-2,incYBox,1);
//     box(fiume,0,0);
//     wrefresh(fiume); 
    
//     // box prato 
//     getmaxyx(fiume, yBox, xBox);
//     incYBox += (yBox);
//     prato=newwin(2,maxX-2,incYBox,1);
//     box(prato,0,0);
//     wrefresh(prato);
//     wbkgd(prato,COLOR_PAIR(1));
//     wrefresh(prato); 

//     // box autostrada
//     righe = (CORSIE * DIMCORSIE)+1;
//     getmaxyx(prato, yBox,xBox);
//     incYBox += (yBox);
//     autostrada=newwin(righe,maxX-2,incYBox,1);
//     box(autostrada,0,0);
//     wrefresh(autostrada); 
    
//     // box marciapiede
//     init_pair(1,COLOR_WHITE,COLOR_GREEN);
//     getmaxyx(autostrada, yBox,xBox);
//     incYBox += (yBox);
//     marciapiede=newwin(2,maxX-2,incYBox,1);
//     box(marciapiede,0,0);
//     wrefresh(marciapiede);    
//     wbkgd(marciapiede,COLOR_PAIR(2));
//     wrefresh(marciapiede); 
    
//     // box tempo
//     getmaxyx(marciapiede, yBox,xBox);
//     incYBox += (yBox);
//     tempo=newwin(2,20,incYBox,maxX-21);
//     box(tempo,0,0);
//     wrefresh(tempo); 

//     // box vite
//     getmaxyx(tempo, yBox,xBox);
//     vite=newwin(2,20,incYBox,1);
//     box(vite,0,0);
//     wrefresh(vite);   
//     return ;
// }


void printHighWay(int p[]){
    close(p[1]);
    elemento data;
    elemento macchine[MACCHINE*CORSIE];
    while (true){
        erase();
        windowGeneration();
        read(p[0],&data, sizeof(elemento));
        if (data.c == '0'){
            macchine[0].x = data.x;
            macchine[0].y = data.y;
            macchine[0].c = data.c;
        }
        else if (data.c=='1'){
            macchine[1].x = data.x;
            macchine[1].y = data.y;
            macchine[1].c = data.c;
        }
        else if (data.c=='2'){
            macchine[2].x = data.x;
            macchine[2].y = data.y;
            macchine[2].c = data.c;
        }
        else if (data.c=='3'){
            macchine[3].x = data.x;
            macchine[3].y = data.y;
            macchine[3].c = data.c;
        }
        else if (data.c=='4'){
            macchine[4].x = data.x;
            macchine[4].y = data.y;
            macchine[4].c = data.c;
        }
        else if (data.c=='5'){
            macchine[5].x = data.x;
            macchine[5].y = data.y;
            macchine[5].c = data.c;
        }
        else if (data.c=='6'){
            macchine[6].x = data.x;
            macchine[6].y = data.y;
            macchine[6].c = data.c;
        }
        else if (data.c=='7'){
            macchine[7].x = data.x;
            macchine[7].y = data.y;
            macchine[7].c = data.c;
        }
        else if (data.c=='8'){
            macchine[8].x = data.x;
            macchine[8].y = data.y;
            macchine[8].c = data.c;
        }
        else if (data.c=='9'){
            macchine[9].x = data.x;
            macchine[9].y = data.y;
            macchine[9].c = data.c;
        }

        for (size_t i=0;i<CORSIE*MACCHINE;i++){
            //mvwaddch(autostrada, macchine[i].y, macchine[i].x, macchine[i].c);
            attron(COLOR_PAIR(4));
            mvprintw(macchine[i].y,macchine[i].x,"/--\\");
            mvprintw(macchine[i].y+1,macchine[i].x, "0--0");
            attroff(COLOR_PAIR(4));
        }
        usleep(DELAY);
        //box(autostrada,0,0);
        refresh();
    }
    
    return;
}