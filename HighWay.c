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


void car1(int descriptor[], WINDOW* autostrada){
    srand(time(NULL));
    close(descriptor[0]);
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(autostrada,maxY,maxX);
    elemento macchina;
    macchina.x = (1+rand()%maxX);
    macchina.y = (1+rand()%maxY);
    macchina.c = '1';
    while (true){
        if (ControlloCollisione(macchina, autostrada)){
            macchina.y=(1+rand()%maxY);
            macchina.x=1;
        }
        else{
            macchina.x+=1;
        }
        write(descriptor[1], &macchina, sizeof(elemento));
        usleep(30000); //introduce latenza di 1 secondo
	}
}

void car2(int descriptor[], WINDOW* autostrada){
    srand(time(NULL));
    close(descriptor[0]);
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(autostrada,maxY,maxX);
    elemento macchina;
    macchina.x = (1+rand()%maxX);
    macchina.y = (1+rand()%maxY);
    macchina.c = '2';
    while (true){
        if (ControlloCollisione(macchina, autostrada)){
            macchina.y=(1+rand()%maxY);
            macchina.x=1;
        }
        else{
            macchina.x+=1;
        }
        write(descriptor[1], &macchina, sizeof(elemento));
        usleep(30000); //introduce latenza di 1 secondo
	}
}
void car3(int descriptor[], WINDOW* autostrada){
    srand(time(NULL));
    close(descriptor[0]);
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(autostrada,maxY,maxX);
    elemento macchina;
    macchina.x = (1+rand()%maxX);
    macchina.y = (1+rand()%maxY);
    macchina.c = '3';
    while (true){
        if (ControlloCollisione(macchina, autostrada)){
            macchina.y=(1+rand()%maxY);
            macchina.x=1;
        }
        else{
            macchina.x+=1;
        }
        write(descriptor[1], &macchina, sizeof(elemento));
        usleep(30000); //introduce latenza di 1 secondo
	}
}
void car4(int descriptor[], WINDOW* autostrada){
    srand(time(NULL));
    close(descriptor[0]);
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(autostrada,maxY,maxX);
    elemento macchina;
    macchina.x = (1+rand()%maxX);
    macchina.y = (1+rand()%maxY);
    macchina.c = '4';
    while (true){
        if (ControlloCollisione(macchina, autostrada)){
            macchina.y=(1+rand()%maxY);
            macchina.x=1;
        }
        else{
            macchina.x+=1;
        }
        write(descriptor[1], &macchina, sizeof(elemento));
        usleep(30000); //introduce latenza di 1 secondo
	}
}
void car5(int descriptor[], WINDOW* autostrada){
    srand(time(NULL));
    close(descriptor[0]);
    int maxX = 0, maxY = 0, x = 0, y = 0;
    getmaxyx(autostrada,maxY,maxX);
    elemento macchina;
    macchina.x = (1+rand()%maxX);
    macchina.y = (1+rand()%maxY);
    macchina.c = '5';
    while (true){
        if (ControlloCollisione(macchina, autostrada)){
            macchina.y=(1+rand()%maxY);
            macchina.x=1;
        }
        else{
            macchina.x+=1;
        }
        write(descriptor[1], &macchina, sizeof(elemento));
        usleep(30000); //introduce latenza di 1 secondo
	}
}




/*
void fnemico2(int p[]){
    posizione oggetto;
    oggetto.x=(rand()%MAXX)+1;
    oggetto.y=1;
    oggetto.c='x';

    while (true){
        if (oggetto.y>=MAXY){
            oggetto.x=(rand()%MAXX)+1;
            oggetto.y=1;
        }
        else{
            oggetto.y+=1;
        }
        write(p[1], &oggetto, sizeof(posizione));
        usleep(30000); //introduce latenza di 1 secondo
	}
    return;
}

*/


bool ControlloCollisione(elemento oggetto, WINDOW* finestra){
    bool flag = false;
    int maxx, maxy;
    getmaxyx(finestra, maxy, maxx);
    if (oggetto.x < 0 || oggetto.x >= maxx |oggetto.y < 0 || oggetto.y >= maxy){
        flag = true;
    }
    return flag;
}



