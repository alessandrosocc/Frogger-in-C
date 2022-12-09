#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
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

//proto
void fGuardia(int[]);
void fLadro(int[]);
void fControllo(int[]);

int main() { 
    srand(time(0));
    initscr();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);
    /* Inizializza lo schermo */
    /* Disabilita la visualizzazione dei tasti premuti */
    /* Abilita la gestione dei tasti freccia */
    /* Nasconde il cursore */
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    
    int p[2];
    pipe(p);
    if(pipe(p)==-1) {
       perror("pipe call");
       _exit(1);
	}

    pid_t ladro=fork();
    if (ladro==-1){ //check prima di consegnare
        printw("errore ladro!\n");
        _exit(1);
    }
    else if (ladro==0){
        fLadro(p);
    }
    else{ //padre
        pid_t guardia=fork();
        if (guardia==-1){
            printw("errore guardia!\n");
            _exit(1);
        }
        else if(guardia==0){
            fGuardia(p);
        }
        else{
            fControllo(p);
        }
        wait(0);
    }
    wait(0);
    endwin();
    return 0;
}
void fLadro(int p[]){
    close(p[0]);
    int arr[]={-1,0,1};
    int x=0,y=0;//direzione
    posizione oggetto;
    getmaxyx(stdscr,oggetto.y,oggetto.x);
    oggetto.x=oggetto.x/2;
    oggetto.y=oggetto.y/2;
    #ifdef DEBUG
    oggetto.x=10;
    oggetto.y=10;
    #endif
    oggetto.c='$';
    while (true){
        x=arr[rand()%3];
        y=arr[rand()%3];
        if (oggetto.x+1>=MAXX || oggetto.x<0){
            x*=-1;
        }
        else{
            oggetto.x+=x;
        }
        if (oggetto.x+1>=MAXX || oggetto.x<0){
            y*=-1;
        }
        else{
            oggetto.y+=y;
        }
        

        write(p[1], &oggetto, sizeof(posizione));
        usleep(300000); //introduce latenza di 1 secondo
	}
    return;
}


void fGuardia(int p[]){
    close(p[0]);
    posizione oggetto;
    oggetto.c='#';
    oggetto.y=0;
    oggetto.x=0; 
    //getmaxyx(stdscr,oggetto.y,oggetto.x);
    while(true){
        int c = getch();
        switch(c) {
            case KEY_UP: 
                if(oggetto.y > 0)
                    oggetto.y -= 1; 
                    break;
            case KEY_DOWN:
                if(oggetto.y < MAXY - 1)
                    oggetto.y += 1; 
                    break;
            case KEY_LEFT: 
                if(oggetto.x > 0)
                    oggetto.x -= 1; 
                break;
            case KEY_RIGHT:
                if(oggetto.x < MAXX - 1)
                    oggetto.x += 1; 
                break;

        }

        write(p[1],&oggetto, sizeof(posizione));
    }
    return;
}
void fControllo(int p[]){
    close(p[1]);
    posizione data,curr_ladro, curr_guardia;

    box(stdscr,0,0);
    while(true){
        clear();
        read(p[0], &data, sizeof(posizione));
        if (data.c=='#'){ //guardia
            curr_guardia.y=data.y;
            curr_guardia.x=data.x;
        }
        else{
            curr_ladro.y=data.y;
            curr_ladro.x=data.x;
            
        }
        
        mvaddch(curr_ladro.y, curr_ladro.x, '$'); 
        mvaddch(curr_guardia.y, curr_guardia.x, '#');
        if(curr_ladro.x==curr_guardia.x && curr_ladro.y==curr_guardia.y){
            clear();
            refresh();
            mvprintw(MAXY/2,MAXX/2,"Ladro trovato!");
            refresh();
            sleep(5);
            break;
        } 
        usleep(1000);
        box(stdscr,0,0);

        refresh();
        
    }
    return;
}