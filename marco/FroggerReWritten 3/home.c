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
#include <fcntl.h>
#include <time.h>
#include <string.h>
//#include "HighWay.h"
#include "frog.h"
#include "home.h"
#include "river.h"

time_t t;
FILE* fp; 


int main(){
    time(NULL);
    srand(time(NULL));
    fp = fopen("log.txt", "w");
    int p1[2];
    int p2[2];
    int p3[2];
    int p4[2];
    int p5[2];
    int p6[2];
    int p7[2];
    int p8[2];    
    int p9[2];
    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);
    pipe(p5);
    pipe(p6);
    pipe(p7);
    pipe(p8);
    pipe(p9);
    fcntl(p2[0], F_SETFL, O_NONBLOCK);
    fcntl(p3[0], F_SETFL, O_NONBLOCK);
    fcntl(p4[0], F_SETFL, O_NONBLOCK);
    fcntl(p5[0], F_SETFL, O_NONBLOCK);
    fcntl(p6[0], F_SETFL, O_NONBLOCK);
    fcntl(p7[0], F_SETFL, O_NONBLOCK);
    fcntl(p8[0], F_SETFL, O_NONBLOCK);
    fcntl(p9[0], F_SETFL, O_NONBLOCK);

    initScreen(&maxY, &maxX); // inizializzo lo schermo
    windowGeneration(); // genero la window
    processGeneration(p1,p2,p3,p4,p5,p6,p7,p8,p9);
    wait(NULL);
    sleep(10);
    endwin();
    fclose(fp);
    return 0;
}

void initScreen(int* maxY, int* maxX){
    initscr(); 
    curs_set(0); // settiamo il cursore per essere invisibile
    noecho();
    keypad(stdscr,1);
    getmaxyx(stdscr,*maxY,*maxX);
}

void windowGeneration(){
    int maxX=0, maxY=0,nCorsie=3, nFiume = 3, yBox = 0, xBox=0, incYBox = 0, corsie = 1, correnti = 1, righe = 1;
    getmaxyx(stdscr,maxY, maxX);
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_RED);
    init_pair(3,COLOR_WHITE,COLOR_YELLOW);
    init_pair(4,COLOR_WHITE,COLOR_CYAN);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);//antiglitch
    init_pair(6,COLOR_WHITE, COLOR_MAGENTA);

    box(stdscr,0,0);

    int offsetSum=1;
    
    //punteggio
    for (size_t i = 1; i<= PUNTEGGIO; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(3));
    }
    
    offsetSum+=3;
    
    //tane
    for (size_t i = offsetSum; i<= offsetSum+TANE; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(3));
    }
    offsetSum+=TANE;

    //fiume
    for (size_t i = offsetSum; i<= FIUME+offsetSum; i++){
        attron(COLOR_PAIR(6));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(6));
    }
    offsetFiume=offsetSum+1;
    offsetSum+=FIUME;

    //prato
    for (size_t i = offsetSum; i<= offsetSum+PRATO; i++){
        attron(COLOR_PAIR(1));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(1));
    }
    offsetSum+=PRATO;

    // autostrada
    for (size_t i = offsetSum; i<= offsetSum+AUTOSTRADA; i++){
        attron(COLOR_PAIR(4));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(4));
    }
    offsetAutostrada=offsetSum-1; //non so perchè ma vuole quel -1 altrimenti non è ben formattato
    offsetSum+=AUTOSTRADA;
    
    offsetMarciapiede=offsetSum;
    //marciapiede
    for (size_t i = offsetSum; i< MARCIAPIEDE+offsetSum; i++){
        attron(COLOR_PAIR(2));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(2));
    }
    offsetSum+=MARCIAPIEDE+1;
    offsetTempo=offsetSum;
    //tempo
    for (size_t i = offsetSum; i<= TEMPO+offsetSum; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', 20);
        attroff(COLOR_PAIR(3));
    }

    //vite
    for (size_t i = offsetSum; i<= VITE+offsetSum; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, maxX-21, ' ', 20);
        attroff(COLOR_PAIR(3));
    }
    //refresh();
    offsetSum+=VITE;
}

void processGeneration(int p1[],int p2[],int p3[],int p4[],int p5[],int p6[], int p7[], int p8[], int p9[]){
    int via = 0;
    pid_t inizializzatoreProcessi;
    pid_t macchine[NUMMACCHINE];
    pid_t rana;
    pid_t tronchi[NUMTRONCHI];
    inizializzatoreProcessi = fork();
    if (inizializzatoreProcessi == 0){
        // forco le macchine
        for (int i = 0; i<NUMMACCHINE; i++){
            macchine[i] = fork();
            if (macchine[i] == 0){
                //chiamo la funzione macchine
                funzioneMacchina(p1,p2,p3,i);
            }
        }
        // forco la rana
        rana = fork();
        if (rana == 0){
            // chiamo la funzione rana
            ffrog(p1,p4,p5,p7);
        }
        // forco i tronchi
        for (int i = 0; i<NUMTRONCHI; i++){
            tronchi[i] = fork();
            if (tronchi[i] == 0){
                // chiamo la funzione tronchi
                legnetto(p1,p6,p8,p9,i);
            }
        }
        
    }
    
    else{
        // inizializzo l'area di gioco
        areaDiGioco(p1,p2,p3,p4,p5,p6,p7,p8, p9);
    }
}

void areaDiGioco(int p1[], int p2[], int p3[],int p4[], int p5[], int p6[], int p7[],int p8[], int p9[]){
    elemento d; 
    elemento* dptr=&d;
    elemento rana,bull, vecchiaRana; // rana e proiettile rana
    elemento* ranaPtr=&rana;
    elemento* bullPtr=&bull;
    elemento woody[NUMTRONCHI], bullets[NUMTRONCHI]; //tronchi e proiettili nemici sui tronchi
    int frogCollision = 0;
    int* frogCollisionPtr=&frogCollision;
    int vecchioWoody[NUMTRONCHI] = {0};
    //inizializziamo macchine
    elemento macchine[NUMMACCHINE];
    int ranaSulTronco = 1;
    int counter = 0;
    // mi assicuro che le macchine vengano generate in maniera corretta
    controlloGenerazioneMacchine(p1,p2,p7);
    
    //inizializzo il ciclo di stampa
    while(true){
        erase();
        windowGeneration();
        mostraVita(vite);
        read(p1[0], &(d), sizeof(elemento));
        getDataFromPipe(p1,dptr,macchine,ranaPtr,bullPtr);
        fprintf(fp,"rana.x %d rana.y %d\n",rana.x,rana.y);
        fflush(fp);
        getTronchiBullets(dptr,woody,bullets);
        //*********************************************************************************************************************************************************************
        //CHECK RANA SCENDE DAL FIUME
        if(vecchiaRana.y==woody[0].y && rana.y==woody[0].y+2){
            rana.cambioMovimento=true;
            write(p5[1], &rana, sizeof(elemento)); //se la rana scende dal fiume, si scrive alla funzione rana di cambiare la posizione di questa con quella nuova
            rana.cambioMovimento=false;
            vecchiaRana.y=10;
        }
        //CHECK RANA SUL FIUME
        if(rana.y<=20 && rana.y>=10){
            rana.cambioMovimento=true;
            for(int i=0; i<5; i++){
                if(woody[i].x!=vecchioWoody[i] && rana.y == woody[i].y){
                    rana.x=rana.x+woody[i].x-vecchioWoody[i];
                    //CHECK RANA SUI TRONCHI
                    if((rana.y==woody[i].y && (rana.x<woody[i].x || rana.x>=woody[i].x+8))||(woody[i].enemy)){ //rana scende dal tronco e cade sul fiume
                        rana.x=maxX/2;
                        rana.y=offsetMarciapiede;
                        vite--; // DECREMENTA VITA SE CADE DAL TRONCO O SE DAL PRATO CADE NEL FIUME
                    }
                    //write che serve per comunicare alla funzione rana la nuova posizione di questa ogni volta che cambia con il movimento del tronco, serve anche in caso la rana scenda dal tronco e cada nel fime
                    write(p5[1],&rana, sizeof(elemento));
                    ranaSulTronco += woody[i].c;
                    for (int i = 0; i<NUMTRONCHI; i++){
                        write(p9[1],&ranaSulTronco, sizeof(int));
                    }
                }
            }
            vecchiaRana=rana; //aggiorno vecchiaRana utilizzato nel controllo quando la rana passa dal tronco al prato 
        }
        //AGGIORNAMENTO OLDLOG
        for(int i=0; i<5; i++){
            vecchioWoody[i]=woody[i].x; //in oldlog vengono la x di ogni tronco che poi verrà confrontato con la nuova nell'iterazione successiva
        }
        //*********************************************************************************************************************************************************************
        collisioneProiettiliMacchine(bullPtr,macchine,bullets,p9);
        collisioneProiettileRanaProiettiliNemici(bullPtr,bullets,p9);
        // stampa macchine
        stampaMacchinaCamion(macchine);
        //Stampa Fiume, tronchi, nemici e proiettili nemici
        stampaTronchiNemici(woody,bullets,rana);
        //Stampa Rana e Proiettile Rana
        stampaRanaBullets(rana,bull);
        collisionRanaVehicles(p4,frogCollisionPtr,ranaPtr,macchine);
        //fprintf(fp, "coordinate proiettile (%d,%d)\n", bull.x, bull.y);
        ranaKillEnemy(rana,bull,woody,p8);
        //frogIsOnLog(p5,p7, rana, woody); 

        
        counter ++;
        refresh();
    }
    return ;
}

void getDataFromPipe(int p1[],elemento* d,elemento macchine[], elemento* rana, elemento* bull){
     //se è la rana
        if (d->c==20){ 
            rana->x=d->x;
            rana->y=d->y;
            
            rana->c=d->c;
            rana->offsetLogOccupied = d->offsetLogOccupied;
            rana->idxLogOccupied = d->idxLogOccupied;
            rana->isOnLog = d->isOnLog;
            if (d->sparato==true){
                bull->sparato=true;
            }
        }
        //se è il proiettile
        else if(d->c==21){ 
            bull->x = d->x;
            bull->y = d->y;
        }
        // allora è la macchina
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
    int x=3;
    for (int i=0;i<n;i++){
        mvaddch(offsetTempo+1,x+=2,'#');
    }
    return;
}

void getTronchiBullets(elemento* d,elemento woody[], elemento bullets[]){
    for (int i=0;i<NUMTRONCHI;i++){
            if (d->c == i+30){ //assegna a legnetto iesimo
                woody[i].x = d->x;
                woody[i].y = d->y;
                woody[i].c = d->c; 
                woody[i].enemy=d->enemy;
                woody[i].logOccupied = d->logOccupied;
            }
            if (d->c  == i+70){ // se è un proiettile allora d.c=70 <=> 30 (tronco ID) + 40 (proiettile ID)
                bullets[i].x = d->x;
                bullets[i].y = d->y;
                bullets[i].c = d->c;
                bullets[i].sparato = d->sparato;
            }
        }
}

void stampaRanaBullets(elemento rana, elemento bull){
    attron(COLOR_PAIR(4));
        mvprintw(rana.y,rana.x,"\\/");
        mvprintw(rana.y+1,rana.x,"/\\");
                //proiettile rana
        if (bull.sparato == true){
            mvprintw(bull.y, bull.x, "*");
        }
        attroff(COLOR_PAIR(4));
}

void stampaTronchiNemici(elemento woody[], elemento bullets[], elemento rana){
    // stampa tronchi e nemici
    attron(COLOR_PAIR(6));
        for(size_t i = 0; i<NUMTRONCHI; i++){ 
            //fprintf(fp, "log %d pos:%d %d\n", woody[i].c, woody[i].x, woody[i].y);
            fflush(fp);
            if(woody[i].enemy==false ){
                mvprintw(woody[i].y,woody[i].x,"|------|");
                mvprintw(woody[i].y+1,woody[i].x,"|------|");
            }
            else if(woody[i].enemy){//C'è un nemico
                mvprintw(woody[i].y,woody[i].x,"|--00--|");
                mvprintw(woody[i].y+1,woody[i].x,"|--||--|");
            }
        }
        //stampa proiettili dei nemici sui tronchi \addindex sparati
        for (size_t i = 0; i< NUMTRONCHI; i++){
            if (bullets[i].sparato){
                mvaddch(bullets[i].y, bullets[i].x, '*');
            }
        }
        attroff(COLOR_PAIR(6));
}

void collisionRanaVehicles(int p4[],int* frogCollision, elemento* rana,elemento macchine[]){
        if (rana->y==offsetMarciapiede && rana->x==maxX/2){
                *(frogCollision)=1;                    
        } 
        //collisioni
        for(size_t i = 0; i<NUMMACCHINE; i++){
            if(macchine[i].type==1){// camion
                if (rana->x>=macchine[i].x && rana->x<macchine[i].x+7 && rana->y==macchine[i].y && *(frogCollision)==1)
                {
                    // comunico alla pipe2 il fatto che le macchine hanno subito una collisione (scrivo in car)
                    fprintf(fp,"collisione camion | vite: %d | frogCollision=%d\n",vite,*(frogCollision));
                    fflush(fp);
                    //write(p2[1], &frogCollision, sizeof(frogCollision));
                    // comunico alla pipe3 che la rana ha subito una collisione (scrivo in ffrog)
                    write(p4[1], frogCollision, sizeof(*(frogCollision))); 
                    
                    //mvprintw(2,2 , "ho scritto sulla pipe %d", frogCollision);
                    //refresh();
                    
                    // se la vita è maggiore di 6 e la rana è in una posizione diversa dalla collisione precedente e siamo in un'iterazion del gioco precedente, allora diminuisci la vita
                    if (vite>0){
                        vite--;
                        *(frogCollision)=0;
                        fprintf(fp,"vite>0? frogCollision %d \n",*(frogCollision));
                        fflush(fp);
                        mostraVita(vite);
                        refresh();
                    }
                    else if(vite==0){
                        clear();
                        refresh();
                        int maxx=0,maxy=0;
                        getmaxyx(stdscr,maxy,maxx);
                        mvprintw(maxy/2,maxx/2,"SCONFITTA");
                        refresh();
                        sleep(5);
                    }
                }
            }
            else{ //macchina
                if (rana->x>=macchine[i].x && rana->x<macchine[i].x+4 && rana->y==macchine[i].y && *(frogCollision)==1)
                {   
                    //write(p2[1], &frogCollision, sizeof(frogCollision));
                    // comunico alla pipe3 che la rana ha subito una collisione (scrivo in ffrog)
                    write(p4[1], frogCollision, sizeof(*(frogCollision)));
                    fprintf(fp,"collisione macchina | vite: %d | frogCollision=%d\n",vite,*(frogCollision));
                    fflush(fp);
                    fprintf(fp,"sizeof %ld sizeof int %ld",sizeof(*(frogCollision)),sizeof(int));
                    fflush(fp);
                    // se la vita è maggiore di 6 e la rana è in una posizione diversa dalla collisione precedente e siamo in un'iterazion del gioco precedente, allora diminuisci la vita
                    if (vite>0){
                        vite--;
                        *(frogCollision)=0;
                        fprintf(fp,"vite>0? frogCollision %d \n",*(frogCollision));
                        fflush(fp);
                        mostraVita(vite);
                        refresh();
                    }
                    else if(vite==0){
                        clear();
                        refresh();
                        int maxx=0,maxy=0;
                        getmaxyx(stdscr,maxy,maxx);
                        mvprintw(maxy/2,maxx/2,"SCONFITTA");
                        refresh();
                        sleep(5);
                    }
                }
            }   
        }
}

void stampaMacchinaCamion(elemento macchine[]){
    // stampa macchine
        for(size_t i = 0; i<CORSIE*MACCHINE; i++){
            if (macchine[i].c!=-1){
                attron(COLOR_PAIR(4));
                // stampa camion
                if(macchine[i].type==1){
                    mvprintw(macchine[i].y,macchine[i].x,"/-----\\");
                    mvprintw(macchine[i].y+1,macchine[i].x,"O-----O");
                }
                else{ //macchina
                    mvprintw(macchine[i].y,macchine[i].x,"/--\\");
                    mvprintw(macchine[i].y+1,macchine[i].x,"O--O");

                }   
                attroff(COLOR_PAIR(4));
            }
        }
}

void controlloGenerazioneMacchine(int p1[], int p2[], int p7[]){
    int counter = 0;
    int via = 1;
    elemento macchine[NUMMACCHINE];
    elemento data;
    int matricePosizioniIniziali[10][2] = {{0}};
    int controlloLettura[10] = {0};
    bool band = true;

    while(band){
    read(p1[0], &data, sizeof(elemento));
    if (counter < 10){
        for (int i = 0; i < 10; i++){
            if (data.c == i && controlloLettura[i] == 0){
                macchine[i].x = data.x;
                macchine[i].y = data.y;
                macchine[i].c = data.c;
                macchine[i].type = data.type;
                counter++;
                matricePosizioniIniziali[i][0] = macchine[i].x;
                matricePosizioniIniziali[i][1] = macchine[i].y;
                controlloLettura[i] = 1;
                fprintf(fp, "counter: %d\n", counter);
            }             
        }
    }
    if (counter == 10){
        int collision = 0;
        // devo avviare il controllo
        for (int i = 0; i<10; i++){
            fprintf(fp, "la macchina %d, si trova in posizione (x: %d, y: %d)\n",i,matricePosizioniIniziali[i][0],matricePosizioniIniziali[i][1]);
        }
        fprintf(fp, "\n\n");
        
        for (int i = 0; i<10; i++){
            for (int j = 0; j<10; j++){
                if (i != j){
                    if (matricePosizioniIniziali[i][1] == matricePosizioniIniziali[j][1]){
                        if (!(matricePosizioniIniziali[j][0]> matricePosizioniIniziali[i][0]+7 || matricePosizioniIniziali[j][0]+7 < matricePosizioniIniziali[i][0])){
                            // collisione in generazione
                            fprintf(fp, "c'è stata una collisione tra la macchina %d e la macchina %d\n", i, j);
                            j = 10; 
                            i = 10;
                            counter = 0;
                            fprintf(fp,"counter: %d\n", counter);
                            collision = 1;
                            // scrivo nella pipe
                            for (int i = 0; i< 10; i++){
                                write(p2[1], &collision, sizeof(int));
                            }
                            // riazzero l'array che controllava la lettura di ogni singola macchina
                            for (int i = 0; i < 10; i++){
                                controlloLettura[i] = 0;
                            }
                        }
                    }
                }
            }
        }
        if (collision == 0){
            for (int i = 0; i< 10; i++){
                write(p2[1], &collision, sizeof(int));
            }
            band = false;
            fprintf(fp, "non ci sono state collisioni");
        }
        }
    }
    return;
}

void frogIsOnLog(int p5[], int p7[],  elemento rana, elemento logs[]){
    int relPos=0;
    int idxLog=0;
    
    
    for (int i = 0; i < NUMTRONCHI; i++){
        // TO DO
        //  [+] se collisione e logs[i].enemy==false allora decrementa vita!
        if (rana.y == logs[i].y && rana.x < logs[i].x+6 && rana.x >= logs[i].x && logs[i].enemy==false){ // la rana sale se collisione e se non c'è un enemy 
            logs[i].logOccupied = true;
            if (rana.isOnLog==false){
                    relPos = (rana.x-logs[i].x);
                    idxLog=i;     
                    write(p5[1],&relPos,sizeof(int));
                    write(p5[1],&idxLog,sizeof(int));
                }
            else if(rana.idxLogOccupied!=i){ // la rana è sul tronco ma cambia tronco!
                relPos = (rana.x-logs[i].x);
                idxLog=i;     
                write(p5[1],&relPos,sizeof(int));
                write(p5[1],&idxLog,sizeof(int));
            }
            write(p7[1], &logs[i], sizeof(elemento));
        }
    }
}

void ranaKillEnemy(elemento rana,elemento ranaProiettile, elemento logs[], int p8[]){
    int removeEnemy = 1;
    if (rana.cambioMovimento){
        for (int i = 0; i<NUMTRONCHI; i++){
            if (ranaProiettile.y==logs[i].y && logs[i].enemy && ranaProiettile.sparato){
                if (ranaProiettile.x == logs[i].x+3 || ranaProiettile.x == logs[i].x+4){
                    // il proiettile della rana ha colpito un nemico, deve lui deve sparire
                    removeEnemy += logs[i].c;
                    for (int i = 0; i<NUMTRONCHI; i++){
                        write(p8[1], &removeEnemy, sizeof(int));
                    }
                    
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
    // controllo se il proiettile di un nemico colpisce una macchina
    for (int i = 0; i<NUMMACCHINE; i++){
        for (int j = 0; j<NUMTRONCHI; j++){
            if (proiettiliNemici[j].y == macchine[i].y){
                if (macchine[i].type == 1){
                    if (proiettiliNemici[j].x >= macchine[i].x &&              proiettiliNemici[j].x <= macchine[i].x+6){
                        comunication = 1 + proiettiliNemici[j].c;
                        //write(p9[1], &comunication, sizeof(int));
                        for (int i = 0; i< NUMTRONCHI; i++){
                            write(p9[1], &comunication, sizeof(int));
                        }
                    }
                }
                else{
                    if (proiettiliNemici[j].x > macchine[i].x && proiettiliNemici[j].x <= macchine[i].x+3){
                        comunication = 1 + proiettiliNemici[j].c;
                        //write(p9[1], &comunication, sizeof(int));
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
        int comunication = 0;
        for (int i = 0; i < NUMTRONCHI; i++){
            comunication = 1;
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
