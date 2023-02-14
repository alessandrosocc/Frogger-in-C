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
#include <signal.h> 
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "frog.h"
#include "home.h"
#include "river.h"

#include "menuGenerator.h"
time_t t;
FILE* fp; 
/*
        PUNTEGGIO
        1. Se rana sale su tronco  = +50
        2. Se rana uccide nemico, +100
        3. Se entra in tana = 500
*/

int main(){
    time(NULL);
    srand(time(NULL));
    fp = fopen("log.txt", "w");
    // #########################
    // Inizializzazione Pipe ###
    // #########################
    int p1[2];
    int p2[2];
    int p3[2];
    int p4[2];
    int p5[2];
    int p6[2];
    int p7[2];
    int p8[2];    
    int p9[2];
    int pipeTempo[2];
    int restartTime[2];
    int stopGame[2];
    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);
    pipe(p5);
    pipe(p6);
    pipe(p7);
    pipe(p8);
    pipe(p9);
    pipe(restartTime);
    pipe(pipeTempo);
    pipe(stopGame);
    // rendiamo alcune pipe non bloccanti
    fcntl(p2[0], F_SETFL, O_NONBLOCK);
    fcntl(p3[0], F_SETFL, O_NONBLOCK);
    fcntl(p4[0], F_SETFL, O_NONBLOCK);
    fcntl(p5[0], F_SETFL, O_NONBLOCK);
    fcntl(p6[0], F_SETFL, O_NONBLOCK);
    fcntl(p7[0], F_SETFL, O_NONBLOCK);
    fcntl(p8[0], F_SETFL, O_NONBLOCK);
    fcntl(p9[0], F_SETFL, O_NONBLOCK);
    fcntl(pipeTempo[0], F_SETFL, O_NONBLOCK);
    fcntl(restartTime[0], F_SETFL, O_NONBLOCK);
    fcntl(stopGame[0], F_SETFL, O_NONBLOCK);
    initScreen(&maxY, &maxX); // inizializzo lo schermo

    // #####################
    // ### Menu Iniziale ###
    // #####################
    int speedVehicles=0; // velocità veicoli, da usare per i vari livelli
    int speedLegnetto=0; // velocità legnetti, da usare per i vari livelli
    char* choices[]={"Inizia a Giocare","Credits"}; // scelte menù principale
    char* choicesDifficulty[]={"Principiante","Intermedio","Esperto","Impossibile"}; // scelte livelli
    char* choicesCredits[]={""};
    int choice=menu("Frogger 2023","Benvenuto in Frogger, un gioco creato con processi, threads e lacrime",choices,2,true,true);
    pid_t openGame=fork(); // processo per musica di apertura del gioco
    if(openGame==0){
        playOpenGame();
    }
    while(choice){ // continua a ciclare finchè non esco dal gioco
        if(choice==2){
            choice=menu("Credits","Alessandro Soccol 60/79/00057, Marco Cosseddu 60/79/00010",choicesCredits,0,false,true);
            pid_t credits=fork(); // musica credits 
            if(credits==0){
                playOpenGame();
            }
        }
        else if(choice==1){
            choice=menu("Difficoltà","Scegli la Difficoltà",choicesDifficulty,4,true,false);
            if(choice){
                // in base al livello di difficoltà scelto vengono ridenfiniti certi parametri per rendere più o meno complicata la vittoria 
                switch(choice){
                    case 1: //principiante
                        lvlvite=11;
                        vite=lvlvite;
                        remainingTime=300000;
                        
                        speedVehicles=30000;
                        speedLegnetto=50000;
                        break;
                    case 2: //intermedio
                        lvlvite=6; 
                        vite=lvlvite;
                        remainingTime=200000; // tempo manche
                        speedVehicles=30000;
                        speedLegnetto=50000;
                    case 3: //difficile
                        lvlvite=4;
                        vite=lvlvite;
                        remainingTime=100000; // tempo manche 
                        speedVehicles=10000;
                        speedLegnetto=30000;
                        break;
                    case 4: //impossibile
                        lvlvite=3;
                        vite=lvlvite;
                        remainingTime=50000; // tempo manche
                        speedVehicles=5000;
                        speedLegnetto=10000;
                        break;
                }
                pid_t startingGame=fork(); // musica inizio gioco
                if(startingGame==0){
                    playStartingGame();
                }
        
                sleep(4); // il tempo di far finire la musica, è un effetto che abbiamo voluto mettere
                clear();
                refresh();
                windowGeneration(); // genero la window
                processGeneration(p1,p2,p3,p4,p5,p6,p7,p8,p9,pipeTempo,restartTime,stopGame,speedVehicles,speedLegnetto);
                wait(NULL);
            }
            
        }
        choice=menu("Frogger 2023","Benvenuto in Frogger, un gioco creato con processi, threads e lacrime",choices,2,true,true);
    }   
    endwin();
    fclose(fp);
    return 0;
}
void playStartingGame(){
    char command[256];
    #ifdef __linux__ // se hai un pc linux, bisogna usare un programma diverso per lanciare la musica
    sprintf(command,"aplay %s","../musica/startingGame.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/startingGame.wav");
        system(command);
    #endif 
    exit(0);
}
void playOpenGame(){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/openSong.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));

    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/openSong.wav");
        system(command);

    #endif
    exit(0);
}
void initScreen(int* maxY, int* maxX){
    initscr(); 
    curs_set(0); // settiamo il cursore per essere invisibile
    noecho(); // non mostare i caratteri che clicco
    keypad(stdscr,1); // attivo i tasti 
    getmaxyx(stdscr,*maxY,*maxX); 
}

void windowGeneration(){
    int nCorsie=3, nFiume = 3, yBox = 0, xBox=0, incYBox = 0, corsie = 1, correnti = 1, righe = 1;
    // definiamo i colori che verranno utilizzato per la grafica 
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_RED);
    init_pair(3,COLOR_WHITE,COLOR_YELLOW);
    init_pair(4,COLOR_WHITE,COLOR_CYAN);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);//antiglitch
    init_pair(6,COLOR_WHITE, COLOR_MAGENTA);
    init_pair(7,COLOR_WHITE,COLOR_BLUE);
    init_pair(8,COLOR_BLACK,COLOR_RED);
    init_pair(9,COLOR_YELLOW,COLOR_BLUE);
    init_pair(10,COLOR_BLACK, COLOR_YELLOW);
    //TEMPO text 
    init_pair(11,COLOR_GREEN,COLOR_BLACK);
    init_pair(12,COLOR_YELLOW,COLOR_BLACK);
    init_pair(13,COLOR_RED,COLOR_BLACK);
    init_pair(14,COLOR_BLACK,COLOR_MAGENTA);
    init_pair(15,COLOR_BLACK,COLOR_RED);
    box(stdscr,0,0);
    int offsetSum=1;
    //punteggio
    for (size_t i = 1; i<= PUNTEGGIOVITE; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(3));
    }
    offsetPunteggio=offsetSum;
    offsetSum+=PUNTEGGIOVITE;
    offsetTane=offsetSum;
    attron(COLOR_PAIR(3));
    mvhline(offsetSum,1,' ',maxX-2);
    attroff(COLOR_PAIR(3));
    offsetSum+=1;
    // attraverso delle variabili globali andremo a definire e dividere ogni porzione della mappa in un settore statico, in modo tale da rendere l'esperienza di gioco più intuitiva
    //tane
    for (size_t i=1;i<=maxX-2;i++){
        
        if (i<=3||(i%(maxX/NTANE)==0) || i>=maxX-3){
            attron(COLOR_PAIR(3));
            mvvline(offsetSum,i,' ',TANE);
            attroff(COLOR_PAIR(3));
        }
        else{
            attron(COLOR_PAIR(9));
            mvvline(offsetSum,i,' ',TANE);
            attroff(COLOR_PAIR(9));
        }
        
    }
    offsetSum+=TANE;
    offsetEndTane=offsetSum;
    //fiume
    for (size_t i = offsetSum; i<= FIUME+offsetSum; i++){
        attron(COLOR_PAIR(9));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(9));
    }
    offsetFiume=offsetSum+1;
    offsetSum+=FIUME;

    //prato
    for (size_t i = offsetSum; i<= offsetSum+PRATO; i++){
        attron(COLOR_PAIR(1));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(1));
    }
    offsetPrato=offsetSum;
    offsetSum+=PRATO;

    // autostrada
    for (size_t i = offsetSum; i<= offsetSum+AUTOSTRADA; i++){
        attron(COLOR_PAIR(15));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(15));
    }
    offsetAutostrada=offsetSum-1; //non so perchè ma vuole quel -1 altrimenti non è ben formattato
    offsetSum+=AUTOSTRADA;
    
    offsetMarciapiede=offsetSum;
    //marciapiede
    for (size_t i = offsetSum; i< MARCIAPIEDE+offsetSum; i++){
        attron(COLOR_PAIR(14));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(14));
    }
    offsetSum+=MARCIAPIEDE;
    offsetTempo=offsetSum;
}

void processGeneration(int p1[],int p2[],int p3[],int p4[],int p5[],int p6[], int p7[], int p8[], int p9[],int pipeTempo[],int restartTime[],int stopGame[],int speedVehicles,int speedLegnetto){
    int via = 0;
    pid_t inizializzatoreProcessi;
    pid_t macchine[NUMMACCHINE];
    pid_t rana,tempoProcess;
    pid_t tronchi[NUMTRONCHI];
    inizializzatoreProcessi = fork();
    if (inizializzatoreProcessi == 0){
        // forco le macchine
        for (int i = 0; i<NUMMACCHINE; i++){
            macchine[i] = fork();
            if (macchine[i] == 0){
                //chiamo la funzione macchine
                funzioneMacchina(p1,p2,p3,stopGame,speedVehicles,i);
            }
        }
        rana = fork();
        if (rana == 0){
            // chiamo la funzione rana
            ffrog(p1,p4,p5,p7,stopGame);
        }
        tempoProcess=fork();
        if (tempoProcess==0){
            tempo(pipeTempo,restartTime,stopGame);
        }
        // forco i tronchi
        for (int i = 0; i<NUMTRONCHI; i++){
            tronchi[i] = fork();
            if (tronchi[i] == 0){
                // chiamo la funzione tronchi
                legnetto(p1,p6,p8,p9,stopGame,i,speedLegnetto);
            }
        }
    }
    else{
        // inizializzo l'area di gioco
        areaDiGioco(p1,p2,p3,p4,p5,p6,p7,p8, p9,pipeTempo,restartTime,stopGame);
    }
    exit(0);

}

void areaDiGioco(int p1[], int p2[], int p3[],int p4[], int p5[], int p6[], int p7[],int p8[], int p9[],int pipeTempo[],int restartTime[],int stopGame[]){
    elemento d; 
    elemento* dptr=&d;
    elemento rana,bull, vecchiaRana; // rana e proiettile rana
    elemento* vecchiaRanaPtr=&vecchiaRana;
    elemento* ranaPtr=&rana;
    elemento* bullPtr=&bull;
    elemento woody[NUMTRONCHI], bullets[NUMTRONCHI]; //tronchi e proiettili nemici sui tronchi
    int frogCollision = 0;
    int frogCollisionChangeLogScore=0;
    int* frogCollisionChangeLogScorePtr=&frogCollisionChangeLogScore;
    int punteggio=0;
    int *punteggioPtr=&punteggio;
    int totaleTaneChiuse=0;
    int* totaleTaneChiusePtr=&totaleTaneChiuse;
    int* frogCollisionPtr=&frogCollision;
    int vecchioWoody[NUMTRONCHI] = {0};
    //inizializziamo macchine
    elemento macchine[NUMMACCHINE];
    int ranaSulTronco = 1;
    int* ranaSulTroncoPtr=&ranaSulTronco;
    int counter = 0;
    int timeSignal=0;
    // mi assicuro che le macchine vengano generate in maniera corretta
    controlloGenerazioneMacchine(p1,p2,p7);
    int timeRestart=1; // è usato come flag nel momento in cui bisogna riavviare la manche
    bool flagTime=1; // è un flag usato nel momento in cui bisogna riavviare la manche

    //inizializzo il ciclo di stampa
    while(gioca){
        erase(); // puliamo la window
        windowGeneration(); // rigeneriamo la window
        mostraPunteggio(punteggio); // mostriamo il punteggio
        mostraVita(vite); // mostra vite
        read(pipeTempo[0],&timeSignal,sizeof(timeSignal)); // leggiamo il tempo che verrà stampato
        displayTime(timeSignal); // mostra la barra del tempo, il tempo è calcolato in un processo, non qua.
        fineMancheThxTime(restartTime,p4,timeSignal,flagTime,punteggioPtr,frogCollisionPtr,timeRestart,stopGame); // controlliamo che il tempo disponbilile non sia finito, in caso contrario la rana perde una manche e una vita
        checkTaneOccupate(taneChiuse,totaleTaneChiusePtr,punteggioPtr);
        read(p1[0], &(d), sizeof(elemento)); // pipe usata per fetch dati con la funzione getDataFromPipe
        getDataFromPipe(p1,dptr,macchine,ranaPtr,bullPtr); // prendiamo i dati dalla pipe
        getTronchiBullets(dptr,woody,bullets); // prendiamo i dati inerenti al fiume dalla pipe 
        checkRanaInTana(frogCollisionPtr, rana,restartTime,timeRestart,punteggioPtr,p4,stopGame); // controlla se la rana è in una tana
        displayTanaChiusa(taneChiuse); // mostra le tane che sono state chiuse; per ogni manche

        checkRanaFiume(timeRestart, restartTime,vecchioWoody,woody,punteggioPtr, ranaPtr, frogCollisionChangeLogScorePtr,  ranaSulTroncoPtr, p5, vecchiaRanaPtr,p9,stopGame); // tutti i controlli da fare quando la rana si trova nel fiume
        collisioneProiettiliMacchine(bullPtr,macchine,bullets,p9); // controllo se un proiettile (di un nemico o della rana) colpisce una macchina, in caso positivo esso scompare
        collisioneProiettileRanaProiettiliNemici(bullPtr,bullets,p9);
        // stampa macchine
        stampaMacchinaCamion(macchine);
        //Stampa Fiume, tronchi, nemici e proiettili nemici
        stampaTronchiNemici(woody,bullets,rana);
        //Stampa Rana e Proiettile Rana
        stampaRanaBullets(rana,bullPtr);
        collisionRanaVehicles(timeRestart, restartTime, p4,frogCollisionPtr,ranaPtr,macchine,punteggioPtr,stopGame); // controllo se la rana è stata investita o meno da una macchina
        proiettiliKillRana(rana, bullets,p4,frogCollisionPtr,punteggioPtr,stopGame); // controllo se la rana è stata, o meno colpita da un proiettile nemico
        //fprintf(fp, "coordinate proiettile (%d,%d)\n", bull.x, bull.y);
        if (bull.sparato){ // per non avere collateral
            ranaKillEnemy(rana,bullPtr,woody,p8,punteggioPtr);
        }
        counter ++;
        refresh();
        read(stopGame[0],&gioca,sizeof(gioca)); // controlla se il gioco è finito, serve per uscire eventualmente dal while.
    }
    return;
}
void checkRanaFiume(int timeRestart, int restartTime[],int vecchioWoody[],elemento woody[],int* punteggioPtr, elemento* ranaPtr, int* frogCollisionChangeLogScorePtr, int* ranaSulTroncoPtr, int p5[],elemento* vecchiaRanaPtr,int p9[],int stopGame[]){
    //CHECK RANA SCENDE DAL FIUME
    if(vecchiaRanaPtr->y==woody[NUMTRONCHI-1].y && ranaPtr->y==woody[NUMTRONCHI-1].y+2){
        ranaPtr->cambioMovimento=true; // la rana si trova o non si trova sul fiume. true = la rana non è nella zona del fiume
        write(p5[1], ranaPtr, sizeof(elemento)); //se la rana scende dal fiume, si scrive alla funzione rana di cambiare la posizione di questa con quella nuova
        ranaPtr->cambioMovimento=false;
        vecchiaRanaPtr->y=NUMMACCHINE; // la vecchia rana viene aggiornata solamente se la rana è sul fiume, viene cambiato in modo tale da non interagire con questo if per sbaglio
    }
    //CHECK RANA SUL FIUME |||| NON CANCELLARE !!!!!! RIATTIVARE!!!! |||||
    if(ranaPtr->y<=offsetPrato && ranaPtr->y>=offsetFiume){ // rana è nel range del fiume [!!!!!!!!]
        ranaPtr->cambioMovimento=true;
        for(int i=0; i<NUMTRONCHI; i++){
            if(woody[i].x!=vecchioWoody[i] && ranaPtr->y == woody[i].y){ // vecchio woody viene utilizzato per controllare se il tronco si è mosso o meno, dato che il tronco quando si trova nei bordi non si muove per un istante
                ranaPtr->x=ranaPtr->x+woody[i].x-vecchioWoody[i];
                //CHECK RANA SUI TRONCHI
                if((ranaPtr->y==woody[i].y && (ranaPtr->x<woody[i].x || ranaPtr->x>=woody[i].x+8))||(woody[i].enemy)){ //rana scende dal tronco e cade sul fiume
                    ranaPtr->x=maxX/2;
                    ranaPtr->y=offsetMarciapiede;
                    if (vite>0){
                        vite--; // DECREMENTA VITA SE CADE DAL TRONCO O SE DAL PRATO CADE NEL FIUME
                        pid_t killato=fork();
                        if(killato==0){
                            playKilled();
                        }
                    }
                    write(restartTime[1], &timeRestart, sizeof(timeRestart)); // riavvio manche
                }
                else if (*frogCollisionChangeLogScorePtr!=i){ // la rana cambia tronco, voglio aggiungere 50 anche se torna sullo stesso
                    *punteggioPtr+=50;
                    *frogCollisionChangeLogScorePtr=i; // tengo traccia di dove sono salito
                }
                //write che serve per comunicare alla funzione rana la nuova posizione di questa ogni volta che cambia con il movimento del tronco, serve anche in caso la rana scenda dal tronco e cada nel fime
                write(p5[1],ranaPtr, sizeof(elemento));
                *ranaSulTroncoPtr += woody[i].c;
                for (int i = 0; i<NUMTRONCHI; i++){
                    write(p9[1],ranaSulTroncoPtr, sizeof(int));
                }
            }
        }
        *vecchiaRanaPtr=*ranaPtr; //aggiorno vecchiaRana utilizzato nel controllo quando la rana passa dal tronco al prato 
    }
    if (ranaPtr->y>=offsetPrato){ // voglio che la rana prenda +50 ogni volta che cambia tronco oppure che torna sul prato oppure è in qualunque altro posto sotto il prato
        *frogCollisionChangeLogScorePtr=-1;// cosi mi aggiunge punteggio anche se la rana torna sul prato e vuole risalire sullo stesso tronco
    }
    if (vite==0 && gioca){// controllo se le vite sono uguali a zero per check rana sul fiume
        clear();
        riprova(punteggioPtr,stopGame);
    }
    //AGGIORNAMENTO OLDLOG
    for(int i=0; i<5; i++){
        vecchioWoody[i]=woody[i].x; //in oldlog vengono la x di ogni tronco che poi verrà confrontato con la nuova nell'iterazione successiva
    }
}
void playKilled(){
    char command[256];
    #ifdef __linux__
        sprintf(command,"aplay %s","../musica/killed.wav");
        system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/killed.wav");
        system(command);
    #endif
    exit(0);
}
void displayTanaChiusa(int taneChiuse[]){ 
    // CHIUDI TANA se è chiusa
    for (size_t i=0;i<NTANE;i++){
        if(taneChiuse[i]==1){ // controlla se la tana è chiusa, 1 = chiusa , 0 = aperta
            chiudiTana(i); // funzione che stampa la tana chiusa colorata
        }
    }
}
void checkRanaInTana(int* frogCollisionPtr, elemento rana, int restartTime[], int timeRestart, int* punteggioPtr, int p4[],int stopGame[]){
    // CHECK RANA IN TANA
    if (rana.y==offsetMarciapiede && rana.x==maxX/2){
        *frogCollisionPtr=1;
    } 
    for (size_t i=0;i<NTANE;i++){ 
        if (rana.y<offsetFiume && (rana.x>i*(maxX/NTANE)) && (rana.x<(i+1)*(maxX/NTANE))){
            if (!taneChiuse[i]){ // se la tana è aperta
                taneChiuse[i]=1; // chiude la tana perchè la rana è dentro
                *punteggioPtr+=PUNTEGGIOXTANA;
                write(restartTime[1], &timeRestart, sizeof(timeRestart)); // riavvia la manche
                usleep(LOADING); // altrimenti la timesignal legge sempre 0 e la barra del tempo impiega troppo a reiniziare
            }
            else if(*frogCollisionPtr){ // tana è chiusa
                 if (vite>0){
                    vite--;
                    pid_t killato=fork(); // musica rana kilata
                    if(killato==0){
                        playKilled(); 
                    }
                }
                else if(vite==0 &&gioca){ // se la rana ha terminato le vite a disposizione viene chiesto se si vuole riprovare 
                    clear();
                    riprova(punteggioPtr,stopGame);
                }
                
            }
            write(p4[1], &*frogCollisionPtr, sizeof(*frogCollisionPtr)); // riporto la rana sul marciapiede perchè è morta
            *frogCollisionPtr=0;
        }
    }
}
void playWinner(){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/winner.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/winner.wav");
        system(command);
    #endif
    exit(0);
}
void checkTaneOccupate(int taneChiuse[],int* totaleTaneChiusePtr,int* punteggioPtr){
    //TANE TUTTE OCCUPATE? -> GIOCATORE HA VINTO
    for (size_t i=0;i<NTANE;i++){
        if(taneChiuse[i]==1){
            *totaleTaneChiusePtr+=1; // conto quante tane sono chiuse
        }
    }
    if (*totaleTaneChiusePtr==NTANE){ // se le tane sono tutte chiuse allora il giocatore ha vinto
        clear();
        refresh();
        if(gioca){
            pid_t winner=fork(); // suoniamo la musica per la vittoria
            if(winner==0){
                playWinner();
            }
        }
        mvprintw(maxY/2,maxX/2,"HAI VINTO!");
        mvprintw(maxY/2+1,maxX/2,"Il tuo score finale è %d",*punteggioPtr);
        refresh();
        sleep(5);
        exit(0);
    }
    else{
        *totaleTaneChiusePtr=0; // le tane non sono tutte chiuse, resetta il contatore e ricontale alla prossima iterazione
    }
}
void fineMancheThxTime(int restartTime[], int p4[],int timeSignal, bool flagTime,int* punteggioPtr,int* frogCollision,int timeRestart, int stopGame[]){
        if (!timeSignal && flagTime){ // flagTime viene asserito ogni volta che bisogna resettare il tempo
            if(vite>0){  //decrementa vita se non chiudi una rana entro la mance
                vite--;
                flagTime=0;
                *frogCollision=0; // modifichiamo frogCollision in modo tale da non registrare più collisioni di quelle necessarie 
            }
            else if(vite==0 && gioca){
                clear();
                refresh();
                riprova(punteggioPtr,stopGame);
            }
            write(p4[1], &frogCollision, sizeof(frogCollision)); // scriviamo alla rana che ha perso la manche 
            write(restartTime[1], &timeRestart, sizeof(timeRestart));  // scriviamo al tempo di rincominciare
            usleep(RELOADTIME); // altrimenti la timesignal legge sempre 0 e la barra del tempo impiega troppo a reiniziare
        }
        if(flagTime==0 && timeSignal!=0){ //il processo creato dalla funzione time invia sempre zeri a caso, quindi cosi gli facciamo decrementare una vita solo se dopo che ha reimpostato il tempo, funziona tutto dandoci il tempo diverso da zero.
            flagTime=1;
        }
}
void getDataFromPipe(int p1[],elemento* d,elemento macchine[], elemento* rana, elemento* bull){
        //se è la rana
        if (d->c==IDRANA){ 
            rana->y=d->y;
            rana->x=d->x;
            rana->c=d->c;
            rana->offsetLogOccupied = d->offsetLogOccupied;
            rana->idxLogOccupied = d->idxLogOccupied;
            rana->isOnLog = d->isOnLog;
            if (d->sparato==true){
                bull->sparato=true;
            }
        }
        //se è il proiettile
        else if(d->c==IDPROIETTILERANA){ 
            bull->x = d->x;
            bull->y = d->y;
        }
        // allora è un veicolo
        else{
            for (int i=0;i<NUMMACCHINE;i++){
                if (d->c == i){ //assegna a macchina iesima
                    macchine[i].x = d->x;
                    macchine[i].y = d->y;
                    macchine[i].c = d->c; 
                    macchine[i].type = d->type;       
                }
            }
        }
}

void mostraVita(int n){
    // mostriamo le vite sotto forma di numero := Vite : <numero>
    attron(COLOR_PAIR(3));
    attron(A_BOLD);
    mvprintw(offsetPunteggio,2,"Vite : ");
    mvprintw(offsetPunteggio,9,"%d",vite);
    attroff(COLOR_PAIR(3));
    attroff(A_BOLD);
    return;
}

void getTronchiBullets(elemento* d,elemento woody[], elemento bullets[]){
    for (int i=0;i<NUMTRONCHI;i++){
            if (d->c == i+OFFSETIDTRONCO){ //assegna a legnetto iesimo
                woody[i].x = d->x;
                woody[i].y = d->y;
                woody[i].c = d->c; 
                woody[i].enemy=d->enemy;
                woody[i].logOccupied = d->logOccupied;
            }
            if (d->c  == i+OFFSETIDTRONCOPROIETTILI){ // se è un proiettile allora d.c=70 <=> 30 (tronco ID) + 40 (proiettile ID)
                bullets[i].x = d->x;
                bullets[i].y = d->y;
                bullets[i].c = d->c;
                bullets[i].sparato = d->sparato;
                if (bullets[i].y==offsetMarciapiede){
                    bullets[i].y=-1;
                }
            }
        }
}

void playProiettile(){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/proiettile.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/proiettile.wav");
        system(command);
    #endif
    
    exit(0);
}
void stampaRanaBullets(elemento rana, elemento* bull){
    // stampa rana
    mvprintw(rana.y,rana.x,"\\/");
    mvprintw(rana.y+1,rana.x,"/\\");
    //proiettile rana, lo stampo sempre se non è arrivato alla fine dell'area di gioco
    if (bull->sparato && bull->y!=offsetEndTane){
        mvprintw(bull->y, bull->x, "*");
        if(!count){ // se cound non è asserito
            // allora lo asserisco e riproduco il suono del proiettile lanciato
            // count mi serve per riprodurre una sola volta la musica e non ogni volta in continuazione che c'è il proiettile.
            pid_t musicProiettile=fork();
            if(musicProiettile==0){
                playProiettile();
            }
            count=1;
        }
    }else{
        count=0;
    }
}

void stampaTronchiNemici(elemento woody[], elemento bullets[], elemento rana){
    // stampa tronchi e nemici
    attron(COLOR_PAIR(9)| A_BOLD);
        for(size_t i = 0; i<NUMTRONCHI; i++){ 
            if(woody[i].enemy==false ){
                // se il tronco non ha alcun nemico sopra, allora la stampa è la seguente
                mvprintw(woody[i].y,woody[i].x,"|------|");
                mvprintw(woody[i].y+1,woody[i].x,"|------|");
            }
            else if(woody[i].enemy){
                // se c' un nemico, invece, esso viene stampato al centro dello schermo
                mvprintw(woody[i].y,woody[i].x,"|--00--|");
                mvprintw(woody[i].y+1,woody[i].x,"|--||--|");
            }
        }
        //stampa proiettili dei nemici sui tronchi sparati
        for (size_t i = 0; i< NUMTRONCHI; i++){
            if (bullets[i].sparato){
                // vengono stampati i proiettili dei nemici, solamente se essi risultano sparati
                mvaddch(bullets[i].y, bullets[i].x, '*');
            }
            
        }
        attroff(COLOR_PAIR(9) | A_BOLD);
}

void collisionRanaVehicles(int timeRestart, int restartTime[],int p4[],int* frogCollision, elemento* rana,elemento macchine[],int* punteggio,int stopGame[]){
        //collisioni
        for(size_t i = 0; i<NUMMACCHINE; i++){
            if(macchine[i].type==1){// camion
                if (rana->x>=macchine[i].x && rana->x<macchine[i].x+7 && rana->y==macchine[i].y && *(frogCollision)==1)
                {
                    // comunico alla pipe3 che la rana ha subito una collisione (scrivo in ffrog)
                    write(p4[1], frogCollision, sizeof(*(frogCollision))); 
                    // se la vita è maggiore di 6 e la rana è in una posizione diversa dalla collisione precedente e siamo in un'iterazion del gioco precedente, allora diminuisci la vita
                    if (vite>0){
                        vite--;
                        *(frogCollision)=0;
                        pid_t killato=fork();
                        if(killato==0){
                            playKilled();
                        }
                        mostraVita(vite);
                        refresh();
                    }
                    else if(vite==0 && gioca){
                        clear();
                        refresh();
                        riprova(punteggio,stopGame);
                    }
                    write(restartTime[1], &timeRestart, sizeof(timeRestart)); // riavvio manche
                }
            }
            else{ //macchina
                if (rana->x>=macchine[i].x && rana->x<macchine[i].x+4 && rana->y==macchine[i].y && *(frogCollision)==1)
                {   
                    // comunico alla pipe3 che la rana ha subito una collisione (scrivo in ffrog)
                    write(p4[1], frogCollision, sizeof(*(frogCollision))); // c'è stata una collisione, riporta la rana al marciapiede
                    // se la vita è maggiore di 6 e la rana è in una posizione diversa dalla collisione precedente e siamo in un'iterazion del gioco precedente, allora diminuisci la vita
                    if (vite>0){
                        vite--;
                        *(frogCollision)=0; 
                        pid_t killato=fork(); // riproduci musica killato
                        if(killato==0){
                            playKilled();
                        }
                        mostraVita(vite);
                        refresh();
                    }
                    else if(vite==0 && gioca){
                        clear();
                        refresh();
                        riprova(punteggio,stopGame);
                    }
                }
            }   
        }
}

void stampaMacchinaCamion(elemento macchine[]){
    // stampa macchine
        for(size_t i = 0; i<CORSIE*MACCHINE; i++){
            if (macchine[i].c!=-1){
                attron(COLOR_PAIR(15)|A_BOLD);
                // stampa camion
                if(macchine[i].type==1){
                    mvprintw(macchine[i].y,macchine[i].x,"/-----\\");
                    mvprintw(macchine[i].y+1,macchine[i].x,"O-----O");
                }
                else{ //macchina
                    mvprintw(macchine[i].y,macchine[i].x,"/--\\");
                    mvprintw(macchine[i].y+1,macchine[i].x,"O--O");

                }   
                attroff(COLOR_PAIR(15)|A_BOLD);
            }
        }
}

void controlloGenerazioneMacchine(int p1[], int p2[], int p7[]){
    int counter = 0;
    int via = 1;
    elemento macchine[NUMMACCHINE];
    elemento data;
    int matricePosizioniIniziali[10][2] = {{0}}; // inizializzo una matrice nella quale verranno salvate le posizioni delle macchine
    int controlloLettura[10] = {0}; // array che controlla se una macchina è stata letta oppure no
    bool band = true; // flag che conclude il while quando non ci sono collisioni tra macchine

    while(band){
    read(p1[0], &data, sizeof(elemento)); // leggo le macchine
    if (counter < NUMMACCHINE){
        for (int i = 0; i < NUMMACCHINE; i++){ // controlla per il numero dei veicoli
            if (data.c == i && controlloLettura[i] == 0){ // continuo a leggere dalla pipe sino a che non ho letto e salvato la posizione iniziale di tutte le macchine 
                macchine[i].x = data.x;
                macchine[i].y = data.y;
                macchine[i].c = data.c;
                macchine[i].type = data.type;
                counter++;
                matricePosizioniIniziali[i][0] = macchine[i].x;
                matricePosizioniIniziali[i][1] = macchine[i].y;
                controlloLettura[i] = 1;
            }
        }
    }
    if (counter == NUMMACCHINE){ // dopo che ho letto e salvato le posizioni di tutt e le macchine
        int collision = 0;
        // controllo le collisioni, controllo per ogni macchina se collide con qualsiasi altra macchina.
        // se trova una collisione allora esco dai for, do ordine alle macchine di generare un'altra posizione e azzero il counter che mi dice se ho letto tutte le macchine (counter) e riazzero anche l'array controllo lettura.
        for (int i = 0; i<NUMMACCHINE; i++){
            for (int j = 0; j<NUMMACCHINE; j++){
                if (i != j){
                    if (matricePosizioniIniziali[i][1] == matricePosizioniIniziali[j][1]){
                        if (!(matricePosizioniIniziali[j][0]> matricePosizioniIniziali[i][0]+7 || matricePosizioniIniziali[j][0]+7 < matricePosizioniIniziali[i][0])){
                            // collisione in generazione
                            // fprintf(fp, "c'è stata una collisione tra la macchina %d e la macchina %d\n", i, j);
                            j = NUMMACCHINE; 
                            i = NUMMACCHINE;
                            counter = 0;
                            // fprintf(fp,"counter: %d\n", counter);
                            collision = 1;
                            // scrivo nella pipe
                            for (int i = 0; i< NUMMACCHINE; i++){
                                write(p2[1], &collision, sizeof(int));
                            }
                            // riazzero l'array che controllava la lettura di ogni singola macchina
                            for (int i = 0; i < NUMMACCHINE; i++){
                                controlloLettura[i] = 0;
                            }
                        }
                    }
                }
            }
        }
        // non ho trovato collisioni, dico alle macchine di muoversi
        if (collision == 0){
            for (int i = 0; i< NUMMACCHINE; i++){
                write(p2[1], &collision, sizeof(int));
            }
            band = false;
        }
        }
    }
    return;
}


void ranaKillEnemy(elemento rana,elemento* ranaProiettile, elemento logs[], int p8[],int* punteggio){
    int removeEnemy = 1;
    if (rana.cambioMovimento){ // se la rana si trova sul fiume
        for (int i = 0; i<NUMTRONCHI; i++){
            if (ranaProiettile->y==logs[i].y && logs[i].enemy && ranaProiettile->sparato){
                if (ranaProiettile->x == logs[i].x+3 || ranaProiettile->x == logs[i].x+4){
                    // il proiettile della rana ha colpito un nemico, deve lui deve sparire
                    removeEnemy += logs[i].c;
                    for (int i = 0; i<NUMTRONCHI; i++){
                        write(p8[1], &removeEnemy, sizeof(int));
                    }
                    ranaProiettile->sparato=false; // scompare proiettile
                    *punteggio+=PUNTEGGIOKILLENEMY;
                }
            }
        }
    }
}

void collisioneProiettiliMacchine(elemento *proiettileRana, elemento macchine[], elemento proiettiliNemici[], int p9[]){
    int comunication = 0;
    
    // controllo se il proiettile di una rana colpisce una delle macchine
    for (int i = 0; i<NUMMACCHINE;i ++){
        if (proiettileRana->y == macchine[i].y){
            if (macchine[i].type == 1){
                if (proiettileRana->x >= macchine[i].x && proiettileRana->x <= macchine[i].x+6){
                    proiettileRana->sparato = false;
                }
            }
            else{
                if (proiettileRana->x >= macchine[i].x && proiettileRana->x <= macchine[i].x+3){
                    proiettileRana->sparato = false;
                }
            }
        }
    }
    // controllo se il proiettile di un nemico colpisce una macchina se questo accade scrivo al proiettile in questione di cancellarsi
    for (int i = 0; i<NUMMACCHINE; i++){
        for (int j = 0; j<NUMTRONCHI; j++){
            if (proiettiliNemici[j].y == macchine[i].y){
                if (macchine[i].type == 1){
                    if (proiettiliNemici[j].x >= macchine[i].x && proiettiliNemici[j].x <= macchine[i].x+6){
                        comunication = 1 + proiettiliNemici[j].c;
                        for (int i = 0; i< NUMTRONCHI; i++){
                            write(p9[1], &comunication, sizeof(int));
                        }
                    }
                }
                else{
                    if (proiettiliNemici[j].x > macchine[i].x && proiettiliNemici[j].x <= macchine[i].x+3){
                        comunication = 1 + proiettiliNemici[j].c;
                        for (int i = 0; i< NUMTRONCHI; i++){
                            write(p9[1], &comunication, sizeof(int));
                        }
                    }
                }
            }
            
        }
    }

}

void collisioneProiettileRanaProiettiliNemici(elemento *ranaProiettile, elemento proiettiliNemici[], int p9[]){
    // controllo se il proiettile della rana collide con un proiettile sparato da un nemico
    int comunication = 0;
    for (int i = 0; i < NUMTRONCHI; i++){
        comunication = 1;// comunica al proiettile nemico di cancellarsi in seguito alla collisione con la rana
        if (ranaProiettile->y == proiettiliNemici[i].y && proiettiliNemici[i].y != 0){
            if (ranaProiettile->x == proiettiliNemici[i].x){
                comunication = 1+proiettiliNemici[i].c;
                for(int i = 0; i<NUMTRONCHI; i++){
                    write(p9[1], &comunication, sizeof(int));
                }
            }
        }
    }
}


void mostraPunteggio(int n){
    attron(COLOR_PAIR(3));
    attron(A_BOLD);
    mvprintw(offsetPunteggio,maxX-MOSTRAPUNTEGGIO,"Punteggio : %d",n);
    attroff(A_BOLD);
    attron(COLOR_PAIR(3));
    return;
}


void chiudiTana(int n){
    // stampa la tana chiusa di colore rosso
    for (size_t i=1;i<=maxX-2;i++){
        attron(COLOR_PAIR(8));
        if ((i>n*(maxX/NTANE)) && (i<(n+1)*(maxX/NTANE))){
            mvvline(offsetTane+1,i,' ',TANE);
        }
        attroff(COLOR_PAIR(8));
    }
}

void proiettiliKillRana(elemento rana, elemento proiettili[], int p4[], int *frogCollision, int* punteggio,int stopGame[]){
    // controllo se un proiettile dei nemici ha hittato la rana, se si dico alla rana che ha perso la manche e di rinascere, con una vita in meno
    for (int i = 0; i<NUMTRONCHI; i++){
        if (proiettili[i].sparato && proiettili[i].y == rana.y && rana.y != 0){
            if (proiettili[i].x == rana.x || proiettili[i].x == rana.x+1){
                if(*frogCollision){
                    vite--;
                    pid_t killato=fork();
                    if(killato==0){
                        playKilled();
                    }
                }
                else if(vite==0 && gioca){
                    clear();
                    refresh();
                    riprova(punteggio,stopGame);
                }
                // riporta alla posizione di partenza
                write(p4[1], frogCollision, sizeof(frogCollision)); // frogCollision è già un puntatore
                *frogCollision=0;
                
            }
        }
    }
}
void playProiettile(){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/proiettile.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/proiettile.wav");
        system(command);
    #endif
    exit(0);
}
void playEndGame(){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/endGame.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/endGame.wav");
        system(command);
    #endif
    exit(0);
}
void riprova(int* punteggio, int stopGame[]){
    char* choices[]={"Si","NO"};
    clear();
    refresh();
    mvprintw(maxY/2,maxX/2,"Hai Perso!");
    mvprintw(maxY/2+1,maxX/2,"Il tuo score finale è %d",*punteggio);
    refresh();
    sleep(4);
    clear();
    if(gioca){ // riproduci musica di fine gioco
        pid_t endGame=fork();
        if(endGame==0){
            playEndGame();
        }
    }
    int choice=menu("HAI PERSO","Vuoi Riprovare?",choices,2,true,true);
    if(choice==0 || choice==2){
        gioca=false; // imposta gioca a false, tutti i processi devono terminare
        for(size_t i=0;i<1000;i++){ // comunico con la pipe a tutti i processi che il nuovo valore di gioca è false, devono uscire dal while
            write(stopGame[1],&gioca,sizeof(gioca));
        }
        return;
    }else{
        // resetto tutto
        vite=lvlvite-1;
        *punteggio=0;
        for(size_t i=0;i<NTANE;i++){
            taneChiuse[i]=0;
        }
    }
}

void tempo(int pipeTempo[],int restartTime[],int stopGame[]){
    int flag=0;
    int secondoSignal=maxX-10;
    while(gioca){
        read(stopGame[0],&gioca,sizeof(gioca));
        // invia ad intervalli di un secondo, un segnale
        write(pipeTempo[1],&secondoSignal,sizeof(secondoSignal));
        secondoSignal--;
        read(restartTime[0],&flag,sizeof(flag));
        if(flag){
            secondoSignal=maxX-10;
            flag=0;
        }
        usleep(remainingTime); 
    }
    exit(0);
    
}
void displayTime(int secondoSegnale){
    if (secondoSegnale>=((maxX-2)/2)){
        attron(COLOR_PAIR(1));
        mvhline(offsetTempo,1,' ',secondoSegnale);
        mvhline(offsetTempo+1,1,' ',secondoSegnale);
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(11));
        mvprintw(offsetTempo,maxX-8,"TEMPO");
        attroff(COLOR_PAIR(11));
    }
    else if(secondoSegnale<((maxX-2)/2) && secondoSegnale>=((maxX-2)/4)){
        attron(COLOR_PAIR(3));
        mvhline(offsetTempo,1,' ',secondoSegnale);
        mvhline(offsetTempo+1,1,' ',secondoSegnale);
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(12));
        mvprintw(offsetTempo,maxX-8,"TEMPO");
        attroff(COLOR_PAIR(12));
    }
    else{
        attron(COLOR_PAIR(2));
        mvhline(offsetTempo,1,' ',secondoSegnale);
        mvhline(offsetTempo+1,1,' ',secondoSegnale);
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(13));
        mvprintw(offsetTempo,maxX-8,"TEMPO");
        attron(COLOR_PAIR(13));
    }
}