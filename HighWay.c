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


void car2(int descriptor[], int id){
    srand(getpid());
    close(descriptor[0]);
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(stdscr,maxY,maxX); //only for getting maxX
    maxY=offsetAutostrada;
    elemento veicoli;

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

    veicoli.y=possibleStartY[(rand()%CORSIE)]+offsetAutostrada;
    veicoli.x=(1+rand()%maxX);
    veicoli.c=id;

    //write(descriptor[1], &(veicoli), sizeof(elemento));
    //printf("start: \n");
    //printf("y: %d - x: %d - car %d\n",veicoli.y,veicoli.x,veicoli.c);

            

    
    //check se una macchina è sopra un'altra, nel caso genera altra posizione
    // if (id > 0){ // check dalla seconda macchina generata
    //     size_t j = id-1;
    //     while(j<CORSIE*MACCHINE){ //genera altre pos se collidono finchè pos è diversa dalle pos di altre macchine
    //         if (veicoli.x == veicoli[j].x && veicoli.y == veicoli[j].y)
    //         {
    //             j = id-1;
    //             veicoli.x=(1+rand()%maxX);
    //         }
    //         j--;
    //     }
    // }

    //printf("Inside WHILE\n\n");
    // // continua ad andare ad x+1
    while (true){
        if (ControlloCollisione(veicoli))
        {
            veicoli.y=possibleStartY[rand()%CORSIE]+offsetAutostrada; // nuova riga
        
            if (veicoli.y == 3+offsetAutostrada || veicoli.y == 7+offsetAutostrada){ 
                veicoli.x =maxX - 1; // la macchina inizia da destra
            }
            else{
                veicoli.x+=1; // la macchina inizia da sinistra
            }
        }
        else{
            if (veicoli.y == 3+offsetAutostrada || veicoli.y == 7+offsetAutostrada){
                veicoli.x-=1;
            }
            else{
                veicoli.x+=1;
            }
                
        }
        //printf("y: %d - x: %d - car %d\n",veicoli.y,veicoli.x,veicoli.c);
        //printf("\n\n");
        write(descriptor[1], &veicoli, sizeof(elemento));
        //usleep(10000000); //introduce latenza di 1 secondo
        usleep(DELAY);
    }
}


bool ControlloCollisione(elemento oggetto){
    bool flag = false;
    int maxx=100, maxy=100;
    getmaxyx(stdscr, maxy, maxx);
    if (oggetto.x < 0 || oggetto.x >= maxx || oggetto.y < 0 || oggetto.y >= maxy){
        flag = true;
    }
    return flag;
}

