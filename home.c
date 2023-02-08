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


int main() {
    t=time(NULL);
    fp=fopen("log.txt","w");
    srand(time(NULL));
    int p1[2];
    int p2[2];
    int p3[2];
    int p4[2];
    int p5[2];
    int p8[2];
    int p9[2];
    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);
    pipe(p5);
    pipe(p8);
    pipe(p9);
    fcntl(p2[0], F_SETFL, O_NONBLOCK);
    fcntl(p3[0], F_SETFL, O_NONBLOCK);
    fcntl(p4[0], F_SETFL, O_NONBLOCK);
    fcntl(p5[0], F_SETFL, O_NONBLOCK);
    fcntl(p8[0], F_SETFL, O_NONBLOCK);
    fcntl(p9[0], F_SETFL, O_NONBLOCK);

    
    initScreen(&maxY,&maxX);
    windowGeneration();
    
    pid_t auto0 = fork();
    if (auto0 < 0){
        perror("errore nella generazione della macchina 1");
    }
    else if (auto0 == 0){
        car(p1,p2,0);
    }
    else{
        pid_t auto1=fork();
        if (auto1<0){
            perror("Errore generazione macchina 0");
        }
        else if(auto1==0){
            car(p1,p2,1);
        }
        else{
            pid_t auto2=fork();
            if (auto2<0){
                perror("Errore generazione macchina 0");
            }
            else if(auto2==0){
                car(p1,p2,2);
            }
            else{
                pid_t auto3=fork();
                if (auto3<0){
                    perror("Errore generazione macchina 0");
                }
                else if(auto3==0){
                    car(p1,p2,3);
                }
                else{
                    pid_t auto4=fork();
                    if (auto4<0){
                        perror("Errore generazione macchina 0");
                    }
                    else if(auto4==0){
                        car(p1,p2,4);
                    }
                    else{
                        pid_t auto5=fork();
                        if (auto5<0){
                            perror("Errore generazione macchina 0");
                        }
                        else if(auto5==0){
                            car(p1,p2,5);
                        }
                        else{
                            pid_t auto6=fork();
                            if (auto6<0){
                                perror("Errore generazione macchina 0");
                            }
                            else if(auto6==0){
                                car(p1,p2,6);
                            }
                            else{
                                pid_t auto7=fork();
                                if (auto7<0){
                                    perror("Errore generazione macchina 0");
                                }
                                else if(auto7==0){
                                    car(p1,p2,7);
                                }
                                else{
                                    pid_t auto8=fork();
                                    if (auto8<0){
                                        perror("Errore generazione macchina 0");
                                    }
                                    else if(auto8==0){
                                        car(p1,p2,8);
                                    }
                                    else{
                                        pid_t auto9=fork();
                                        if (auto9<0){
                                            perror("Errore generazione macchina 0");
                                        }
                                        else if(auto9==0){
                                            car(p1,p2,9);
                                        }
                                        else{
                                            pid_t frog = fork();
                                            if (frog == 0){
                                                ffrog(p1,p3,p8,p9);
                                            }
                                            else{
                                                pid_t proiettile=fork();
                                                if (proiettile==0){
                                                    bullet(p1);
                                                }
                                                else{
                                                    pid_t log0 = fork();
                                                    if (log0 == 0){
                                                        legnetto(p1,p4,0);
                                                    }
                                                    else{
                                                        pid_t log1 = fork();
                                                        if (log1 == 0){
                                                            legnetto(p1,p4,1);
                                                        }
                                                        else
                                                        {
                                                            pid_t log2 = fork();
                                                            if (log2 == 0){
                                                                legnetto(p1, p4,2);
                                                            }
                                                            else{
                                                                pid_t log3=fork();
                                                                if (log3==0){
                                                                    legnetto(p1,p4, 3);

                                                                }
                                                                else{
                                                                    pid_t log4=fork();
                                                                    if (log4==0){
                                                                        legnetto(p1,p4, 4);
                                                                    }
                                                                    else{
                                                                        printAll(p1,p2,p3,p4,p5,p8,p9);
                                                                    }      
                                                                } 
                                                            }
                                                        }
                                                    }
                                                } 
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }   
        }
    }
    
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


void printAll(int p[], int p2[], int p3[],int p4[],int p5[],int p8[],int p9[]){
    elemento d; 
    elemento* dptr=&d;
    elemento rana,bull; // rana e proiettile rana
    elemento* ranaPtr=&rana;
    elemento* bullPtr=&bull;
    elemento woody[5], bullets[5]; //tronchi e proiettili nemici sui tronchi
    int frogCollision = 0;
    int* frogCollisionPtr=&frogCollision;
    //inizializziamo macchine
    elemento macchine[CORSIE*MACCHINE];
    for (size_t i = 0; i< CORSIE*MACCHINE; i++){
        macchine[i].x=-1;
        macchine[i].y=-1;
        macchine[i].c=-1;
    }

    int relPos=0;
    int idxLog=0;
    int previdx=0;

    while(true){
        erase();
        windowGeneration();
        mostraVita(vite);
        
        read(p[0], &(d), sizeof(elemento));
        getDataFromPipe(p,dptr,macchine,ranaPtr,bullPtr,woody,bullets);
        // stampa macchine
        stampaMacchinaCamion(macchine);
        //Stampa Rana e Proiettile Rana
        stampaRanaBullets(rana,bull);
        //Stampa Fiume, tronchi, nemici e proiettili nemici
        
        getTronchiBullets(dptr,woody,bullets); // CANCELLA
        collisionRanaVehicles(p3,frogCollisionPtr,ranaPtr,macchine);
        stampaTronchiNemici(woody,bullets,rana);
        for (int i=0;i<NUMLOGS;i++){
            fprintf(fp,"PRIMA DI FROGISONLOG woody[%d].x %d woody[%d].y %d logsoccupied %d\n",i,woody[i].x,i,woody[i].y,woody[i].logOccupied);
            fflush(fp);
        }
        //frogIsOnLog(p8,p9, rana, woody);
        //FROG IS ON LOG
        for (int i = 0; i < NUMLOGS; i++){
        // fprintf(fp,"log x %d y %d occupied %d\n");
        // fflush(fp);
        // TO D
        //  [+] se collisione e woody[i].enemy==false allora decrementa vita!
        for (int i=0;i<NUMLOGS;i++){
            fprintf(fp,"DOPO FROGISONLOG woody[%d].x %d woody[%d].y %d logsoccupied %d\n",i,woody[i].x,i,woody[i].y,woody[i].logOccupied);
            fflush(fp);
        }
        if (rana.y == woody[i].y && rana.x < woody[i].x+6 && rana.x >= woody[i].x && woody[i].enemy==false){ // la rana sale se collisione e se non c'è un enemy 
            woody[i].logOccupied = true;
            // fprintf(fp,"woody[%d].x %d y %d",i,woody[i].x,woody[i].y);
            // fflush(fp);
            if (rana.isOnLog==false){
                relPos = (rana.x-woody[i].x);
                idxLog=i;     
                write(p8[1],&relPos,sizeof(int));
                write(p8[1],&idxLog,sizeof(int));
            }
            else if(rana.idxLogOccupied!=i){ // la rana è sul tronco ma cambia tronco!
                relPos = (rana.x-woody[i].x);
                idxLog=i;     
                write(p8[1],&relPos,sizeof(int));
                write(p8[1],&idxLog,sizeof(int));
            }
            write(p9[1], &woody[i], sizeof(elemento));
        }
    }
    // FROG IS ON LOG
        refresh();
    }
    
}


void mostraVita(int n){
    int x=3;
    for (int i=0;i<n;i++){
        mvaddch(offsetTempo+1,x+=2,'#');
    }
    return;
}

void stampaTronchiNemici(elemento woody[], elemento bullets[],elemento rana){
    // stampa tronchi e nemici
        // for(size_t i = 0; i<CORSIE; i++)
        // { 
        //     attron(COLOR_PAIR(3));
        //     if(woody[i].enemy==false){
        //         mvprintw(woody[i].y,woody[i].x,"/----\\");
        //         mvprintw(woody[i].y+1,woody[i].x,"\\----/");
        //     }
        //     else{//C'è un nemico
        //         mvprintw(woody[i].y,woody[i].x,"/-00-\\");
        //         mvprintw(woody[i].y+1,woody[i].x,"\\-||-/");
        //     }
        // }
        //stampa proiettili dei nemici sui tronchi \addindex sparati
        for (size_t i = 0; i< CORSIE; i++){
            if (bullets[i].sparato == true){
                mvaddch(bullets[i].y, bullets[i].x, '*');
                //debug
                //mvprintw(i+40,1,"Posizione bull %d -> y: %d x: %d c: %d",i,bullets[i].y,bullets[i].x,bullets[i].c);
            }
        }
        for(size_t i = 0; i<NUMLOGS; i++){ 
            attron(COLOR_PAIR(3));
            if(woody[i].enemy==false ){
                if (rana.idxLogOccupied==i){
                    mvprintw(woody[i].y,woody[i].x,sprite1[rana.offsetLogOccupied]);
                    mvprintw(woody[i].y+1,woody[i].x,sprite2[rana.offsetLogOccupied]);
                }
                else{
                    mvprintw(woody[i].y,woody[i].x,"|------|");
                    mvprintw(woody[i].y+1,woody[i].x,"|------|");
                }
            }
            else if(woody[i].enemy==true ){//C'è un nemico
                mvprintw(woody[i].y,woody[i].x,"|--00--|");
                mvprintw(woody[i].y+1,woody[i].x,"|--||--|");
            }
        }
        attroff(COLOR_PAIR(3));
}

void getTronchiBullets(elemento* d,elemento woody[], elemento bullets[]){
    // for (int i=0;i<NUMLOGS;i++){
    //     if (d->c == i+30){ //assegna a legnetto iesimo
    //         woody[i].x = d->x;
    //         woody[i].y = d->y;
    //         woody[i].c = d->c; 
    //         woody[i].enemy=d->enemy;
    //         woody[i].logOccupied=d->logOccupied;
    //     }
    //     if (d->c  == i+70){ // se è un proiettile allora d.c=70 <=> 30 (tronco ID) + 40 (proiettile ID)
    //         bullets[i].x = d->x;
    //         bullets[i].y = d->y;
    //         bullets[i].c = d->c;
    //         bullets[i].sparato = d->sparato;
            
    //     }
    // }
}


void stampaRanaBullets(elemento rana, elemento bull){
        attron(COLOR_PAIR(4));
        // mvprintw(rana.y,rana.x,"\\/");
        // mvprintw(rana.y+1,rana.x,"/\\");
        if (!rana.isOnLog){
            mvprintw(rana.y,rana.x,"\\/");
            mvprintw(rana.y+1,rana.x,"/\\");
        }
        //proiettile rana
        if (bull.sparato == true){
            mvprintw(bull.y, bull.x, "*");
        }
        attroff(COLOR_PAIR(4));
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
void getDataFromPipe(int p[],elemento* d,elemento macchine[], elemento* rana, elemento* bull,elemento woody[],elemento bullets[]){
    for (int i=0;i<NUMLOGS;i++){
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
            for (int i=0;i<CORSIE*MACCHINE;i++){
                if (d->c == i){ //assegna a macchina iesima
                    macchine[i].x = d->x;
                    macchine[i].y = d->y;
                    macchine[i].c = d->c; 
                    macchine[i].type = d->type;       
                }
            }
        }
}


void collisionRanaVehicles(int p3[],int* frogCollision, elemento* rana,elemento macchine[]){
        if (rana->y==offsetMarciapiede && rana->x==maxX/2){
                *(frogCollision)=1;                    
        } 
        //collisioni
        for(size_t i = 0; i<CORSIE*MACCHINE; i++){
            if(macchine[i].type==1){// camion
                if (rana->x>=macchine[i].x && rana->x<macchine[i].x+7 && rana->y==macchine[i].y && *(frogCollision)==1)
                {
                    // comunico alla pipe2 il fatto che le macchine hanno subito una collisione (scrivo in car)
                    fprintf(fp,"collisione camion | vite: %d | frogCollision=%d\n",vite,*(frogCollision));
                    fflush(fp);
                    //write(p2[1], &frogCollision, sizeof(frogCollision));
                    // comunico alla pipe3 che la rana ha subito una collisione (scrivo in ffrog)
                    write(p3[1], frogCollision, sizeof(*(frogCollision))); 
                    
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
                    // comunico alla pipe2 il fatto che le macchine hanno subito una collisione (scrivo in car)
                    //write(p2[1], &frogCollision, sizeof(frogCollision));
                    // comunico alla pipe3 che la rana ha subito una collisione (scrivo in ffrog)
                    write(p3[1], frogCollision, sizeof(*(frogCollision)));
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