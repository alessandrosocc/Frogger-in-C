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
#include <signal.h> //per killª
#include <time.h>
#include <string.h>
#include "HighWay.h"
#define NTANE 5
time_t t;
FILE* fp; 
extern bool gioca;
extern pthread_mutex_t mutex;
extern int maxX;
extern int maxY;
extern int offsetMarciapiede;

extern elemento rana;
extern elemento ranaProiettile;

void* ffrog();
void* bullet();
