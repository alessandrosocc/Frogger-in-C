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
    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);
    pipe(p5);
    pipe(p6);
    pipe(p7);
    fcntl(p2[0], F_SETFL, O_NONBLOCK);
    fcntl(p3[0], F_SETFL, O_NONBLOCK);
    fcntl(p4[0], F_SETFL, O_NONBLOCK);
    fcntl(p5[0], F_SETFL, O_NONBLOCK);
    fcntl(p6[0], F_SETFL, O_NONBLOCK);
    fcntl(p7[0], F_SETFL, O_NONBLOCK);

    initScreen(&maxY, &maxX); // inizializzo lo schermo
    windowGeneration(); // genero la window
    processGeneration(p1,p2,p3,p4,p5,p6,p7);
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
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(3));
    }
    offsetFiume=offsetSum-1;
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

void processGeneration(int p1[],int p2[],int p3[],int p4[],int p5[],int p6[], int p7[]){
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
            ffrog(p1,p4,p5);
        }
        // forco i tronchi
        for (int i = 0; i<NUMTRONCHI; i++){
            tronchi[i] = fork();
            if (tronchi[i] == 0){
                // chiamo la funzione tronchi
                legnetto(p1,p6,i);
            }
        }
        
    }
    
    else{
        // inizializzo l'area di gioco
        areaDiGioco(p1,p2,p3,p4,p5,p6,p7);
    }
}

void areaDiGioco(int p1[], int p2[], int p3[],int p4[], int p5[], int p6[], int p7[]){
    elemento d; 
    elemento* dptr=&d;
    elemento rana,bull; // rana e proiettile rana
    elemento* ranaPtr=&rana;
    elemento* bullPtr=&bull;
    elemento woody[NUMTRONCHI], bullets[NUMTRONCHI]; //tronchi e proiettili nemici sui tronchi
    int frogCollision = 0;
    int* frogCollisionPtr=&frogCollision;
    //inizializziamo macchine
    elemento macchine[NUMMACCHINE];
    // mi assicuro che le macchine vengano generate in maniera corretta
    controlloGenerazioneMacchine(p1,p2,p7);
    //inizializzo il ciclo di stampa
    while(true){
        erase();
        windowGeneration();
        mostraVita(vite);
        read(p1[0], &(d), sizeof(elemento));
        getDataFromPipe(p1,dptr,macchine,ranaPtr,bullPtr);
        // stampa macchine
        stampaMacchinaCamion(macchine);
        //Stampa Rana e Proiettile Rana
        stampaRanaBullets(rana,bull);
        //Stampa Fiume, tronchi, nemici e proiettili nemici
        getTronchiBullets(dptr,woody,bullets);
        stampaTronchiNemici(woody,bullets);
        collisionRanaVehicles(p4,frogCollisionPtr,ranaPtr,macchine);

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

void stampaTronchiNemici(elemento woody[], elemento bullets[]){
    // stampa tronchi e nemici
        for(size_t i = 0; i<NUMTRONCHI; i++)
        { 
            attron(COLOR_PAIR(3));
            if(woody[i].enemy==false){
                mvprintw(woody[i].y,woody[i].x,"/----\\");
                mvprintw(woody[i].y+1,woody[i].x,"\\----/");
            }
            else{//C'è un nemico
                mvprintw(woody[i].y,woody[i].x,"/-00-\\");
                mvprintw(woody[i].y+1,woody[i].x,"\\-||-/");
            }
        }
        //stampa proiettili dei nemici sui tronchi \addindex sparati
        for (size_t i = 0; i< NUMTRONCHI; i++){
            if (bullets[i].sparato == true){
                mvaddch(bullets[i].y, bullets[i].x, '*');
                //debug
                //mvprintw(i+40,1,"Posizione bull %d -> y: %d x: %d c: %d",i,bullets[i].y,bullets[i].x,bullets[i].c);
            }
        }
        attroff(COLOR_PAIR(3));
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
    write(p7[1], &via, sizeof(int));
    return;

}
