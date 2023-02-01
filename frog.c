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

void ffrog(int p[], int p3[], int p5[]){
    elemento rana, tmp;
    int maxx=0,maxy=0, counter = 0, collisionDetection=0, direzione = 1, x = 0;
    getmaxyx(stdscr,maxy,maxx);
    rana.c = 20; //identificativo Rana
    rana.y=offsetMarciapiede;
    rana.x=maxx/2;
    rana.sparato=false;
    rana.isOnLog = false;
    rana.idxLogOccupied=-1;
    rana.offsetLogOccupied=-1;
    int relPos=0;
    int idxLog=0;
    write(p[1],&rana, sizeof(elemento));
    while(true){
        // prima di tutto leggo la pipe che mi fa comunicare con la funzione di controllo
        read(p3[0], &(relPos), sizeof(int));
        read(p3[0], &(idxLog), sizeof(int));

        read(p5[0], &tmp, sizeof(elemento)); 
        //fprintf(fp,"x %d y %d c %d onfrog %d\n",tmp.x,tmp.y,tmp.c,tmp.isOnLog);
        // se l'intero letto è uguale a 1 allora si è verificata una collisione, quindi riporto la rana alla posizione di partenza
        //rana.collision = collisionDetection;
        // if (rana.collision == 1){
        //     //collisionDetection=0;
        //     //fflush(fp);
        //     rana.y=offsetMarciapiede;
        //     rana.x = maxx/2;
        //     write(p[1], &rana, sizeof(elemento)); // scrivo direttamente, in modo da non dover aspettare il getch
        // }

        //mvprintw(1,1,"ranax %d ranay %d tmp.x %d tmp.y %d",rana.x,rana.y,tmp.x,tmp.y);
        if (tmp.logOccupied){
            tmp.logOccupied = false;
            rana.isOnLog = true;
            rana.x = tmp.x+relPos;
            rana.idxLogOccupied=idxLog;
            // fprintf(fp,"rana.offsetLogOccupied %d | rana.idxLogOccupied",rana.offsetLogOccupied,rana.idxLogOccupied);
            // fflush(fp);
        }
        else{
            rana.isOnLog=false;
        }
        
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
                {
                    
                    rana.isOnLog?relPos-=1:1; // la rana si sposta sul tronco
                    rana.x -= 1;
                }
                break;
            case KEY_RIGHT:
                if(rana.x < maxx - 1){
                    rana.isOnLog?relPos+=1:1; // la rana si sposta sul tronco
                    rana.x += 1;
                }
                break;
        }
        // fprintf(fp,"offset in pasto alla pipe %d \n",rana.offsetLogOccupied);
        // fflush(fp);
        rana.offsetLogOccupied=relPos;
        write(p[1], &rana, sizeof(elemento));
        rana.sparato = false;
        rana.idxLogOccupied=-1;
        rana.offsetLogOccupied=-1;
        rana.isOnLog=false;
        // la rana non può andare sotto il marciapiede
        rana.y>offsetMarciapiede?rana.y=offsetMarciapiede:1;
        // nel caso in cui la rana subisca una collisione
        ////mvprintw(1,1    , "coll detection %d",collisionDetection);
    }
    return;
}

void frogIsOnLog(int p3[], int p5[],  elemento rana, elemento logs[]){
    int relPos=0;
    int idxLog=0;
    int previdx=0;
    
    for (int i = 0; i < CORSIE; i++){
        // TO DO
        //  [+] se collisione e logs[i].enemy==false allora decrementa vita!
        if (rana.y == logs[i].y && rana.x < logs[i].x+6 && rana.x >= logs[i].x && logs[i].enemy==false){ // la rana sale se collisione e se non c'è un enemy 
            logs[i].logOccupied = true;
            if (rana.isOnLog==false){
                relPos = (rana.x-logs[i].x);
                idxLog=i;     
                write(p3[1],&relPos,sizeof(int));
                write(p3[1],&idxLog,sizeof(int));
            }
            else if(rana.idxLogOccupied!=i){ // la rana è sul tronco ma cambia tronco!
                relPos = (rana.x-logs[i].x);
                idxLog=i;     
                write(p3[1],&relPos,sizeof(int));
                write(p3[1],&idxLog,sizeof(int));
            }
            write(p5[1], &logs[i], sizeof(elemento));
        }
    }
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