#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <stdbool.h>
#include "HighWay.h"

elemento veicoli[CORSIE*MACCHINE];


void car(int descriptor[], int connection[], int id){
    srand(getpid());
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(stdscr,maxY,maxX); //only for getting maxX
    maxY=offsetAutostrada;
    elemento tmp[CORSIE*MACCHINE];
    

    // generazione corsie possibili per le macchine
    int possibleStartY[CORSIE]={0};
    int counter = 0;
    for(size_t i=0;i<CORSIE*MACCHINE;i++){
        if (i%2!=0 && i != 0){
            possibleStartY[counter]=i;
            counter++;
        }
    }
    
    // generazione posizioni delle macchine
    veicoli[id].y=possibleStartY[(rand()%CORSIE)]+offsetAutostrada;
    veicoli[id].x=(1+rand()%maxX);
    veicoli[id].c=id;

    // generazione casuale macchina e camion in base ad attributo type
    if (1+rand()%6 > 4){
        veicoli[id].type=0; //macchina
    }
    else{
        veicoli[id].type=1; //camion
    }
    // controllo delle collisioni durante la generazione delle macchine
    if (id!=0){ 
        for (size_t i = 0; i < CORSIE*MACCHINE; i++ ){ //prendi scorsi elementi
            read(descriptor[0], &(tmp[i]), sizeof(elemento));
        }

        for (size_t i = 0; i<CORSIE*MACCHINE; i++){ //scorriamo gli altri elementi per cercare collisioni
                if(veicoli[id].y==tmp[id].y && veicoli[id].x>=tmp[i].x-7 && veicoli[id].x<=tmp[i].x+7){
                    veicoli[id].y=possibleStartY[(rand()%CORSIE)]+offsetAutostrada;
                    veicoli[id].x=(1+rand()%maxX);
                    i = 0; 
                }
        }
        for (size_t i = 0; i < CORSIE*MACCHINE; i++){
            write(descriptor[1], &(tmp[i]), sizeof(elemento));
        }
    }
    close(descriptor[0]);
    
    // continua ad andare ad x+1
    while (true){
        if (ControlloCollisione(veicoli[id]))
        {
            veicoli[id].y=possibleStartY[rand()%CORSIE]+offsetAutostrada; // nuova riga
        
            if (veicoli[id].y == 3+offsetAutostrada || veicoli[id].y == 7+offsetAutostrada){ 
                veicoli[id].x =maxX - 1; // la macchina inizia da destra
            }
            else{
                veicoli[id].x+=1; // la macchina inizia da sinistra
            }
        }
        else{
            if (veicoli[id].y == 3+offsetAutostrada || veicoli[id].y == 7+offsetAutostrada){
                veicoli[id].x-=1;
            }
            else{
                veicoli[id].x+=1;
            }
                
        }      
        write(descriptor[1], &veicoli[id], sizeof(elemento));
        usleep(DELAY);
    }
}

bool ControlloCollisione(elemento oggetto){
    bool flag = false;
    int maxx=0, maxy=0;
    getmaxyx(stdscr, maxy, maxx);
    if (oggetto.x < 0 || oggetto.x >= maxx || oggetto.y < 0 || oggetto.y >= maxy){
        flag = true;
    }
    return flag;
}

