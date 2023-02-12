#define NUMTRONCHI 5
#define DELAYT 50000
//da togliere quando usi vm
#define true 1
#define false 0
#define TIMERPROIETTILI 100
extern FILE* fp;
void printLog(int[]);
void legnetto(int[],int[],int[],int[],int[], int,int);
void enemyBullet(int[], int[],int [], elemento);
void enemyBulletShoot(int[], int[],int[], elemento);
void printEnemyBullets(int[]);
extern bool gioca;
extern int offsetFiume;
extern int offsetMarciapiede;
