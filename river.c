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
                } 
            }
        }
    }
    endwin();
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
    // if (rand()%6 > 4){
    //     woody.enemy=true;
    //     enemyBullet(descriptor,woody);
    // }
    
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
                enemyBullet(descriptor,woody);
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
    elemento bullets[5];
    elemento d;
    while (true){
        clear();
        read(p[0],&d,sizeof(elemento));
        for (int i=0;i<5;i++){
            if (d.c == i){ //assegna a legnetto iesimo
                woody[i].x = d.x;
                woody[i].y = d.y;
                woody[i].c = d.c; 
                woody[i].enemy=d.enemy;
            }
            if (d.c - 40 == i){
                bullets[i].x = d.x;
                bullets[i].y = d.y;
                bullets[i].c = d.c;
                bullets[i].sparato = d.sparato;
                
            }
        }       
        // stampa tronchi e nemici
        for(size_t i = 0; i<CORSIE; i++)
        { 
            //DEBUG
            mvprintw(i+20,1,"Posizione woody %d -> y: %d x: %d c: %d enemy: %d, sparato %d",i,woody[i].y,woody[i].x,woody[i].c,woody[i].enemy, woody[i].sparato);
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
        //stampa proiettili quando sparati
        for (size_t i = 0; i< CORSIE; i++){
            if (bullets[i].sparato == true){
                mvaddch(bullets[i].y, bullets[i].x, '*');
                mvprintw(i+40,1,"Posizione bull %d -> y: %d x: %d c: %d",i,bullets[i].y,bullets[i].x,bullets[i].c);
            }
        }
        refresh();
    }
}





//nemico == woody
void enemyBullet(int p[], elemento nemico){
    pid_t bullet=fork();
    if (bullet==0){
        enemyBulletShoot(p, nemico);
    }

}
void enemyBulletShoot(int p[], elemento enemy){
    close(p[0]);
    elemento proiettileNemico;
    int x=0,y=1;
    int counter = 0;
    proiettileNemico.c = 40+enemy.c; // da modificare quando si aggiunge a home
    bool flag = false;
    while(true){
        if (flag == false){
            proiettileNemico.x = enemy.x;
            proiettileNemico.y = enemy.y;  
        }
        if (counter == 30){
            enemy.sparato = true;
            flag = true;
            counter = 0;
        }
        if (enemy.sparato == true){
            proiettileNemico.y+=1;
            //proiettileNemico.y = proiettileNemico.y+1;
            // proiettileNemico.x = proiettileNemico.x+1;
            write(p[1], &proiettileNemico, sizeof(elemento));
        }
        counter++;
        usleep(30000);
    }
}
// void printEnemyBullets(int p[]){
//     close(p[1]);
//     elemento proiettili;
//     elemento data;
//     while(true){
//         read(p[0], &data, sizeof(elemento));
//         mvaddch(data.y, data.x, '*');
//     }
// }
//  void f_proiettile(int p[2]){

//     elemento oggetto = {-1, -1, '^','P'};
//     elemento letto;

//     while(true){
//         read(p[0], &letto, sizeof(elemento)); //lettura della rana
//         usleep(100000);
//         if (letto.c >= 0 && letto.sparato == 1){ //controllo se si tratta della rana 
//              //prendo le coordinate della rana da cui deve partire il proittile
//                 oggetto.x = letto.x;
//                 oggetto.y = letto.y-1;

//                 while(oggetto.y > -1){ //movimento del proiettile 
//                     oggetto.y += 1;
//                     write(p[1], &oggetto, sizeof(elemento));
//                     usleep(100000);
//             }
//         }
//         else{
//             write(p[1], &oggetto, sizeof(elemento)); //se non è la rana quella che legge fa questa write
//             usleep(100000);
//         }
//     }    
//     return;
// }