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


int offsetFiume = 0; //temp da cancellare


int main(){
    initscr(); 
    curs_set(0); // settiamo il cursore per essere invisibile
    noecho();
    keypad(stdscr,1);

    int p[2];
    pipe(p);
    
    pid_t log0 = fork();
    if (log0 == 0){
        legnetto(p, 0);
    }
    else{
        pid_t log1 = fork();
        if (log1 == 0){
            legnetto(p,1);
            
        }
        else
        {
            pid_t log2 = fork();
            if (log2 == 0){
                legnetto(p, 2);

            }
            else{
                pid_t log3=fork();
                if (log3==0){
                    legnetto(p, 3);

                }
                else{
                    pid_t log4=fork();
                    if (log4==0){
                        legnetto(p, 4);
                    }
                    else{
                        printLog(p);
                    }
                   endwin();
                   } 
            }
        }
    }
}



void legnetto(int descriptor[], int start){
    srand(getpid());

    close(descriptor[0]);
    
    int maxX = 0, maxY = 0, x = 0, startRow = 0, direzione = 1;
    int timeLimit = 0, counter = 0;
    getmaxyx(stdscr, maxY, maxX);
    elemento woody;
    timeLimit = 1+rand()%500; //generare un limite di tempo casuale per ogni tronco
    // inizializziamo le posizioni
    woody.y = (start*2)+offsetFiume;
    woody.x=(1+rand()%maxX);
    woody.c = start;   
    woody.enemy=false; 
    
    //generazione di un nemico!
    if (rand()%6 > 4){
        woody.enemy=true;
        //enemyBullet(woody);
    }
    
    write(descriptor[1], &woody, sizeof(elemento));
    while(true){
        x = woody.x + direzione;
        // right
        if(x > maxX-1 || x < 0){
            direzione *= -1;
            //woody.y = 1 + rand()%(maxY-1);
        }
        else{
                woody.x += direzione;
        }
        if (woody.enemy == false){
            if (timeLimit == counter){
                woody.enemy = true;
                //enemyBullet(woody);
            }
        }
        write(descriptor[1], &woody, sizeof(elemento));
        usleep(15000);
        //sleep(); //OGNI QUANTO GENERARE TRONCHI?
        counter++; //delay enemy
    }
    
}



void printLog(int p[]){
    close(p[1]);
    elemento woody[5];
    elemento d;
    while (true){
        clear();
        read(p[0],&d,sizeof(elemento));
        for (int i=0;i<5;i++){
            if (d.c == i){ //assegna a macchina iesima
                woody[i].x = d.x;
                woody[i].y = d.y;
                woody[i].c = d.c; 
                woody[i].enemy=d.enemy;
            }
        }       
       
        for(size_t i = 0; i<CORSIE; i++)
        { 
            //DEBUG
            mvprintw(i+20,1,"Posizione woody %d -> y: %d x: %d c: %d enemy: %d",i,woody[i].y,woody[i].x,woody[i].c,woody[i].enemy);
            if(woody[i].enemy==false){
                mvprintw(woody[i].y,woody[i].x,"/----\\");
                mvprintw(woody[i].y+1,woody[i].x,"\\----/");
            }
            //C'è un nemico!
            else{
                mvprintw(woody[i].y,woody[i].x,"/-00-\\");
                mvprintw(woody[i].y+1,woody[i].x,"\\-||-/");
            }
            
        }
        refresh();
    }
}

void enemyBullet(elemento enemy){
    int p[2];
    pipe(p);
    pid_t bullet=fork();
    if (bullet==0){
        enemyBulletShoot(p, enemy);
    }
    else{
        printEnemyBullets(p);
    }
}
void enemyBulletShoot(int p[], elemento enemy){
    close(p[0]);
    elemento proiettileNemico;
    int counter = 0;
    proiettileNemico.c = 40+enemy.c; // da modificare quando si aggiunge a home
    while(true){
        if (enemy.sparato == false){
            proiettileNemico.x = enemy.x;
            proiettileNemico.y = enemy.y;   
        }
        if (counter == 30){
            enemy.sparato = true;
        }
        else{
            proiettileNemico.y = proiettileNemico.y+1;
            write(p[1], &proiettileNemico, sizeof(elemento));
        }
        counter++;
    }
}
void printEnemyBullets(int p[]){
    close(p[1]);
    elemento proiettili[5];
    elemento data;
    bool flag = false;
    while(true){
        clear();
        read(p[0], &data, sizeof(elemento));
        for (size_t i = 0; i< CORSIE || flag == false; i++){
            if (data.c-40 == i){// da modificare il -40
                proiettili[i].x = data.x;
                proiettili[i].y = data.y;
                proiettili[i].c = data.c; 
                flag = true;
            }
        }

        for(size_t i = 0; i<CORSIE; i++)
        { 
            mvaddch(proiettili[i].y, proiettili[i].x, '*');
        }
        refresh();

    }

}
