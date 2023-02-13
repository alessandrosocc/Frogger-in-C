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

void legnetto(int p1[],int p6[], int p8[],int p9[],int stopGame[], int riga,int speedLegnetto){
    bool gioca=true;
    srand(getpid());
    close(p1[0]);
    int maxX = 0, maxY = 0, x = 0,direzione = 1, removeEnemy = 0, id1 = 0,id2 = 0, comunication = 0;
    int timeLimit = 0, counter = 0;
    getmaxyx(stdscr, maxY, maxX);
    elemento woody;
    timeLimit = 1+rand()%LIMTIMERNEMICI; //generare un limite di tempo casuale per ogni tronco, in modo tale da rendere casuale la nascita di un proiettile
    // inizializziamo le posizioni
    woody.y = (riga*2)+offsetFiume-1; // scegliamo in quale riga posizionare un determinato tronco in base al suo id
    woody.x=(1+rand()%maxX); // la sua x iniziale, invece, viene scelta in maniera casuale
    woody.c = riga+IDTRONCHIOFFSET; 
    woody.enemy=false;
    enemyBullet(p1,p6,p9,woody); // inizializziamo già da qui il processo per la generazioine del proiettile
    write(p1[1], &woody, sizeof(elemento)); // scriviamo il tronco la prima volta per una stampa iniziale
    while(gioca){
        read(stopGame[0],&gioca,sizeof(gioca)); // leggiamo se dobbiamo o meno interrompere il gioco
        removeEnemy = 0; // settiamo a 0 per non interferire con gli if successivi
        comunication = 0; // settiamo a 0 per non interferire con gli if successivi
        id1 = 0; // settiamo a 0 per non interferire con gli if successivi
        id2 = 0; // settiamo a 0 per non interferire con gli if successivi
        read(p9[0], &comunication, sizeof(int));
        read(p8[0],&removeEnemy, sizeof(int));
        id2 = comunication -1; // controlliamo il tronco di appartenenza per capire se il messaggio era o non era riferito a noi (tronco)
        id1 = removeEnemy - 1;
        x = woody.x + direzione; // il tronco si sposta 
        // right
        if (removeEnemy > 0 && woody.c == id1){ // se una rana ha ucciso un nemico viene modificata la stampa del tronco, viene scritto al proiettile di non essere sparato e viene resettato il counter per permettergli di nascere nuovamente
            woody.enemy = false;
            woody.sparato = false;
            write(p6[1], &woody, sizeof(elemento));
            counter = 0;
            timeLimit = 1+rand()%LIMTIMERNEMICI/2;
        }
        // sposta il legnetto
        if(x > maxX-LUNGHEZZATRONCO || x < 0 ){
            direzione *= -1;
        }
        else{
            woody.x += direzione;
        }
        //woody.x=120;
        if (woody.enemy == false){
            if (comunication <= 0 && id2 != woody.c){ // controliamo se è possibile spownare un nemico 
                if (timeLimit == counter){  // se il counter è arrivato al limite allora viene spownato
                    woody.enemy = true; // TRUE PER AVERE I NEMICI
                    counter = 0; // e viene riazzerato il counter per lo sparo del proiettile
                }
            }
            else{
                woody.enemy = false;
                counter = 0;
            }
        }
        else if(woody.enemy == true && counter == TIMERPROIETTILI){ // se il tronco ha un nemico e il counter == limite dello sparo allora un proiettile viene sparato
            woody.sparato = true;
            write(p6[1], &woody, sizeof(elemento));
            woody.sparato = false;
            counter = 0; // riazzeriamo il counter per il prossimo proiettile
        }
        write(p1[1], &woody, sizeof(elemento));
        usleep(speedLegnetto);
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
        enemyBulletShoot(p1, p6,p9, nemico);
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
    proiettileNemico.c = IDTRONCHIPROIETTILIOFFSET+enemy.c; // da modificare quando si aggiunge a home
    proiettileNemico.x = -1;
    proiettileNemico.y = -1;
    bool flag = true;
    while(flag && gioca){
        read(p6[0],&tmp, sizeof(elemento)); // leggo dalla funzione tronco se posso sparare il proiettile
        read(p9[0], &comunication, sizeof(int)); // leggo dalla funzione area di gioco per eventuali interrupt (collisioni con macchine, o con altri proiettili)
        if(tmp.sparato){ // Se il proiettile viene sparato allora ridefiniamo la sua x e la sua y in base a quella del tronco quando l'ha sparato
            proiettileNemico.x = tmp.x+3;
            proiettileNemico.y = tmp.y;
            proiettileNemico.sparato = true;
        }
        if (comunication-1 == proiettileNemico.c && comunication > 0){ // se viene ordinato dalla funzione area di gioco, il proiettile non viene più sparato
            proiettileNemico.sparato = false;
        }

        if (proiettileNemico.y<offsetMarciapiede){ // sino a che il proiettile non arriva al marciapiede continuo a mandarlo avanti 
            proiettileNemico.y += 1;
            write(p1[1], &proiettileNemico, sizeof(elemento));
        }
        
        comunication = 0;
        tmp.sparato = false;
        usleep(DELAYPROIETTILI);
    }
    exit(0);
}
