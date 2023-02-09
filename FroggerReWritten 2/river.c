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




void legnetto(int p1[],int p6[], int p8[], int riga){
    srand(getpid());
    close(p1[0]);
    int maxX = 0, maxY = 0, x = 0,direzione = 1, removeEnemy = 0, id = 0;
    int timeLimit = 0, counter = 0;
    getmaxyx(stdscr, maxY, maxX);
    elemento woody;
    timeLimit = 1+rand()%500; //generare un limite di tempo casuale per ogni tronco
    // inizializziamo le posizioni
    woody.y = (riga*2)+offsetFiume-1;
    woody.x=(1+rand()%maxX);
    woody.c = riga+30;   
    woody.enemy=false;
    write(p1[1], &woody, sizeof(elemento));
    while(true){
        removeEnemy = 0;
        read(p8[0],&removeEnemy, sizeof(int));
        id = removeEnemy - 1;
        x = woody.x + direzione;
        // right
        if (removeEnemy > 0 && woody.c == id){
            woody.enemy = false;
            woody.sparato = false;
            write(p6[1], &woody, sizeof(elemento));
            counter = 0;
            timeLimit = 1+rand()%500;
        }
        if(x > maxX-1 || x < 0){
            direzione *= -1;
            //woody.y = 1 + rand()%(maxY-1);
        }
        else{
            woody.x += direzione;
        }
        if (woody.enemy == false){
            if (timeLimit == counter){
                //woody.enemy = true; // RIATTIVAAAA!!!! SE VUOI NEMICII!!
                enemyBullet(p1,p6,woody);
            }
        }
        else if(woody.enemy == true){
            woody.sparato = true;
            write(p6[1], &woody, sizeof(elemento));
            woody.sparato = false;
        }
        write(p1[1], &woody, sizeof(elemento));
        usleep(DELAYT);
        //sleep(); //OGNI QUANTO GENERARE TRONCHI?
        counter++; //delay enemy
    }
    
}

//nemico == woody
void enemyBullet(int p1[],int p6[], elemento nemico){
    pid_t bullet=fork();
    if (bullet==0){
        fprintf(fp,"\n ho generato un processo proiettile \n");
        enemyBulletShoot(p1, p6, nemico);
    }
    //wait(NULL);
}

//aggiorna le posizioni del proiettile
//se counter==30 allora stampa un proiettile
void enemyBulletShoot(int p1[], int p6[],elemento enemy){
    close(p1[0]);
    elemento proiettileNemico, tmp;
    tmp.sparato = false;
    int x=0,y=1;
    int maxY = 0, maxX = 0;
    getmaxyx(stdscr, maxY, maxX);
    int counter = 0;
    proiettileNemico.c = 40+enemy.c; // da modificare quando si aggiunge a home
    bool flag = true;
    tmp.collision = 0;
    while(flag){
        int n = read(p6[0],&tmp, sizeof(elemento));
        if(proiettileNemico.y > 100 && proiettileNemico.c == tmp.c+40){
            proiettileNemico.x = tmp.x;
            proiettileNemico.y = tmp.y;
            proiettileNemico.sparato = false;
        }
        if (tmp.sparato == true){
            proiettileNemico.y += 1;
            proiettileNemico.sparato = true;
            write(p1[1], &proiettileNemico, sizeof(elemento));
        }
        tmp.collision = 0;
        usleep(50000);
    }
    return ;
}
