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

#define NUMLOGS 5

int offsetMarciapiede=29;
int offsetFiume=10;
char* sprite1[]={"|\\/----|","|-\\/---|","|--\\/--|","|---\\/-|","|----\\/|"};
char* sprite2[]={"|/\\----|","|-/\\---|","|--/\\--|","|---/\\-|","|----/\\|"};

time_t t;
FILE* fp; 

typedef struct{
    int x;
    int y;
    int c; //0-9 identificativo macchina ; 20 = rana ; 21 = proiettile ; 30-35 legnetto ; 40 - 45 proiettili nemici su tronco
    int type; //camion o auto?
    bool sparato;
    bool enemy; //il tronco ha un nemico sopra? gestito in river.c
    bool isOnLog;
    bool logOccupied;
    int idxLogOccupied;
    int offsetLogOccupied;
    int collision; 
}elemento;

void processGeneration(int[],int[],int[], int[], int[]);
void printAll(int[], int[], int[], int [], int[]);
void ffrog(int[], int[], int[]);
void bullet(int[]);
void legnetto(int[],int[], int);
void enemyBullet(int [],int [],elemento);
void enemyBulletShoot(int[],int[], elemento);
void frogIsOnLog(int p3[], int p5[],  elemento, elemento[]);

int main(){
    initscr(); 
    curs_set(0); // settiamo il cursore per essere invisibile
    noecho();
    keypad(stdscr,1);
    fp=fopen("log.txt","w");
    int p1[2];
    int p2[2];
    int p3[2];
    int p4[2];
    int p5[2];
    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);
    pipe(p5);
    fcntl(p2[0], F_SETFL, O_NONBLOCK);
    fcntl(p3[0], F_SETFL, O_NONBLOCK);
    fcntl(p4[0], F_SETFL, O_NONBLOCK);
    fcntl(p5[0], F_SETFL, O_NONBLOCK);
    processGeneration(p1,p2,p3,p4,p5);
    return 0;
}

void processGeneration(int p1[],int p2[],int p3[], int p4[], int p5[]){
    pid_t processInit = fork();
    if (processInit == 0){
        pid_t frog = fork();
        if (frog == 0){
            ffrog(p1,p3,p5);
        }
        pid_t proiettile=fork();
        if (proiettile==0){
            bullet(p1);
        }
        pid_t logs[NUMLOGS];
        for(size_t i = 0; i<NUMLOGS; i++){
            logs[i] = fork();
            if (logs[i] == 0){
                legnetto(p1,p4,i);
            }
        }
    }
    else{
        printAll(p1,p2,p3,p4,p5);
        sleep(5);
        endwin();
    }
}

void printAll(int p1[], int p2[], int p3[], int p4[], int p5[]){
    erase();
    elemento logs[NUMLOGS], bullets[NUMLOGS];
    elemento rana;
    elemento proiettile;
    elemento data;
    int counter = 0; //++++
    while(true){
        erase();
        read(p1[0], &data, sizeof(elemento));
        if (data.c == 20){
            rana = data;
            if (data.sparato == true){
                proiettile.sparato = true;
            }
        }
        else if(data.c == 21){
            proiettile = data;
        }
        else{
            for(int i = 0; i< NUMLOGS; i++){
                if( data.c == i+30){
                    logs[i] = data;
                }
                else if (data.c == i+70){
                    bullets[i] = data;
                }
            }
        }
        // fprintf(fp,"rana.x %d rana.y %d\n",rana.x,rana.y);
        // fflush(fp);

        if (!rana.isOnLog){
            mvprintw(rana.y,rana.x,"\\/");
            mvprintw(rana.y+1,rana.x,"/\\");
        }
        for(size_t i = 0; i<NUMLOGS; i++){ 
            if(logs[i].enemy==false ){
                if (rana.idxLogOccupied==i){
                    mvprintw(logs[i].y,logs[i].x,sprite1[rana.offsetLogOccupied]);
                    mvprintw(logs[i].y+1,logs[i].x,sprite2[rana.offsetLogOccupied]);
                }
                else{
                    mvprintw(logs[i].y,logs[i].x,"|------|");
                    mvprintw(logs[i].y+1,logs[i].x,"|------|");
                }
            }
            else if(logs[i].enemy==true ){//C'è un nemico
                mvprintw(logs[i].y,logs[i].x,"|--00--|");
                mvprintw(logs[i].y+1,logs[i].x,"|--||--|");
            }
        }

        //stampa proiettili dei nemici sui tronchi sparati
        for (size_t i = 0; i < NUMLOGS; i++){
            if (bullets[i].sparato == true){
                mvaddch(bullets[i].y, bullets[i].x, '*');
            }
        }

        mvprintw(7,1,"frog is on log: %d", rana.isOnLog);
        mvprintw(6,1,"pos rana : x %d y %d ",rana.x,rana.y);
        mvprintw(5,1,"rana.idxLogOccupied %d | rana.offsetLogOccupied %d/5 | rana.isOnLog %d",rana.idxLogOccupied,rana.offsetLogOccupied,rana.isOnLog);
        if (rana.idxLogOccupied!=-1){
            mvprintw(8,1,"log occupied pos x %d y %d ",logs[rana.idxLogOccupied].x,logs[rana.idxLogOccupied].x);
        }

        // proiettile rana
        if (proiettile.sparato == true){
            mvprintw(proiettile.y, proiettile.x, "*");
        }
        
        frogIsOnLog(p3,p5, rana, logs);
        refresh();
    }
}
    

void legnetto(int descriptor[], int connection[], int start){
    srand(getpid());
    close(descriptor[0]);
    int maxX = 0, maxY = 0, x = 0, startRow = 0, direzione = 1;
    int timeLimit = 0, counter = 0;
    getmaxyx(stdscr, maxY, maxX);
    elemento woody;
    timeLimit = 1+rand()%500; //generare un limite di tempo casuale per ogni tronco
    // inizializziamo le posizioni
    woody.y = (start*2)+offsetFiume+1;
    woody.x = (1+rand()%maxX);
    woody.c = start+30;   
    woody.enemy = false; 
    
    //generazione di un nemico!
    // if (rand()%6 > 4){
    //     woody.enemy=true;
    //     
    // }
    
    write(descriptor[1], &woody, sizeof(elemento));
    while(true){
        x = woody.x + direzione;
        // right
        if(x+7 >= maxX-1 || x < 0){
            direzione *= -1;
            //woody.y = 1 + rand()%(maxY-1);
        }
        else{
            woody.x += direzione;
        }
        if (woody.enemy == false){
            if (counter == timeLimit){
                //woody.enemy = true; // METTI A TRUE PER AVERE I NEMICI!!!!!!!!!!!!!!!!!!!!!
                enemyBullet(descriptor, connection, woody);
            }
            counter++;
        }
        if (woody.enemy == true){
            woody.sparato = true;
            write(connection[1], &woody, sizeof(elemento));
            woody.sparato = false;
        }
        write(descriptor[1], &woody, sizeof(elemento));
        usleep(150000);
    }
    
}

//nemico == woody
void enemyBullet(int p[], int connectionPipe[], elemento nemico){
    pid_t bullet=fork();
    if (bullet==0){
        enemyBulletShoot(p,connectionPipe, nemico);
    }
}

//aggiorna le posizioni del proiettile
//se counter==30 allora stampa un proiettile
void enemyBulletShoot(int p[],int connection[], elemento enemy){
    close(p[0]);
    elemento proiettileNemico, tmp;
    tmp.sparato = false;
    int x=0,y=1;
    int maxY = 0, maxX = 0;
    getmaxyx(stdscr, maxY, maxX);
    int counter = 0;
    proiettileNemico.c = 40+enemy.c; // da modificare quando si aggiunge a home
    bool flag = false;
    while(true){
        read(connection[0],&tmp, sizeof(tmp));
        if(proiettileNemico.y > 100){
            proiettileNemico.x = tmp.x;
            proiettileNemico.y = tmp.y;
        }
        if (tmp.sparato == true){
            proiettileNemico.y += 1;
            write(p[1], &proiettileNemico, sizeof(elemento));
        }
        usleep(30000);
    }
}

void ffrog(int p[], int p3[], int p5[]){
    elemento rana, tmp;
    int maxx=0,maxy=0, counter = 0, collisionDetection=0, direzione = 1, x = 0;
    getmaxyx(stdscr,maxy,maxx);
    rana.c = 20; //identificativo Rana
    rana.y=maxy/2;//offsetMarciapiede;
    rana.x=maxx/2;
    fprintf(fp,"rana.y %d rana.x %d\n",rana.y,rana.x);
    fflush(fp);
    rana.sparato=false;
    rana.isOnLog = false;
    rana.idxLogOccupied=-1;
    rana.offsetLogOccupied=-1;
    int relPos=0;
    int idxLog=0;
    write(p[1],&rana, sizeof(elemento));
    while(true){
        // prima di tutto leggo la pipe che mi fa comunicare con la funzione di controllo
        read(p3[0], &(relPos), sizeof(int));
        read(p3[0], &(idxLog), sizeof(int));

        read(p5[0], &tmp, sizeof(elemento)); 
        //fprintf(fp,"x %d y %d c %d onfrog %d\n",tmp.x,tmp.y,tmp.c,tmp.isOnLog);
        // se l'intero letto è uguale a 1 allora si è verificata una collisione, quindi riporto la rana alla posizione di partenza
        //rana.collision = collisionDetection;
        // if (rana.collision == 1){
        //     //collisionDetection=0;
        //     //fflush(fp);
        //     rana.y=offsetMarciapiede;
        //     rana.x = maxx/2;
        //     write(p[1], &rana, sizeof(elemento)); // scrivo direttamente, in modo da non dover aspettare il getch
        // }

        //mvprintw(1,1,"ranax %d ranay %d tmp.x %d tmp.y %d",rana.x,rana.y,tmp.x,tmp.y);
        if (tmp.logOccupied){
            tmp.logOccupied = false;
            rana.isOnLog = true;
            rana.x = tmp.x+relPos;
            rana.idxLogOccupied=idxLog;
            // fprintf(fp,"rana.offsetLogOccupied %d | rana.idxLogOccupied",rana.offsetLogOccupied,rana.idxLogOccupied);
            // fflush(fp);
        }
        else{
            rana.isOnLog=false;
        }
        
        timeout(1);
        int c = getch();
        switch(c) {
            case 32: //barra spaziatrice
                rana.sparato=true;
                break;
            case KEY_UP:
                if(rana.y > 0)
                    rana.y -= 2;
                    break;
            case KEY_DOWN:
                if(rana.y < maxy - 1)
                    rana.y += 2;
                    break;
            case KEY_LEFT:
                if(rana.x > 0)
                {
                    
                    rana.isOnLog?relPos-=1:1; // la rana si sposta sul tronco
                    rana.x -= 1;
                }
                break;
            case KEY_RIGHT:
                if(rana.x < maxx - 1){
                    rana.isOnLog?relPos+=1:1; // la rana si sposta sul tronco
                    rana.x += 1;
                }
                break;
        }
        // fprintf(fp,"offset in pasto alla pipe %d \n",rana.offsetLogOccupied);
        // fflush(fp);
        rana.offsetLogOccupied=relPos;
        write(p[1], &rana, sizeof(elemento));
        rana.sparato = false;
        rana.idxLogOccupied=-1;
        rana.offsetLogOccupied=-1;
        rana.isOnLog=false;
        // la rana non può andare sotto il marciapiede
        rana.y>offsetMarciapiede?rana.y=offsetMarciapiede:1;
        // nel caso in cui la rana subisca una collisione
        ////mvprintw(1,1    , "coll detection %d",collisionDetection);
    }
    return;
}

void bullet(int p[]){
    elemento proiettile,data;
    while(true){
        read(p[0], &data, sizeof(elemento));
        if (data.sparato == true && data.c == 20){
            proiettile.c = 21;
            proiettile.y=data.y;
            proiettile.x=data.x;
            while(proiettile.y>0){
                    proiettile.y -= 1;
                    write(p[1], &proiettile, sizeof(elemento));
                    usleep(30000);
            }
        }
        else{
            write(p[1],&data,sizeof(elemento));
        }
    }
}
void frogIsOnLog(int p3[], int p5[],  elemento rana, elemento logs[]){
    int relPos=0;
    int idxLog=0;
    int previdx=0;
    
    for (int i = 0; i < NUMLOGS; i++){
        // TO DO
        //  [+] se collisione e logs[i].enemy==false allora decrementa vita!
        if (rana.y == logs[i].y && rana.x < logs[i].x+6 && rana.x >= logs[i].x && logs[i].enemy==false){ // la rana sale se collisione e se non c'è un enemy 
            logs[i].logOccupied = true;
            if (rana.isOnLog==false){
                relPos = (rana.x-logs[i].x);
                idxLog=i;     
                write(p3[1],&relPos,sizeof(int));
                write(p3[1],&idxLog,sizeof(int));
            }
            else if(rana.idxLogOccupied!=i){ // la rana è sul tronco ma cambia tronco!
                relPos = (rana.x-logs[i].x);
                idxLog=i;     
                write(p3[1],&relPos,sizeof(int));
                write(p3[1],&idxLog,sizeof(int));
            }
            write(p5[1], &logs[i], sizeof(elemento));
        }
    }
}
