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
#include <stdbool.h>
#include "frog.h"
#include "HighWay.h"



void ffrog(int p[]){
    close(p[0]);
    elemento rana;
    int maxx=0,maxy=0, counter = 0;
    getmaxyx(stdscr,maxy,maxx);
    rana.c = 20; //identificativo Rana
    rana.y=maxy/2+5;
    rana.x=maxx/2; 
    rana.sparato=false;
    write(p[1],&rana, sizeof(elemento));
    while(true){
        int c = getch();      
        switch(c) {
            case 32: //barra spaziatrice
                rana.sparato=true;
                break;
            case KEY_UP: 
                if(rana.y > 0)
                    rana.y -= 2; 
                    break;
            case KEY_DOWN:
                if(rana.y < maxy - 1)
                    rana.y += 2; 
                    break;
            case KEY_LEFT: 
                if(rana.x > 0)
                    rana.x -= 1; 
                break;
            case KEY_RIGHT:
                if(rana.x < maxx - 1)
                    rana.x += 1;
                break;
        }
        write(p[1],&rana, sizeof(elemento));
        rana.sparato = false;
    }
    return;
}



void bullet(int p[]){
    elemento proiettile,data;
    while(true){
        read(p[0], &data, sizeof(elemento));
        if (data.sparato == true && data.c == 20){
            proiettile.c = 21;
            proiettile.y=data.y;
            proiettile.x=data.x;
            while(proiettile.y>0){
                    proiettile.y -= 1;
                    write(p[1], &proiettile, sizeof(elemento));
                    usleep(10000);
            }
        }
        else{
            write(p[1],&data,sizeof(elemento));
        }  
    }
      
}

