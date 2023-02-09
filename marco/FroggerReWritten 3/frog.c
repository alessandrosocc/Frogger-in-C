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


void ffrog(int p1[], int p4[], int p5[], int p7[]){

    //close(p[0]);
    elemento rana, tmp, oldfrog;
    tmp.cambioMovimento=false; // ALTRIMENTI SPAWNA IN ALTO A SINISTRA
    // fprintf(fp,"x %d y %d\n",rana.x,rana.y);
    // fflush(fp);
    int maxx=0,maxy=0, counter = 0, collisionDetection=0;
    bool ritornaInPosizione = false;
    getmaxyx(stdscr,maxy,maxx);
    rana.c = 20; //identificativo Rana
    rana.y=offsetMarciapiede;
    rana.x=maxx/2;
    rana.sparato=false;
    
    // fprintf(fp,"ffrog prima write -> rana.x %d rana.y %d\n",rana.x,rana.y);
    // fflush(fp);
    write(p1[1],&rana, sizeof(elemento));

    while(true){
        // fprintf(fp,"ffrog start while write -> rana.x %d rana.y %d\n",rana.x,rana.y);
        // fflush(fp);
        // prima di tutto leggo la pipe che mi fa comunicare con la funzione di controllo
        read(p4[0], &(collisionDetection), sizeof(int));
        read(p5[0], &tmp, sizeof(elemento));
        // fprintf(fp,"ffrog dopo 2 read write -> rana.x %d rana.y %d\n",rana.x,rana.y);
        // fflush(fp);
        // read(p5[0], &(relPos), sizeof(int));
        // read(p5[0], &(idxLog), sizeof(int));
        // read(p7[0], &tmp, sizeof(elemento)); 
        //se l'intero letto è uguale a 1 allora si è verificata una collisione, quindi riporto la rana alla posizione di partenza
        if (collisionDetection == 1){
            collisionDetection=0;
            ritornaInPosizione = true;
            // fprintf(fp,"ffrog collisionDetection -> rana.x %d rana.y %d\n",rana.x,rana.y);
            // fflush(fp);
            write(p1[1],&rana, sizeof(elemento)); // scrivo direttamente, in modo da non dover aspettare il getch
        }
        else if(tmp.cambioMovimento){
            rana.x = tmp.x;
            rana.y = tmp.y;
            tmp.cambioMovimento = false;
        }
        if (ritornaInPosizione){
            rana.y=offsetMarciapiede;
            rana.x = maxx/2;
            ritornaInPosizione = false;
        }
        // fprintf(fp,"ffrog dopo ifs write -> rana.x %d rana.y %d\n",rana.x,rana.y);
        // fflush(fp);
        //se non si verifica alcuna collisione allora la rana si muoverà come sempre
        timeout(1);
        int c = getch();
        switch(c) {
            case 32: //barra spaziatrice
                rana.sparato=true;
                bulletGeneration(p1,p7, rana);
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
                if(rana.x > 0){
                rana.x -= 1;
                }
                break;
            case KEY_RIGHT:
                if(rana.x < maxx - 1){
                rana.x += 1;
                }
                break;
        }
        
        // fprintf(fp,"ffrog write in while -> rana.x %d rana.y %d\n",rana.x,rana.y);
        // fflush(fp);
        write(p1[1], &rana, sizeof(elemento));
        oldfrog = rana;
        rana.sparato = false;

        // la rana non può andare sotto il marciapiede
        rana.y>offsetMarciapiede?rana.y=offsetMarciapiede:1;
        // nel caso in cui la rana subisca una collisione
        ////mvprintw(1,1    , "coll detection %d",collisionDetection);
    }
    return;
}



void bulletGeneration(int p1[],int p7[], elemento rana){
    pid_t processProiettile = fork();
    if (processProiettile == 0){
        // passo la rana alla funzione spostamento del proiettile
        bulletMovement(p1,p7, rana);
    }
    wait(NULL);
}
void bulletMovement(int p1[],int p7[], elemento rana){
    bool flag = true;
    elemento proiettile;
    proiettile.y = rana.y;
    proiettile.x = rana.x;
    proiettile.c = 21;
    int comunication = 0;
    while(proiettile.y>=0){
        proiettile.y -= 1;
        write(p1[1], &proiettile, sizeof(elemento));
        usleep(20000);
    }
}