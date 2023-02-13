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


void funzioneMacchina(int p1[], int p2[], int p3[],int stopGame[],int speedVehicles, int id){
    bool gioca=true; //condizione per esecuzione processo
    srand(getpid()); // la srand viene fatta utilizzando il pid del processo così rendendo il seme ottenuto unico per ogni macchina
    int maxX = 0, maxY = 0, n = 0, counter = 0;
    getmaxyx(stdscr,maxY,maxX); 
    maxY=offsetAutostrada;
    elemento macchina;
    int possibleStartY[CORSIE]={0}; // possibili corsie in cui possono spawnare la macchina
    bool flag = true;
    bool cambioCorsia = true;
    // generazione corsie possibili per le macchine
    for(int i=0;i<NUMMACCHINE;i++){
        if (i%PARI!=0 && i != 0){
            possibleStartY[counter]=i;
            counter++;
        }
    }
    // randomicamente decidiamo se il veicolo generato sarà un camion oppure una macchina
    if (1+rand()%DADO > DADO-PARI){
        macchina.type=0; //macchina
    }
    else{
        macchina.type=1; //camion
    }
    // inizializzo la prima posizione della macchina
    macchina.y=possibleStartY[(rand()%5)]+offsetAutostrada;
    macchina.x=(1+rand()%maxX);
    macchina.c=id;
    // scrivo nella pipe 1 la prima posizione della macchina per effettuare un controllo nel main, funzione controlloGenerazioneMacchine
    write(p1[1],&macchina,sizeof(elemento));
    // controllo che la posizione non sia già stata presa da un'altra macchina
    while (flag){
        int num = CORSIE; // setto la variabile num a 3 così da non entrare in nessuno dei due if sottostanti per sbaglio
        read(p2[0], &num, sizeof(int)); // controlla se c'è stata una collisione e da l'ordine di generare una macchina, 0 va tutto bene 1 c'è stata una collisione
        usleep(speedVehicles/10); 
        //num == 1 o 0, 1 collisione; 0 no collisione
        if (num == 1){ // c'è una collisione, genera un'altra posizione
            macchina.y=possibleStartY[(rand()%5)]+offsetAutostrada;
            macchina.x=(1+rand()%maxX);
            write(p1[1],&macchina,sizeof(elemento));
        }
        else if (num == 0){ // se num == 0 non si sono verificate collisioni in generazione quindi usciamo dal while
            flag = false;
        }
    }
    write(p1[1],&macchina,sizeof(elemento)); // scriviamo alla funzione stampa la macchina generata correttamente per la sua prima stampa a video
    while (gioca){
        read(stopGame[0],&gioca,sizeof(gioca)); // quando l'utente non deve più giocare viene letto un valore false dalla pipe, uscendo dal while
        cambioCorsia = true; // while attivo finchè la macchina non ha fatto il cambio corsia
        if (ControlloCollisione(macchina)){ // verifico se c'è stata una collisione tra macchina e bordo
            while(cambioCorsia){ // se si entro nel while 
                n = 0;
                read(p3[0], &n, sizeof(int)); // leggo dalla pipe 3 n, per controllare se qualche altra macchina ha chiesto di cambiare riga
                if (n == 1){ // se un'altra macchina sta cambiando riga allora aspetto un tempo definito e ci riprovo
                    usleep(ATTESACAMBIOCORSIA*PARI);
                }
                else{ // se la coda è libera allora imposto la cambioCorsia a false, occupo il cambio corsia scrivendo 1 e effettuo il cambio
                    cambioCorsia = false;
                    n = 1;
                    for (int i = 0; i<NUMMACCHINE; i++){
                        write(p3[1], &n, sizeof(int));
                    }
                    // cambia senso macchina e riga
                    macchina.y=possibleStartY[(rand()%5)]+offsetAutostrada;
                    if(macchina.y == 3+offsetAutostrada || macchina.y == 7+offsetAutostrada){
                        macchina.x = maxX-1;
                    }
                    else{
                        macchina.x = 1;
                    }
                    // effettuato il cambio faccio aspettare la macchina prima di continuare il movimento
                    usleep(ATTESACAMBIOCORSIA/2);
                }
            }
        }
        else{ // se la macchina non si trova in uno dei bordi, falle muovere nel senso della corsia
            if (macchina.y == TRE+offsetAutostrada || macchina.y == SETTE+offsetAutostrada){
                macchina.x -=1; // la macchina si sposta verso sinistra
            }
            else{
                macchina.x += 1; // la macchina si sposta verso destra
            }
        } 
        usleep(speedVehicles); // velocità di spostamento veicoli
        write(p1[1], &macchina, sizeof(elemento)); // comunica posizione della macchina alla funzione AreaDiGioco nel main
    }
    exit(0);
}

bool ControlloCollisione(elemento oggetto){
    bool flag = false;
    int maxx=0, maxy=0;
    getmaxyx(stdscr, maxy, maxx);
    // se l'oggetto, in questo caso una macchina, si trova in prossimità di un bordo allora viene restituito true e si entra nel while di cambio corsia
    if (oggetto.x < 0 || oggetto.x > maxx || oggetto.y < 0 || oggetto.y >= maxy){
        flag = true;
    }
    return flag;
}

