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


void ffrog(int p1[], int p3[],int p8[],int p9[]){
    elemento rana,tmp;
    int maxx=0,maxy=0, counter = 0, collisionDetection=0;
    getmaxyx(stdscr,maxy,maxx);
    rana.c = 20; //identificativo Rana
    rana.y=offsetMarciapiede;
    rana.x=maxx/2;
    rana.sparato=false;
    rana.isOnLog = false;
    rana.idxLogOccupied=-1;
    rana.offsetLogOccupied=-1;
    int relPos=-1;
    int idxLog=-1;
    write(p1[1],&rana, sizeof(elemento));
    

    while(true){
        // prima di tutto leggo la pipe che mi fa comunicare con la funzione di controllo
        read(p3[0], &(collisionDetection), sizeof(collisionDetection));

        read(p8[0], &(relPos), sizeof(int));
        read(p8[0], &(idxLog), sizeof(int));
        read(p9[0], &tmp, sizeof(elemento)); 
        // fprintf(fp,"P8 P9 collDe %d relPos %d idxLog %d tmp.x %d tmp.y %d tmp.logOccupied %d\n",collisionDetection,relPos,idxLog,tmp.x,tmp.y,tmp.logOccupied);
        // fflush(fp);
        //RANA SUL TRONCO
        

        // se l'intero letto è uguale a 1 allora si è verificata una collisione, quindi riporto la rana alla posizione di partenza
        
        // if (tmp.logOccupied){
        //     tmp.logOccupied = false;
        //     rana.isOnLog = true;
        //     relPos!=-1?rana.x = tmp.x+relPos:1;
        //     rana.idxLogOccupied=idxLog;
        //     // fprintf(fp,"rana.offsetLogOccupied %d | rana.idxLogOccupied",rana.offsetLogOccupied,rana.idxLogOccupied);
        //     // fflush(fp);
        // }
        // else{
        //     rana.isOnLog=false;
        // }
        fprintf(fp,"collisiondet PRIMA%d\n",collisionDetection);
        fflush(fp);
        fprintf(fp,"rana x %d y %d\n tmp.x %d logoccupied %d\n",rana.x,rana.y,tmp.x,tmp.logOccupied);
        fflush(fp);
        if (collisionDetection == 1 ){
            collisionDetection=0;
            rana.y=offsetMarciapiede;
            rana.x = maxx/2;
            write(p1[1],&rana, sizeof(elemento)); // scrivo direttamente, in modo da non dover aspettare il getch
            printf(fp,"collisiondet DOPO %d\n",collisionDetection);
            fflush(fp);
        }
        
        
        // se non si verifica alcuna collisione allora la rana si muoverà come sempre
        else{
            // fprintf(fp,"rana x %d y %d\n",rana.x,rana.y);
            // fflush(fp);
            // if (tmp.logOccupied){
            //     tmp.logOccupied = false;
            //     rana.isOnLog = true;
            //     fprintf(fp,"POCO PRIMA RELPOS rana x %d y %d relpos %d\ntmp.x %d \n",rana.x,rana.y,relPos,tmp.x);
            //     fflush(fp);
            //     relPos!=-1?rana.x = tmp.x+relPos:1;
            //     fprintf(fp,"SUBITO DOPO RELPOS rana x %d y %d\n",rana.x,rana.y);
            //     fflush(fp);
            //     rana.idxLogOccupied=idxLog;
            //     // fprintf(fp,"rana.offsetLogOccupied %d | rana.idxLogOccupied",rana.offsetLogOccupied,rana.idxLogOccupied);
            //     // fflush(fp);
            // }
            // else{
            //     rana.isOnLog=false;
            // }
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
                    if(rana.x < maxx - 1)
                    {
                        rana.isOnLog?relPos+=1:1; // la rana si sposta sul tronco
                        rana.x += 1;
                    }
                    break;
            }
            
        }
        rana.offsetLogOccupied=relPos;
        fprintf(fp,"rana x %d y %d\n",rana.x,rana.y);
        fflush(fp);
        write(p1[1], &rana, sizeof(elemento));
        rana.sparato = false;
        rana.idxLogOccupied=-1;
        rana.offsetLogOccupied=-1;
        rana.isOnLog=false;
        // la rana non può andare sotto il marciapiede
        rana.y>offsetMarciapiede?rana.y=offsetMarciapiede:1;
        fprintf(fp,"FINE WHILE rana x %d y %d\n",rana.x,rana.y);
        fflush(fp);
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

void frogIsOnLog(int p8[], int p9[],  elemento rana, elemento logs[]){
    int relPos=0;
    int idxLog=0;
    int previdx=0;
    for (int i = 0; i < NUMLOGS; i++){
        // fprintf(fp,"log x %d y %d occupied %d\n");
        // fflush(fp);
        // TO D
        //  [+] se collisione e logs[i].enemy==false allora decrementa vita!
        for (int i=0;i<NUMLOGS;i++){
            fprintf(fp,"DOPO FROGISONLOG logs[%d].x %d logs[%d].y %d logsoccupied %d\n",i,logs[i].x,i,logs[i].y,logs[i].logOccupied);
            fflush(fp);
        }
        if (rana.y == logs[i].y && rana.x < logs[i].x+6 && rana.x >= logs[i].x && logs[i].enemy==false){ // la rana sale se collisione e se non c'è un enemy 
            logs[i].logOccupied = true;
            // fprintf(fp,"logs[%d].x %d y %d",i,logs[i].x,logs[i].y);
            // fflush(fp);
            if (rana.isOnLog==false){
                relPos = (rana.x-logs[i].x);
                idxLog=i;     
                write(p8[1],&relPos,sizeof(int));
                write(p8[1],&idxLog,sizeof(int));
            }
            else if(rana.idxLogOccupied!=i){ // la rana è sul tronco ma cambia tronco!
                relPos = (rana.x-logs[i].x);
                idxLog=i;     
                write(p8[1],&relPos,sizeof(int));
                write(p8[1],&idxLog,sizeof(int));
            }
            write(p9[1], &logs[i], sizeof(elemento));
        }
    }
}
