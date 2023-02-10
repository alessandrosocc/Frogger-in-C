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
#include "HighWay.h"
time_t t;
FILE* fp; 

extern int offsetFiume;
extern pthread_mutex_t mutex;
extern int maxX;
extern int maxY;
elemento tronchi[NUMTRONCHI];
elemento tronchiProiettili[NUMTRONCHI];

void generaProiettile();
void* log(void*);
void* logBullets(void*);