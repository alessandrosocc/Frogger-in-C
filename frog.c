#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <signal.h> //per kill
#include <stdbool.h>


typedef struct{
    int x;
    int y;
    int c;
    bool sparato;
}elemento;

void ffrog(int[]);
void bullet(int[]);
void padre(int[]);


int main(){
    elemento data;
    int p[2];
    pipe(p);
    initscr();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);
    pid_t frog=fork();
    if (frog < 0){
        _exit(3);
    }
    if(frog == 0){
        ffrog(p);
    }
    else{
        pid_t proiettile = fork();
        if(proiettile == 0){
            bullet(p);
        }
        else{
            padre(p);
        }
    }
    endwin();
}


void ffrog(int p[]){
    elemento rana;
    close(p[0]);
    int maxx=0,maxy=0;
    getmaxyx(stdscr,maxy,maxx);
    rana.c= 1;
    rana.y=maxy/2;
    rana.x=maxx/2; 
    rana.sparato=false;
    //getmaxyx(stdscr,oggetto.y,oggetto.x);
    while(true){
        int c = getch();
        switch(c) {
            case KEY_ENTER:
                rana.sparato=true;
                break;
            case KEY_UP: 
                if(rana.y > 0)
                    rana.y -= 1; 
                    break;
            case KEY_DOWN:
                if(rana.y < maxy - 1)
                    rana.y += 1; 
                    break;
            case KEY_LEFT: 
                if(rana.x > 0)
                    rana.x -= 1; 
                break;
            case KEY_RIGHT:
                if(rana.x < maxx - 1)
                    rana.x += 1;
                break;
            

        }
        write(p[1],&rana, sizeof(elemento));
    }
    return;
}

void padre(int p[]){
    close(p[1]);
    elemento data, animale, bull;

    while(true){
        clear();
        read(p[0], &data,sizeof(elemento));
        if (data.c == 1){
            animale.x = data.x;
            animale.y = data.y;
            if (data.sparato==true){
                bull.sparato=true;
            }
        }
        else if (data.c == 2){
            bull.x = data.x;
            bull.y = data.y;
        }
        // stampa
        mvprintw(animale.y,animale.x,"\\/");
        mvprintw(animale.y+1,animale.x,"/\\");
        if (bull.sparato == true){
            mvprintw(bull.y, bull.x, "*");
        }
        //box(stdscr, 0, 0);
        refresh();
        
    }
    sleep(5);
    return;
}
void bullet(int p[]){
    elemento proiettile,data;
    while(true){
        read(p[0], &data, sizeof(elemento));
        if (data.sparato == true){
            proiettile.c = 2;
            proiettile.y=data.y;
            proiettile.x=data.x;
            while(true){
                proiettile.y -= 1;
                write(p[1], &proiettile, sizeof(elemento));
                sleep(1);
            }
        }
        else{
            write(p[1],&data,sizeof(elemento));
        }
    }
    
}