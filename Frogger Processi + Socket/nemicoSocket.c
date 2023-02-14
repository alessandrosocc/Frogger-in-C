#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close

typedef struct{
    int x;
    int y;
    int c; //0-9 identificativo macchina ; 20 = rana ; 21 = proiettile ; 30-35 legnetto ; 40 - 45 proiettili nemici su tronco
    int type; //camion o auto?
    int offsetLogOccupied;
    int idxLogOccupied;
    int collision;
    bool sparato;
    bool inVolo;
    bool enemy; //il tronco ha un nemico sopra? gestito in river.c
    bool isOnLog;
    bool logOccupied;
    bool cambioMovimento;

}elemento;

void ffrog(int[], int[], int[]); 
void bulletGeneration(int[], int[], elemento);
void proiettileRana(int [], int[], elemento);

int main (void){
    initscr(); 
    curs_set(0);
    noecho();
    keypad(stdscr,1);

    elemento data;
    int maxX = 0, maxY = 0;
    getmaxyx(stdscr,maxY, maxX);
    
    int server_socket; 
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

     // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9003);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind our socket to a specified IP and port
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 10);

    int client_socket; 
    client_socket = accept(server_socket, NULL, NULL);
    int flags = fcntl(server_socket, F_GETFL, 0);
    fcntl(server_socket, F_SETFL, flags & (~O_NONBLOCK));

    int p1[2];
    int p2[2];
    int p3[2];
    pipe(p1);
    pipe(p2);
    pipe(p3);

    fcntl(p2[0], F_SETFL, O_NONBLOCK);
    fcntl(p3[0], F_SETFL, O_NONBLOCK);

    pid_t frog = fork();
    if (frog == 0){
        //chiamo la funzione rana
        ffrog(p1,p2,p3);
    }
    else{
        
        // funzione padre
        while(true){
            read(p1[0], &data,sizeof(elemento));
            send(client_socket, &data, sizeof(elemento), 0);
        }
    
    }
    close(server_socket);




    return 0; 
}

void ffrog(int p1[], int p2[], int p3[]){
    elemento rana;
    int maxX = 0, maxY = 0;
    getmaxyx(stdscr, maxY, maxX);
    rana.c = 10;
    rana.x = maxX/2;
    rana.y = maxY/2;
    rana.sparato = false;
    while(true){
        timeout(1);
        int c = getch();
        switch(c) {
            case 32: //barra spaziatrice
                rana.sparato=true;
                // se premiamo la barra spaziatrice allora viene sparato un proiettile
                bulletGeneration(p1,p2,rana);
                break;
            case KEY_UP:
                if(rana.y > 0)
                    rana.y -= 2;
                    break;
            case KEY_DOWN:
                if(rana.y < maxY - 1)
                    rana.y += 2;
                    break;
            case KEY_LEFT:
                if(rana.x > 0){
                rana.x -= 1;
                }
                break;
            case KEY_RIGHT:
                if(rana.x < maxX - 1){
                rana.x += 1;
                }
                break;
        }
        write(p1[1], &rana, sizeof(elemento));
        rana.sparato = false;
    }
}

void bulletGeneration(int p1[], int p2[], elemento rana){
    pid_t proiettile = fork();
    if (proiettile == 0){
        proiettileRana(p1,p2,rana);
    }
    wait(NULL);
}


void proiettileRana(int p1[], int p2[], elemento rana){
    elemento proiettile;
    proiettile.y = rana.y; 
    proiettile.x = rana.x+2;
    proiettile.c = 20;
    while (proiettile.y<50){
        proiettile.y += 1;
        write(p1[1], &proiettile, sizeof(elemento));
        usleep(10000);
    }

}
