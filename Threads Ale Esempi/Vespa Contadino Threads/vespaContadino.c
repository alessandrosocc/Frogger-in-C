#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <pthread.h>

#define MAXX 202 /* Numero di righe dello schermo */ 
#define MAXY 63 /* Numero di colonne dello schermo */
#define true 1

typedef struct {
    int x; 
    int y;
    char c;
    int vite;
}posizione;

int nVite=3, xPosVite=1, iterazione=1,iterMax=50,xTrap1,yTrap1,xTrap2,yTrap2,xTrap3,yTrap3,nTrappole=3;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
posizione vespa,contadino;
//proto
void* fContadino(void*);
void* fVespa(void*);
void* fTrappole(void*);
void AreaGioco();



int main() { 
    srand(time(NULL));
    initscr();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);

    getmaxyx(stdscr,vespa.y,vespa.x);
    vespa.x=vespa.x/2;
    vespa.y=vespa.y/2;
    vespa.c='^';

    getmaxyx(stdscr,contadino.y,contadino.x);
    contadino.c='#';
    contadino.y=contadino.y/2-5;
    contadino.x=contadino.x/2-5; 

    xTrap1=(rand()%MAXY-1)+1;
    yTrap1=(rand()%MAXY-1)+1;
    xTrap2=(rand()%MAXY-1)+1;
    yTrap2=(rand()%MAXY-1)+1;
    xTrap3=(rand()%MAXY-1)+1;
    yTrap3=(rand()%MAXY-1)+1;

    pthread_t vespaT,contadinoT,trappolaT;
    pthread_create(&vespaT,NULL,&fVespa,NULL);
    pthread_create(&contadinoT,NULL,&fContadino,NULL);
    pthread_create(&trappolaT,NULL,&fTrappole,NULL);

    AreaGioco();
    pthread_join(vespaT,NULL);
    pthread_join(contadinoT,NULL);
    pthread_join(trappolaT,NULL);
    endwin();
    return 0;
}
void* fTrappole(void* params){
    while(true){
        pthread_mutex_lock(&mutex);
        if (iterazione==iterMax){
            iterazione=1;
            yTrap1=(rand()%MAXY-1)+1;
            xTrap1=(rand()%MAXX-1)+1;
            yTrap2=(rand()%MAXY-1)+1;
            xTrap2=(rand()%MAXX-1)+1;
            yTrap3=(rand()%MAXY-1)+1;
            xTrap3=(rand()%MAXX-1)+1;
        }
        else{
            iterazione++;
        }
        pthread_mutex_unlock(&mutex);
    }
}
void* fVespa(void* param){
    //close(p[0]);
    int arr[]={-1,0,1};
    int x=0,y=0;//direzione

    x=arr[rand()%3];
    y=arr[rand()%3];
    int count=rand()%5;
    while (true){
        pthread_mutex_lock(&mutex);
        if (count<=5){
            vespa.x+=x;
            vespa.y+=y;
            count+=1;
        }
        else{
            x=arr[rand()%3];
            y=arr[rand()%3];
            if (vespa.x+1>=MAXX || vespa.x<0){
                x*=-1;
            }
            else{
                vespa.x+=x;
            }
            if (vespa.x+1>=MAXX || vespa.x<0){
                y*=-1;
            }
            else{
                vespa.y+=y;
            }
            count=0;
        }
        pthread_mutex_unlock(&mutex);
        usleep(300000); //introduce latenza di 1 secondo
        
	}
}


void* fContadino(void* param){
    
    while(true){
        int c = getch();
        pthread_mutex_lock(&mutex);
        switch(c) {
            case KEY_UP: 
                if(contadino.y > 0)
                    contadino.y -= 1; 
                    break;
            case KEY_DOWN:
                if(contadino.y < MAXY - 1)
                    contadino.y += 1; 
                    break;
            case KEY_LEFT: 
                if(contadino.x > 0)
                    contadino.x -= 1; 
                break;
            case KEY_RIGHT:
                if(contadino.x < MAXX - 1)
                    contadino.x += 1; 
                break;
        }
        pthread_mutex_unlock(&mutex);
    }
}



void AreaGioco(){    
    box(stdscr,0,0);
    refresh();
    
    while(true){
        erase();
        //mostra Vite
        for (int i=0;i<nVite;i++){
            mvaddch(1,xPosVite+=2,'.');
        }
        xPosVite=0;
        mvprintw(10,1,"posvite: %d",xPosVite);
        mvaddch(yTrap1,xTrap1,'x');
        mvaddch(yTrap2,xTrap2,'x');
        mvaddch(yTrap3,xTrap3,'x');

        mvaddch(vespa.y, vespa.x, '^'); 
        mvaddch(contadino.y, contadino.x, '#');

        mvprintw(2,1,"vespa %d %d",vespa.y,vespa.x);
        mvprintw(3,1,"contadino %d %d",contadino.y,contadino.x);
        mvprintw(4,1,"vite %d",nVite);
        mvprintw(7,1,"trap1 y:%d x:%d",yTrap1,xTrap1);
        mvprintw(8,1,"trap2 y:%d x:%d",yTrap2,xTrap3);
        mvprintw(9,1,"trap3 y:%d x:%d",yTrap3,xTrap3);

        if(vespa.x==contadino.x && vespa.y==contadino.y){
            if (nVite>0){
                nVite-=1;
                //sleep(5);
                //mostraVite
                // for (int i=0;i<nVite;i++){
                //     mvaddch(1,xPosVite+=2,'.');
                // }
                // refresh();
            }
            else if(nVite==0){
                clear();
                refresh();
                mvprintw(MAXY/2,MAXX/2,"GAME OVER");
                mvprintw(4,1,"vite %d",nVite);
                refresh();
                sleep(5);
                break;
            }
        } 
        else if((vespa.x == xTrap1 && vespa.y==yTrap1)||(vespa.x == xTrap2 && vespa.y==yTrap2)||(vespa.x == xTrap3 && vespa.y==yTrap3)){
            if(nVite<6){
                nVite++;
                iterazione=iterMax;
            }
            else{
                clear();
                refresh();
                mvprintw(MAXY/2,MAXX/2,"WIN");
                refresh();
                sleep(5);
                break;
            }
            
        } 
        usleep(100000); //SE TROPPO PICCOLO ALLORA IL CONTADINO NON FA IN TEMPO A SPOSTARSI CHE LE VITE DEL CONTADINO VANNO A ZERO PERCHè LO VEDE COME SE FOSSE SEMPRE SOPRA
        box(stdscr,0,0);
        refresh();     
    }
}
