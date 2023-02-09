#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>

#define MAXX 202 /* Numero di righe dello schermo */ 
#define MAXY 63 /* Numero di colonne dello schermo */
#define true 1
typedef struct {
    int x; 
    int y;
    char c;
}posizione;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
posizione ladro, guardia, avvocato;
// posizione ladro={0,0,LADRO};
// posizione guardia={0,0,GUARDIA};
// posizione avvocato={0,0,AVVOCATO};


//proto
void* fGuardia(void*);
void* fLadro(void*);
void fAreaGioco();
void* fAvvocato(void*);

int main() { 
    
    initscr();/* Inizializza lo schermo */
    noecho();/* Disabilita la visualizzazione dei tasti premuti */
    keypad(stdscr, 1);/* Abilita la gestione dei tasti freccia */
    curs_set(0);/* Nasconde il cursore */   
    
    int maxx,maxy;
    getmaxyx(stdscr,maxy,maxx);
    ladro.x=maxx/2-5;
    ladro.y=maxy/2-5;
    ladro.c='$';

    guardia.c='#';
    guardia.x=maxx-2;
    guardia.y=maxy-2;
    
    avvocato.x=maxx/2;
    avvocato.y=maxy/2;
    avvocato.c='A';

    pthread_t guardiaM, avvocatoM, ladroM;
    pthread_create(&guardiaM,NULL, &fGuardia,NULL);
    pthread_create(&avvocatoM,NULL, &fAvvocato,NULL);
    pthread_create(&ladroM,NULL, &fLadro,NULL);
    fAreaGioco();
    pthread_join(guardiaM,NULL);
    pthread_join(avvocatoM,NULL);
    pthread_join(ladroM,NULL);
    endwin();
    return 0;
}
void* fLadro(void* param){
    srand(time(NULL));
    int arr[]={-1,0,1};
    int x=0,y=0;//direzione
    
    while (true){
        pthread_mutex_lock(&mutex);
        x=arr[rand()%3];
        y=arr[rand()%3];
        if (ladro.x+1>=MAXX || ladro.x<0){
            x*=-1;
        }
        else{
            ladro.x+=x;
        }
        if (ladro.x+1>=MAXX || ladro.x<0){
            y*=-1;
        }
        else{
            ladro.y+=y;
        }
        pthread_mutex_unlock(&mutex);

        //write(p[1], &oggetto, sizeof(posizione));
        usleep(300000); //introduce latenza di 1 secondo
	}
}


void* fGuardia(void* param){
    srand(time(NULL));
    while(true){
        int c = getch();
        pthread_mutex_lock(&mutex);
        switch(c) {
            case KEY_UP: 
                if(guardia.y > 0)
                    guardia.y -= 1; 
                    break;
            case KEY_DOWN:
                if(guardia.y < MAXY - 1)
                    guardia.y += 1; 
                    break;
            case KEY_LEFT: 
                if(guardia.x > 0)
                    guardia.x -= 1; 
                break;
            case KEY_RIGHT:
                if(guardia.x < MAXX - 1)
                    guardia.x += 1; 
                break;
        }
        pthread_mutex_unlock(&mutex);
    }
    
}

void* fAvvocato(void* param){
    //close(p[0]);
    int arr[]={0,-1,1};
    int x=0,y=0;//direzione
    //posizione oggetto;
    
    while (true){
        pthread_mutex_lock(&mutex);
        x=arr[rand()%3];
        y=arr[rand()%3];
        if (avvocato.x+1>=MAXX || avvocato.x<0){
            x*=-1;
        }
        else{
            avvocato.x+=x;
        }
        if (avvocato.x+1>=MAXX || avvocato.x<0){
            y*=-1;
        }
        else{
            avvocato.y+=y;
        } 
        pthread_mutex_unlock(&mutex);
        usleep(300000); //introduce latenza di 1 secondo
    }
}

//guardia -> ladro : win
//avvocato -> ladro : lose
void fAreaGioco(){
    box(stdscr,0,0);
    while(true){
        erase();
        mvaddch(ladro.y, ladro.x, '$');
        mvaddch(guardia.y, guardia.x, '#');
        mvaddch(avvocato.y, avvocato.x, 'A');
        // mvprintw(1,1,"ladro : %d %d",ladro.y,ladro.x);
        // mvprintw(2,1,"guardia : %d %d",guardia.y,guardia.x);
        // mvprintw(3,1,"avvocato : %d %d",avvocato.y,avvocato.x);

        if(ladro.x==guardia.x && ladro.y==guardia.y){ //guardia === Ladro -> WIN
            erase();
            refresh();
            // mvprintw(1,1,"ladro : %d %d",ladro.y,ladro.x);
            // mvprintw(2,1,"guardia : %d %d",guardia.y,guardia.x);
            // mvprintw(3,1,"avvocato : %d %d",avvocato.y,avvocato.x);
            mvprintw(MAXY/2,MAXX/2,"WIN");
            refresh();
            sleep(5);
            break;
        }
        else if (avvocato.x==ladro.x && avvocato.y==ladro.y)//avvocato === Ladro -> LOSE
        {
            erase();
            refresh();
            // mvprintw(1,1,"ladro : %d %d",ladro.y,ladro.x);
            // mvprintw(2,1,"guardia : %d %d",guardia.y,guardia.x);
            // mvprintw(3,1,"avvocato : %d %d",avvocato.y,avvocato.x);
            mvprintw(MAXY/2,MAXX/2,"LOSE");
            refresh();
            sleep(5);
            break;
        }

        //usleep(1000);
        //box(stdscr,0,0);
        refresh();
        
    }

}
