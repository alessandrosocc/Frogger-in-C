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
#include <fcntl.h>
#include "frog.h"
#include "HighWay.h"

void ffrog(int p[], int connection[]){
    //close(p[0]);
    elemento rana;
    int maxx=0,maxy=0, counter = 0, collisionDetection=0,r=0;
    getmaxyx(stdscr,maxy,maxx);
    rana.c = 20; //identificativo Rana
    rana.y=offsetMarciapiede;
    rana.x=maxx/2;
    rana.sparato=false;
    write(p[1],&rana, sizeof(elemento));


    while(true){
        // prima di tutto leggo la pipe che mi fa comunicare con la funzione di controllo
        read(connection[0], &(collisionDetection), sizeof(collisionDetection));
        // se l'intero letto è uguale a 1 allora si è verificata una collisione, quindi riporto la rana alla posizione di partenza
        if (collisionDetection == 1){
            rana.y=offsetMarciapiede;
            rana.x = maxx/2;
            write(p[1],&rana, sizeof(elemento)); // scrivo direttamente, in modo da non dover aspettare il getch
        }
        // se non si verifica alcuna collisione allora la rana si muoverà come sempre
        else{
            timeout(1);
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
            write(p[1], &rana, sizeof(elemento));
            rana.sparato = false;
        }
        // la rana non può andare sotto il marciapiede
        rana.y>offsetMarciapiede?rana.y=offsetMarciapiede:1;
        // nel caso in cui la rana subisca una collisione
        ////mvprintw(1,1    , "coll detection %d",collisionDetection);
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