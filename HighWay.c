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

void car(int descriptor[]){
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(stdscr,maxY,maxX);
    maxY=offsetAutostrada;
    elemento macchina[CORSIE*MACCHINE];
    
    // generazione corsie possibili per le macchine
    int possibleStartY[CORSIE]={0}, counter = 0;
    for(size_t i=0;i<CORSIE*MACCHINE;i++){
        if (i%2!=0 && i != 0){
            possibleStartY[counter]=i;
            counter++;
        }
    }
    
    // generazione posizioni delle macchine
    for (int i = 0; i < CORSIE*MACCHINE; i++){
        macchina[i].y= possibleStartY[(rand()%CORSIE)]+offsetAutostrada;
        macchina[i].x=(1+rand()%maxX);
        if (i > 0){ // check dalla seconda macchina generata
            size_t j=0;
            while(j<CORSIE*MACCHINE){ //genera altre pos se collidono finchè pos è diversa dalle pos di altre macchine
                if (macchina[i].x == macchina[j].x && i != j)
                {
                    j = 0;
                    macchina[i].x=(1+rand()%maxX);
                }
                j++;
            }

        }
        macchina[i].c = i+1;
    }
    while (true){
        for (size_t i=0;i<CORSIE*MACCHINE;i++){
            if (ControlloCollisione(macchina[i])){
                macchina[i].y=possibleStartY[rand()%5]+offsetAutostrada;
                if (macchina[i].y == 3+offsetAutostrada || macchina[i].y == 7+offsetAutostrada){
                    macchina[i].x =maxX - 1;
                }
                else{
                    macchina[i].x=1;
                }
            }
            else{
                if (macchina[i].y == 3+offsetAutostrada || macchina[i].y == 7+offsetAutostrada){
                    macchina[i].x-=1;
                }
                else{
                    macchina[i].x+=1;
                }
                 
            }
            usleep(DELAY); //introduce latenza di 1 secondo
        }
        
	}
}



void car2(int descriptor[], int id){
    close(descriptor[0]);
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(stdscr,maxY,maxX);
    maxY=offsetAutostrada;

    // generazione corsie possibili per le macchine
    int possibleStartY[CORSIE]={0}, counter = 0;
    for(size_t i=0;i<CORSIE*MACCHINE;i++){
        if (i%2!=0 && i != 0){
            possibleStartY[counter]=i;
            counter++;
        }
    }

    // generazione posizioni delle macchine
    veicoli[id].y= possibleStartY[(rand()%CORSIE)]+offsetAutostrada;
    veicoli[id].x=(1+rand()%maxX);
    veicoli[id].c = id;
    write(descriptor[1], &(veicoli[id]), sizeof(elemento));

    //check se una macchina è sopra un'altra, nel caso genera altra posizione
    if (id > 0){ // check dalla seconda macchina generata
        size_t j = id-1;
        while(j<CORSIE*MACCHINE){ //genera altre pos se collidono finchè pos è diversa dalle pos di altre macchine
            if (veicoli[id].x == veicoli[j].x && veicoli[id].y == veicoli[j].y)
            {
                j = id-1;
                veicoli[id].x=(1+rand()%maxX);
            }
            j--;
        }
    }

    
    // // continua ad andare ad x+1
    while (true){
        if (ControlloCollisione(veicoli[id])){
            veicoli[id].y=possibleStartY[rand()%5]+offsetAutostrada;
            if (veicoli[id].y == 3+offsetAutostrada || veicoli[id].y == 7+offsetAutostrada){
                veicoli[id].x =maxX - 1;
            }
            else{
                veicoli[id].x=1;
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
        write(descriptor[1], &(veicoli[id]), sizeof(elemento));
        usleep(DELAY); //introduce latenza di 1 secondo
	}
}


bool ControlloCollisione(elemento oggetto){
    bool flag = false;
    int maxx, maxy;
    getmaxyx(stdscr, maxy, maxx);
    if (oggetto.x < 0 || oggetto.x >= maxx || oggetto.y < 0 || oggetto.y >= maxy){
        flag = true;
    }
    return flag;
}

