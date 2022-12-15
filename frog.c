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



// int main(){
//     elementoFrog data;
//     int p[2];
//     pipe(p);
//     initscr();
//     noecho();
//     keypad(stdscr, 1);
//     curs_set(0);
//     pid_t frog=fork();
//     if (frog < 0){
//         _exit(3);
//     }
//     if(frog == 0){
//         ffrog(p);
//     }
//     else{
//         pid_t proiettile = fork();
//         if(proiettile == 0){
//             bullet(p);
//         }
//         else{
//             padre(p);
//         }
//     }
//     endwin();
// }


void ffrog(int p[]){
    elementoFrog rana;
    close(p[0]);
    int maxx=0,maxy=0;
    getmaxyx(stdscr,maxy,maxx);
    rana.c= 1;
    rana.y=maxy/2;
    rana.x=maxx/2; 
    rana.sparato=false;
    write(p[1],&rana, sizeof(elementoFrog));
    //getmaxyx(stdscr,oggetto.y,oggetto.x);
    while(true){
        int c = getch();
        switch(c) {
            case 32: //barra spaziatrice
                rana.sparato=true;
                break;
            case KEY_UP: 
                if(rana.y > 0)
                    rana.y -= 1; 
                    break;
            case KEY_DOWN:
                if(rana.y < maxy - 1)
                    rana.y += 1; 
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
        write(p[1],&rana, sizeof(elementoFrog));
        rana.sparato = false;
    }
    return;
}

// void printFrog(int p[]){
//     close(p[1]);
//     elementoFrog data, animale, bull;

//     while(true){
//         // erase();
//         // windowGeneration();
//         read(p[0], &data,sizeof(elementoFrog));
//         if (data.c == 1){
//             animale.x = data.x;
//             animale.y = data.y;
//             if (data.sparato==true){
//                 bull.sparato=true;
//             }
//         }
//         else if (data.c == 2){
//             bull.x = data.x;
//             bull.y = data.y;
//         }
//         // stampa
//         mvprintw(animale.y,animale.x,"\\/");
//         mvprintw(animale.y+1,animale.x,"/\\");
//         if (bull.sparato == true){
//             mvprintw(bull.y, bull.x, "*");
//         }
//         //box(stdscr, 0, 0);
//         // refresh();
        
//     }
//     usleep(1000);
//     return;
// }

void bullet(int p[]){
    elementoFrog proiettile,data;
    while(true){
        read(p[0], &data, sizeof(elementoFrog));
        if (data.sparato == true && data.c == 1){
            proiettile.c = 2;
            proiettile.y=data.y;
            proiettile.x=data.x;
            while(proiettile.y>0){
                    proiettile.y -= 1;
                    write(p[1], &proiettile, sizeof(elementoFrog));
                    usleep(10000);
            }
        }
        else{
            write(p[1],&data,sizeof(elementoFrog));
        }  
    }
      
}

// void f_proiettile(int p[2]){

//     getmaxyx(stdscr, maxy, maxx);
//     posizione oggetto = {-1, -1, '^','P'};
//     posizione letto;

//     while(true){
//         read(p[0], &letto, sizeof(posizione)); //lettura della rana
//         usleep(100000);
//         if (letto.ID == 'R' && letto.sparato == 1){ //controllo se si tratta della rana 
//              //prendo le coordinate della rana da cui deve partire il proittile
//                 oggetto.x = letto.x;
//                 oggetto.y = letto.y-1;

//                 while(oggetto.y > -1){ //movimento del proiettile 
//                     oggetto.y -= 1;
//                     write(p[1], &oggetto, sizeof(posizione));
//                     usleep(100000);
//             }
//         }
//         else{
//             write(p[1], &oggetto, sizeof(posizione)); //se non è la rana quella che legge fa questa write
//             usleep(100000);
//         }
//     }    
//     return;
// }