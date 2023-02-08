#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <stdbool.h>
#include "HighWay.h"


void funzioneMacchina(int p1[], int p2[], int p3[], int id){
    srand(getpid());
    int maxX = 0, maxY = 0, x = 0, y = 0, n = 0, counter = 0;
    getmaxyx(stdscr,maxY,maxX); //only for getting maxX
    maxY=offsetAutostrada;
    elemento macchina;
    int possibleStartY[CORSIE]={0};
    bool flag = true;
    
    
    // generazione corsie possibili per le macchine
    for(int i=0;i<NUMMACCHINE;i++){
        if (i%2!=0 && i != 0){
            possibleStartY[counter]=i;
            counter++;
        }
    }
    // randomicamente decidiamo se il veicolo generato sarà un camion oppure una macchina
    if (1+rand()%6 > 4){
        macchina.type=0; //macchina
    }
    else{
        macchina.type=1; //camion
    }
    // inizializzo la prima posizione della macchina
    macchina.y=possibleStartY[(rand()%5)]+offsetAutostrada;
    macchina.x=(1+rand()%maxX);
    macchina.c=id;
    // scrivo nella pipe 1 la prima posizione della macchina per effettuare un controllo
    write(p1[1],&macchina,sizeof(elemento));
    // controllo che la posizione non sia contestata

    while (flag){
        int num = CORSIE;
        read(p2[0], &num, sizeof(int));
        usleep(DELAYM/10);
        if (num == 1){
            //fprintf(fp,"ho cambiamo la posizione della macchine %d\n", id);
            macchina.y=possibleStartY[(rand()%5)]+offsetAutostrada;
            macchina.x=(1+rand()%maxX);
            write(p1[1],&macchina,sizeof(elemento));
        }
        else if (num == 0){
            flag = false;
        }
        //fprintf(fp, "\nsono bloccato qua %d\n", id);
    }
    //fprintf(fp,"\nciao sono qua\n");
    write(p1[1],&macchina,sizeof(elemento));

    while (true){
        flag = true;
        if (ControlloCollisione(macchina)){ // verifico se c'è stata una collisione
            while(flag){ // se si entro nel while 
                n = 0;
                read(p3[0], &n, sizeof(int)); // leggo dalla pipe 3 n, per controllare se qualche altra macchina ha chiesto di cambiare riga
                if (n == 1){ // se un'altra macchina sta cambiando riga allora aspetto un tempo definito e ci riprovo
                    usleep(50000);
                }
                else{ // se la coda è libera allora imposto la flag a false, occupo il cambio corsia scrivendo 1 e effettuo il cambio
                    flag = false;
                    n = 1;
                    for (int i = 0; i<NUMMACCHINE; i++){
                        write(p3[1], &n, sizeof(int));
                    }
                    macchina.y=possibleStartY[(rand()%5)]+offsetAutostrada;
                    if(macchina.y == 3+offsetAutostrada || macchina.y == 7+offsetAutostrada){
                        macchina.x = maxX-1;
                    }
                    else{
                        macchina.x = 1;
                    }
                    usleep(ATTESACAMBIOCORSIA/2);
                    //write(p1[1], &macchina, sizeof(elemento));
                }
            }
        }
        else{
            if (macchina.y == 3+offsetAutostrada || macchina.y == 7+offsetAutostrada){
                macchina.x -=1;
            }
            else{
                macchina.x += 1;
            }
        }
        usleep(30000);
        write(p1[1], &macchina, sizeof(elemento));
    }
}

bool ControlloCollisione(elemento oggetto){
    bool flag = false;
    int maxx=0, maxy=0;
    getmaxyx(stdscr, maxy, maxx);
    if (oggetto.x < 0 || oggetto.x > maxx || oggetto.y < 0 || oggetto.y >= maxy){
        flag = true;
    }
    return flag;
}

