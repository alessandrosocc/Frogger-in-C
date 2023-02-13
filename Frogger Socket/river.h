#define NUMTRONCHI 5
#define DELAYT 50000
#define DELAYPROIETTILI 30000
//da togliere quando usi vm
#define true 1
#define false 0
#define TIMERPROIETTILI 200
#define IDTRONCHIOFFSET 30
#define IDTRONCHIPROIETTILIOFFSET 40
#define LIMTIMERNEMICI 1000
#define LUNGHEZZATRONCO 7
extern FILE* fp;
void printLog(int[]);
void legnetto(int[],int[],int[],int[],int[], int,int);
void enemyBullet(int[], int[],int [], elemento);
void enemyBulletShoot(int[], int[],int[], elemento);
void printEnemyBullets(int[]);
extern bool gioca;
extern int offsetFiume;
extern int offsetMarciapiede;
