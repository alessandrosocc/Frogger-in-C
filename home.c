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
#include "home.h"
#include "river.h"



int main() {
    srand(time(NULL));
    int p[2],p2[2];
    pipe(p);
    
    int maxX=0, maxY=0;
    initScreen(&maxY,&maxX);
    windowGeneration();
    
    pid_t auto0 = fork();
    if (auto0 < 0){
        perror("errore nella generazione della macchina 1");
    }
    else if (auto0 == 0){
        car(p,0);
    }
    else{
        pid_t auto1=fork();
        if (auto1<0){
            perror("Errore generazione macchina 0");
        }
        else if(auto1==0){
            car(p,1);
        }
        else{
            pid_t auto2=fork();
            if (auto2<0){
                perror("Errore generazione macchina 0");
            }
            else if(auto2==0){
                car(p,2);
            }
            else{
                pid_t auto3=fork();
                if (auto3<0){
                    perror("Errore generazione macchina 0");
                }
                else if(auto3==0){
                    car(p,3);
                }
                else{
                    pid_t auto4=fork();
                    if (auto4<0){
                        perror("Errore generazione macchina 0");
                    }
                    else if(auto4==0){
                        car(p,4);
                    }
                    else{
                        pid_t auto5=fork();
                        if (auto5<0){
                            perror("Errore generazione macchina 0");
                        }
                        else if(auto5==0){
                            car(p,5);
                        }
                        else{
                            pid_t auto6=fork();
                            if (auto6<0){
                                perror("Errore generazione macchina 0");
                            }
                            else if(auto6==0){
                                car(p,6);
                            }
                            else{
                                pid_t auto7=fork();
                                if (auto7<0){
                                    perror("Errore generazione macchina 0");
                                }
                                else if(auto7==0){
                                    car(p,7);
                                }
                                else{
                                    pid_t auto8=fork();
                                    if (auto8<0){
                                        perror("Errore generazione macchina 0");
                                    }
                                    else if(auto8==0){
                                        car(p,8);
                                    }
                                    else{
                                        pid_t auto9=fork();
                                        if (auto9<0){
                                            perror("Errore generazione macchina 0");
                                        }
                                        else if(auto9==0){
                                            car(p,9);
                                        }
                                        else{
                                            pid_t frog = fork();
                                            if (frog == 0){
                                                ffrog(p);
                                            }
                                            else{
                                                pid_t proiettile=fork();
                                                if (proiettile==0){
                                                    bullet(p);
                                                }
                                                else{
                                                    pid_t log0 = fork();
                                                    if (log0 == 0){
                                                        legnetto(p, 0);
                                                    }
                                                    else{
                                                        pid_t log1 = fork();
                                                        if (log1 == 0){
                                                            legnetto(p,1);
                                                        }
                                                        else
                                                        {
                                                            pid_t log2 = fork();
                                                            if (log2 == 0){
                                                                legnetto(p, 2);
                                                            }
                                                            else{
                                                                pid_t log3=fork();
                                                                if (log3==0){
                                                                    legnetto(p, 3);

                                                                }
                                                                else{
                                                                    pid_t log4=fork();
                                                                    if (log4==0){
                                                                        legnetto(p, 4);
                                                                    }
                                                                    else{
                                                                        printAll(p);
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
    offsetFiume=offsetSum-1;
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

    //vite
    for (size_t i = offsetSum; i<= VITE+offsetSum; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, maxX-21, ' ', 20);
        attroff(COLOR_PAIR(3));
    }
    //refresh();
    offsetSum+=VITE;
}


void printAll(int p[]){
    close(p[1]);
    elemento d, rana,bull; //bull=bullet
    elemento woody[5];
    elemento bullets[5]; //bullets nemici sui tronchi
    //inizializziamo macchine
    elemento macchine[CORSIE*MACCHINE];
    for (size_t i = 0; i< CORSIE*MACCHINE; i++){
        macchine[i].x=-1;
        macchine[i].y=-1;
        macchine[i].c=-1;
    }

    //get valori rana, bullet se presente, macchine, tronchi e bullet nemici sui trochi
    while(true){
        erase();
        windowGeneration();
        read(p[0], &(d), sizeof(elemento));
        if (d.c==20){ //se è la rana
            rana.x=d.x;
            rana.y=d.y;
            rana.c=d.c;
            if (d.sparato==true){
                bull.sparato=true;
            }
        }
        else if(d.c==21){ //se è il proiettile
            bull.x = d.x;
            bull.y = d.y;
        }
        else{
            for (int i=0;i<CORSIE*MACCHINE;i++){
                if (d.c == i){ //assegna a macchina iesima
                    macchine[i].x = d.x;
                    macchine[i].y = d.y;
                    macchine[i].c = d.c; 
                    macchine[i].type = d.type;       
                }
            }
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

        //Stampa Rana
        attron(COLOR_PAIR(4));
        mvprintw(rana.y,rana.x,"\\/");
        mvprintw(rana.y+1,rana.x,"/\\");
        
        if (bull.sparato == true){
            mvprintw(bull.y, bull.x, "*");
        }
        attroff(COLOR_PAIR(4));
        
        //Stampa Fiume, tronchi, nemici e proiettili nemici
        for (int i=0;i<CORSIE;i++){
            if (d.c == i+30){ //assegna a legnetto iesimo
                woody[i].x = d.x;
                woody[i].y = d.y;
                woody[i].c = d.c; 
                woody[i].enemy=d.enemy;
            }
            if (d.c  == i+70){ // se è un proiettile => 30 (tronco ID) + 40 (proiettile ID)
                bullets[i].x = d.x;
                bullets[i].y = d.y;
                bullets[i].c = d.c;
                bullets[i].sparato = d.sparato;
                
            }
        }       
        // stampa tronchi e nemici
        for(size_t i = 0; i<CORSIE; i++)
        { 
            //DEBUG
            attron(COLOR_PAIR(3));
            //debug
            //mvprintw(i+20,1,"Posizione woody %d -> y: %d x: %d c: %d enemy: %d, sparato %d",i,woody[i].y,woody[i].x,woody[i].c,woody[i].enemy, woody[i].sparato);
            if(woody[i].enemy==false){
                mvprintw(woody[i].y,woody[i].x,"/----\\");
                mvprintw(woody[i].y+1,woody[i].x,"\\----/");
            }
            //C'è un nemico!
            else{
                mvprintw(woody[i].y,woody[i].x,"/-00-\\");
                mvprintw(woody[i].y+1,woody[i].x,"\\-||-/");
            }
        }
        //stampa proiettili quando sparati
        for (size_t i = 0; i< CORSIE; i++){
            if (bullets[i].sparato == true){
                mvaddch(bullets[i].y, bullets[i].x, '*');
                //debug
                mvprintw(i+40,1,"Posizione bull %d -> y: %d x: %d c: %d",i,bullets[i].y,bullets[i].x,bullets[i].c);
            }
        }
        attroff(COLOR_PAIR(3));
        
        
        refresh();
    
    }
}
