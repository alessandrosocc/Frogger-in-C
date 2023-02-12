#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h> //per kill
#include "HighWay.h"
#include "river.h"



//da togliere quando usi vm
#define true 1
#define false 0
#define KILLPROIETTILE 99

void legnetto(int p1[],int p6[], int p8[],int p9[],int stopGame[], int riga){
    bool gioca=true;
    srand(getpid());
    close(p1[0]);
    int maxX = 0, maxY = 0, x = 0,direzione = 1, removeEnemy = 0, id1 = 0,id2 = 0, comunication = 0;
    int timeLimit = 0, counter = 0;
    getmaxyx(stdscr, maxY, maxX);
    elemento woody;
    timeLimit = 1+rand()%100; //generare un limite di tempo casuale per ogni tronco
    // inizializziamo le posizioni
    woody.y = (riga*2)+offsetFiume-1;
    woody.x=(1+rand()%maxX);
    woody.c = riga+30;
    woody.enemy=false;
    enemyBullet(p1,p6,p9,woody);
    write(p1[1], &woody, sizeof(elemento));
    while(gioca){
        read(stopGame[0],&gioca,sizeof(gioca));
        removeEnemy = 0;
        comunication = 0;
        id1 = 0;
        id2 = 0;
        read(p9[0], &comunication, sizeof(int));
        read(p8[0],&removeEnemy, sizeof(int));
        id2 = comunication -1;
        id1 = removeEnemy - 1;
        x = woody.x + direzione;
        // right
        if (removeEnemy > 0 && woody.c == id1){
            woody.enemy = false;
            woody.sparato = false;
            write(p6[1], &woody, sizeof(elemento));
            counter = 0;
            timeLimit = 1+rand()%200;
        }

        if(x > maxX-7 || x < 0 ){
            direzione *= -1;
            //woody.y = 1 + rand()%(maxY-1);
        }
        else{
            woody.x += direzione;
        }
        //woody.x=120;
        if (woody.enemy == false){
            if (comunication <= 0 && id2 != woody.c ){
                if (timeLimit == counter){ 
                    woody.enemy = true; // TRUE PER AVERE I NEMICI

                    counter = 0;
                }
            }
            else{
                woody.enemy = false;
                counter = 0;
            }
        }
        else if(woody.enemy == true && counter == TIMERPROIETTILI){
            woody.sparato = true;
            write(p6[1], &woody, sizeof(elemento));
            woody.sparato = false;
            counter = 0;
        }
        write(p1[1], &woody, sizeof(elemento));
        usleep(DELAYT);
        //sleep(); //OGNI QUANTO GENERARE TRONCHI?
        counter++; //delay enemy
    }
    exit(0);
}

//nemico == woody
void enemyBullet(int p1[],int p6[],int p9[], elemento nemico){
    pid_t bullet=fork();
    int comunication = 0;
    if (bullet==0){
        fprintf(fp,"\n ho generato un processo proiettile\n");
        enemyBulletShoot(p1, p6,p9, nemico);
        fprintf(fp,"ho killato un proiettile\n");
    }
}

//aggiorna le posizioni del proiettile
//se counter==30 allora stampa un proiettile
void enemyBulletShoot(int p1[], int p6[],int p9[], elemento enemy){
    bool gioca=true;
    close(p1[0]);
    elemento proiettileNemico, tmp;
    tmp.sparato = false;
    int maxY = 0, maxX = 0, comunication = 0;
    getmaxyx(stdscr, maxY, maxX);
    proiettileNemico.c = 40+enemy.c; // da modificare quando si aggiunge a home
    proiettileNemico.x = enemy.x;
    proiettileNemico.y = enemy.y;
    bool flag = true;
    while(flag && gioca){
        read(p6[0],&tmp, sizeof(elemento));
        read(p9[0], &comunication, sizeof(int));
        if(tmp.sparato){
            proiettileNemico.x = tmp.x+3;
            proiettileNemico.y = tmp.y;
            proiettileNemico.sparato = true;
        }
        if (comunication-1 == proiettileNemico.c && comunication > 0){
            proiettileNemico.sparato = false;
        }

        if (proiettileNemico.y<offsetMarciapiede){
            proiettileNemico.y += 1;
            write(p1[1], &proiettileNemico, sizeof(elemento));
        }
        
        comunication = 0;
        tmp.sparato = false;
        usleep(40000);
    }
    exit(0);
}
