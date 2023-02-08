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


void ffrog(int p1[], int p4[], int p5[]){

    //close(p[0]);
    elemento rana;
    int maxx=0,maxy=0, counter = 0, collisionDetection=0;
    getmaxyx(stdscr,maxy,maxx);
    rana.c = 20; //identificativo Rana
    rana.y=offsetMarciapiede;
    rana.x=maxx/2;
    rana.sparato=false;
    write(p1[1],&rana, sizeof(elemento));
    

    while(true){
        // prima di tutto leggo la pipe che mi fa comunicare con la funzione di controllo
        read(p4[0], &(collisionDetection), sizeof(int));
        // se l'intero letto è uguale a 1 allora si è verificata una collisione, quindi riporto la rana alla posizione di partenza
        if (collisionDetection == 1){
            collisionDetection=0;
            fflush(fp);
            rana.y=offsetMarciapiede;
            rana.x = maxx/2;
            write(p1[1],&rana, sizeof(elemento)); // scrivo direttamente, in modo da non dover aspettare il getch
        }
        // se non si verifica alcuna collisione allora la rana si muoverà come sempre
        else{
            timeout(1);
            int c = getch();
            switch(c) {
                case 32: //barra spaziatrice
                    rana.sparato=true;
                    bulletGeneration(p1, rana);
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
            write(p1[1], &rana, sizeof(elemento));
            rana.sparato = false;
        }
        // la rana non può andare sotto il marciapiede
        rana.y>offsetMarciapiede?rana.y=offsetMarciapiede:1;
        // nel caso in cui la rana subisca una collisione
        ////mvprintw(1,1    , "coll detection %d",collisionDetection);
    }
    return;
}



void bulletGeneration(int p1[], elemento rana){
    pid_t processProiettile = fork();
    if (processProiettile == 0){
        // passo la rana alla funzione spostamento del proiettile
        bulletMovement(p1, rana);
    }
    wait(NULL);
}
void bulletMovement(int p1[], elemento rana){
    bool flag = true;
    elemento proiettile;
    proiettile.y = rana.y;
    proiettile.x = rana.x;
    proiettile.c = 21;
    while(flag){
        if (proiettile.y < 0){
            flag = false;
        } 
        else{
            proiettile.y -= 1;
        }
        write(p1[1], &proiettile, sizeof(elemento));
        usleep(10000);
    }
}