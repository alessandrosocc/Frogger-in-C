#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h> //per kill
#include "HighWay.h"
#include "frog.h"

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
void initScreen(int*, int*);


// global variables
int offsetAutostrada = 0;
int idMacchine = 0;

int main() {
    srand(time(NULL));
    int p[2],p2[2];
    pipe(p);
    
    int maxX=0, maxY=0;
    initScreen(&maxY,&maxX);
    windowGeneration();

    // generazione dei processi

    // pid_t rana = fork();
    // if (rana < 0){
    //     perror("Errore nella generazione della rana");
    // }
    // else if (rana == 0){
    //     ffrog(p);
    // }
    // else{
    //     pid_t proiettile = fork();
    //     if(proiettile == 0){
    //         bullet(p);
    //     }
        
    pid_t auto0 = fork();
    if (auto0 < 0){
        perror("errore nella generazione della macchina 1");
    }
    else if (auto0 == 0){
        car2(p,0);
    }
    else{
        pid_t auto1=fork();
        if (auto1<0){
            perror("Errore generazione macchina 0");
        }
        else if(auto1==0){
            car2(p,1);
        }
        else{
            pid_t auto2=fork();
            if (auto2<0){
                perror("Errore generazione macchina 0");
            }
            else if(auto2==0){
                car2(p,2);
            }
            else{
                pid_t auto3=fork();
                if (auto3<0){
                    perror("Errore generazione macchina 0");
                }
                else if(auto3==0){
                    car2(p,3);
                }
                else{
                    pid_t auto4=fork();
                    if (auto4<0){
                        perror("Errore generazione macchina 0");
                    }
                    else if(auto4==0){
                        car2(p,4);
                    }
                    else{
                        pid_t auto5=fork();
                        if (auto5<0){
                            perror("Errore generazione macchina 0");
                        }
                        else if(auto5==0){
                            car2(p,5);
                        }
                        else{
                            pid_t auto6=fork();
                            if (auto6<0){
                                perror("Errore generazione macchina 0");
                            }
                            else if(auto6==0){
                                car2(p,6);
                            }
                            else{
                                pid_t auto7=fork();
                                if (auto7<0){
                                    perror("Errore generazione macchina 0");
                                }
                                else if(auto7==0){
                                    car2(p,7);
                                }
                                else{
                                    pid_t auto8=fork();
                                    if (auto8<0){
                                        perror("Errore generazione macchina 0");
                                    }
                                    else if(auto8==0){
                                        car2(p,8);
                                    }
                                    else{
                                        pid_t auto9=fork();
                                        if (auto9<0){
                                            perror("Errore generazione macchina 0");
                                        }
                                        else if(auto9==0){
                                            car2(p,9);
                                        }
                                        else{
                                            pid_t frog = fork();
                                            if (frog == 0){
                                                ffrog(p);
                                            }
                                            else{
                                                printAll(p);
                                            }
                                            //padre(p); 
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }   
        }
    }
    sleep(10);
    endwin();
    return 0;
}

void initScreen(int* maxY, int* maxX){
    initscr(); 
    curs_set(0); // settiamo il cursore per essere invisibile
    noecho();
    keypad(stdscr,1);
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
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(3));
    }
    
    offsetSum+=3;
    
    //tane
    for (size_t i = offsetSum; i<= offsetSum+TANE; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(3));
    }
    offsetSum+=TANE;

    //fiume
    for (size_t i = offsetSum; i<= FIUME+offsetSum; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(3));
    }
    offsetSum+=FIUME;

    //prato
    for (size_t i = offsetSum; i<= offsetSum+PRATO; i++){
        attron(COLOR_PAIR(1));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(1));
    }
    offsetSum+=PRATO;

    // autostrada
    for (size_t i = offsetSum; i<= offsetSum+AUTOSTRADA; i++){
        attron(COLOR_PAIR(4));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(4));
    }
    offsetAutostrada=offsetSum-1; //non so perchè ma vuole quel -1 altrimenti non è ben formattato
    offsetSum+=AUTOSTRADA;
    

    //marciapiede
    for (size_t i = offsetSum; i<= MARCIAPIEDE+offsetSum; i++){
        attron(COLOR_PAIR(2));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(2));
    }
    offsetSum+=MARCIAPIEDE+1;

    //tempo
    for (size_t i = offsetSum; i<= TEMPO+offsetSum; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', 20);
        attroff(COLOR_PAIR(3));
    }
    //offsetSum+=TEMPO;

    //vite
    for (size_t i = offsetSum; i<= VITE+offsetSum; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, maxX-21, ' ', 20);
        attroff(COLOR_PAIR(3));
    }
    //refresh();
    offsetSum+=VITE;
}


void printHighWay(int p[]){
    close(p[1]);
    elemento d,d2, rana;
    elemento macchine[CORSIE*MACCHINE];
    //inizializziamo macchine
    elemento macchine[CORSIE*MACCHINE];
    for (size_t i = 0; i< CORSIE*MACCHINE; i++){
        macchine[i].x=-1;
        macchine[i].y=-1;
        macchine[i].c=-1;
    }
    while(true){
        erase();
        windowGeneration();
        read(p[0], &(d), sizeof(elemento));
        for (int i=0;i<CORSIE*MACCHINE;i++){
            if (d.c == i){ //assegna a macchina iesima
            macchine[i].x = d.x;
            macchine[i].y = d.y;
            macchine[i].c = d.c; 
            macchine[i].type = d.type;       
        }
        



        
        // stampa macchine
        for(size_t i = 0; i<CORSIE*MACCHINE; i++){
            if (macchine[i].c!=-1){
                attron(COLOR_PAIR(4));
                //stampa macchina
                if(macchine[i].type==1){ //camion
                    mvprintw(macchine[i].y,macchine[i].x,"/-----\\");
                    mvprintw(macchine[i].y+1,macchine[i].x,"O-----O");
                }
                else{ //macchina
                    mvprintw(macchine[i].y,macchine[i].x,"/--\\");
                    mvprintw(macchine[i].y+1,macchine[i].x,"O--O");
                }
                // mvaddch(macchine[i].y,macchine[i].x, '0'+macchine[i].c);
                attroff(COLOR_PAIR(4));
            }
        }

        
        
        //printf("x: %d y: %d, id: %d\n", d.x,d.y, d.c);
        
        refresh();
        usleep(DELAY);
    }
}


void printFFrog(int p2[]){
    close(p2[1]);
    elementoFrog data, animale, bull;
    read(p2[0], &data,sizeof(elementoFrog));
    if (data.c == 1){
        animale.x = data.x;
        animale.y = data.y;
        if (data.sparato==true){
            bull.sparato=true;
        }
    }
    else if (data.c == 2){
        bull.x = data.x;
        bull.y = data.y;
    }
    // stampa
    mvprintw(animale.y,animale.x,"\\/");
    mvprintw(animale.y+1,animale.x,"/\\");
    if (bull.sparato == true){
        mvprintw(bull.y, bull.x, "*");
    }
}
