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


void ffrog(int p1[], int p4[], int p5[], int p7[],int stopGame[]){
    bool gioca=true;
    elemento rana, tmp;
    tmp.cambioMovimento=false; // ALTRIMENTI SPAWNA IN ALTO A SINISTRA
    int maxx=0,maxy=0, counter = 0, collisionDetection=0;
    bool ritornaInPosizione = false;
    getmaxyx(stdscr,maxy,maxx);
    // inizializziamo l'oggetto rana, in modo tale che la sua prima posizione sia al centro del marciapiede
    rana.c = 20; //identificativo Rana
    rana.y=offsetMarciapiede;
    rana.x=maxx/2;
    rana.sparato=false;
    write(p1[1],&rana, sizeof(elemento)); //push dati iniziali rana 
    
    while(gioca){
        read(stopGame[0],&gioca,sizeof(gioca));  // gioca è un booleano che è impostato a false quando i processi devono terminare
        // prima di tutto leggo la pipe che mi fa comunicare con la funzione di controllo
        read(p4[0], &(collisionDetection), sizeof(int)); // leggiamo attraverso una pipe non bloccate se la rana ha subito qualche collisione
        read(p5[0], &tmp, sizeof(elemento)); // questa pipe viene utilizzata nel caso in cui la rana si trovi dentro il fiume, per seguire la posizione del tronco sul quale è salita

        //se l'intero letto è uguale a 1 allora si è verificata una collisione, quindi riporto la rana alla posizione di partenza
        if (collisionDetection == 1){
            collisionDetection=0;
            ritornaInPosizione = true;
            write(p1[1],&rana, sizeof(elemento)); // scrivo direttamente, in modo da non dover aspettare il getch
        }
        else if(tmp.cambioMovimento){
            // se tmp.cambioMovimento == true allora la posizione della rana seguirà quella del tronco
            rana.x = tmp.x;
            rana.y = tmp.y;
            tmp.cambioMovimento = false;
        }
        if (ritornaInPosizione){
            // nel caso in cui la rana ha subito una collisione (tana, proiettile, cade sul fiume, viene colpita da una macchina) ritorna alla posizione di partenza per rincominciare la manche
            rana.y=offsetMarciapiede;
            rana.x = maxx/2;
            ritornaInPosizione = false;
        }
        //se non si verifica alcuna collisione allora la rana si muoverà come sempre
        timeout(1);
        int c = getch();
        switch(c) {
            case 32: //barra spaziatrice
                rana.sparato=true;
                // se premiamo la barra spaziatrice allora viene sparato un proiettile
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
        write(p1[1], &rana, sizeof(elemento));
        rana.sparato = false;
        rana.y>offsetMarciapiede?rana.y=offsetMarciapiede:1; // la rana non può andare sotto il marciapiede
    }
    exit(0);
}



void bulletGeneration(int p1[],int p7[], elemento rana){
    pid_t processProiettile = fork();
    if (processProiettile == 0){
        // passo la rana alla funzione spostamento del proiettile
        bulletMovement(p1,p7, rana); //il proiettile è un oggetto dinamico, usiamo un processo
    }
    wait(NULL);
}
void bulletMovement(int p1[],int p7[], elemento rana){
    elemento proiettile;
    proiettile.y = rana.y;
    proiettile.x = rana.x;
    proiettile.c = 21;
    int comunication = 0;
    while(proiettile.y>offsetEndTane){ // il proiettile continua a muoversi verso l'alto a partire dalla rana se non ha raggiunto le tane.
        proiettile.y -= 1; 
        write(p1[1], &proiettile, sizeof(elemento)); //push dati proiettile
        usleep(VELOCITAPROIETTILE); // intervallo di tempo per lo spostamento del proiettile
    }
    proiettile.sparato=false;
}