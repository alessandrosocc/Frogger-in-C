#include <ncurses.h>
#include <pthread.h> 
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
#include <time.h>
#include <string.h>
#define NUMTRONCHI 5
// definizione strutture per variabili globali
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
    bool generatoCorrettamente;
    bool macchinaFineRiga;
    bool killed;
    bool sparato2;
}elemento;
time_t t;
FILE* fp; 
#define NUMACCHINE 10
#define CORSIE 5
#define DADI 6
#define PARI 2
extern int maxX;
extern int maxY;
extern pthread_mutex_t mutex;
extern int offsetAutostrada;
extern bool macchineGenerateCorrettamente;
extern bool gioca;
elemento macchine[NUMACCHINE];
elemento tronchi[NUMTRONCHI];
elemento tronchiProiettili[NUMTRONCHI];
extern int speedVehicles;
void* car(void*);
void printMacchine();
