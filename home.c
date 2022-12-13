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



//WINDOW *<nomeWindow> = newwin(intsetlocale(LC_CTYPE, ""); nlinee, int ncols, int inizio_y, int inizio_x)

int main() {
    srand(time(NULL));
    int p[2];
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
                                            printHighWay(p);
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

    sleep(5);
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

    elemento d, macchina0, macchina1,macchina2,macchina3,macchina4,macchina5,macchina6, macchina7, macchina8, macchina9;
    while(true){
        erase();
        windowGeneration();
        read(p[0], &(d), sizeof(elemento));
        if (d.c == 0){
            macchina0.x = d.x;
            macchina0.y = d.y;
            macchina0.c = d.c;
        }
        else if(d.c == 1){
            macchina1.x = d.x;
            macchina1.y = d.y;
            macchina1.c = d.c;
        }
        else if(d.c == 2){
            macchina2.x = d.x;
            macchina2.y = d.y;
            macchina2.c = d.c;
        }
        else if(d.c == 3){
            macchina3.x = d.x;
            macchina3.y = d.y;
            macchina3.c = d.c;
        }
        else if(d.c == 4){
            macchina4.x = d.x;
            macchina4.y = d.y;
            macchina4.c = d.c;
        }
        else if(d.c == 5){
            macchina5.x = d.x;
            macchina5.y = d.y;
            macchina5.c = d.c;
        }
        else if(d.c == 6){
            macchina6.x = d.x;
            macchina6.y = d.y;
            macchina6.c = d.c;
        }
        else if(d.c == 7){
            macchina7.x = d.x;
            macchina7.y = d.y;
            macchina7.c = d.c;
        }
        else if(d.c == 8){
            macchina8.x = d.x;
            macchina8.y = d.y;
            macchina8.c = d.c;
        }
        else if(d.c == 9){
            macchina9.x = d.x;
            macchina9.y = d.y;
            macchina9.c = d.c;
        }

        mvprintw(macchina0.y, macchina0.x, "0");
        mvprintw(macchina1.y, macchina1.x, "1");
        mvprintw(macchina2.y, macchina2.x, "2");
        mvprintw(macchina3.y, macchina3.x, "3");
        mvprintw(macchina4.y, macchina4.x, "4");
        mvprintw(macchina5.y, macchina5.x, "5");
        mvprintw(macchina6.y, macchina6.x, "6");
        mvprintw(macchina7.y, macchina7.x, "7");
        mvprintw(macchina8.y, macchina8.x, "8");
        mvprintw(macchina9.y, macchina9.x, "9");


        
        
        //printf("x: %d y: %d, id: %d\n", d.x,d.y, d.c);
        
        refresh();
        usleep(DELAY);
    }
}

