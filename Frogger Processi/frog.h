#include "HighWay.h"

#define VELOCITAPROIETTILE 20000

extern void windowGeneration();
extern int offsetMarciapiede;
extern FILE* fp;
void ffrog(int[], int[], int[], int[]);
void bulletGeneration(int[],int[], elemento rana);
void bulletMovement(int[], int[], elemento rana);
void printFrog(int[]);

extern bool gioca;