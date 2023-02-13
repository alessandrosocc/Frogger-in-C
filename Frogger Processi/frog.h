#include "HighWay.h" // includiamo il file contenente la variabile oggetto
#define VELOCITAPROIETTILE 10000 
extern void windowGeneration();
extern int offsetMarciapiede;
extern FILE* fp;
void ffrog(int[], int[], int[], int[],int[]);
void bulletGeneration(int[],int[], elemento rana);
void bulletMovement(int[], int[], elemento rana);
void printFrog(int[]);
extern bool gioca;
extern int offsetEndTane;