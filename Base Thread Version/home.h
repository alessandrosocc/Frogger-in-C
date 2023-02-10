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
#include "HighWay.h"


// puntatore del file per il debug
time_t t;
FILE* fp; 

// inizializzazione mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


#define CORRENTI 5
#define DIMCORRENTI 2
#define PUNTEGGIO 3
#define TANE 6
#define FIUME 10
#define PRATO 2
#define AUTOSTRADA 10
#define MARCIAPIEDE 2
#define TEMPO 2
#define VITE 3
#define NTANE 5
#define NUMTRONCHI 5
#define SPACE_BAR 32
#define NUMACCHINE 10
#define NTANE 5
// variabili globali
int offsetAutostrada = 0;
int offsetFiume=0;
int offsetVite=0;
int offsetMarciapiede=0;
int offsetPunteggio=0;
int offsetTane=0;
int offsetPrato=0;
int offsetTempo=0;


int idMacchine = 0;
int vite=5;
int punteggio=0;
int secondiRimanenti=0;
int maxX=0, maxY=0;
bool macchineGenerateCorrettamente = false;
bool cambioRigaLibero = true;
bool flagTime=0;
int timeSignal=0;
int taneChiuse[NTANE]={0};
int totaleTaneChiuse=0;



elemento rana;
elemento ranaProiettile;



// prototipi funzioni
void initScreen();
void windowGeneration();
void areaDiGioco();
void generaProiettile();
void mostraPunteggio();
void mostraVita();
void displayTime();
void ranaCollideConMacchine();
void proiettileRanaCollideConMacchine();
void ranaKillTronchi();
void enemyKillRana();
void printRana();
void printMacchine();
void printTronchi();
void riprova();
void checkTane();
void checkRanaInTana();
void chiudiTana(int);
void checkRanaInFiume();
extern void* ffrog();
extern void* bullet();
extern void* log(void*);
extern void* logBullets(void*);

