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


void car(int descriptor[]){
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(autostrada,maxY,maxX);
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
    for (size_t i = 0; i < CORSIE*MACCHINE; i++){
        macchina[i].y= possibleStartY[(rand()%CORSIE)];
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
        macchina[i].c = i + '0';
    }
    while (true){
        for (size_t i=0;i<CORSIE*MACCHINE;i++){
            if (ControlloCollisione(macchina[i], autostrada)){
                macchina[i].y=possibleStartY[rand()%5];
                if (macchina[i].y == 3 || macchina[i].y == 7){
                    macchina[i].x =maxX - 1;
                }
                else{
                    macchina[i].x=1;
                }
                
            }
            else{
                if (macchina[i].y == 3 || macchina[i].y == 7){
                    macchina[i].x-=1;
                }
                else{
                    macchina[i].x+=1;
                }
                 
            }
            write(descriptor[1], &(macchina[i]), sizeof(elemento));
            usleep(DELAY); //introduce latenza di 1 secondo
        }
        
	}

    
}

bool ControlloCollisione(elemento oggetto, WINDOW* finestra){
    bool flag = false;
    int maxx, maxy;
    getmaxyx(finestra, maxy, maxx);
    if (oggetto.x < 0 || oggetto.x >= maxx || oggetto.y < 0 || oggetto.y >= maxy){
        flag = true;
    }
    return flag;
}

