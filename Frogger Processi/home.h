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
#define PUNTEGGIOVITE 1
void areaDiGioco(int p1[], int p2[], int p3[],int p4[], int p5[], int p6[], int p7[], int p8[], int p9[],int pipeTempo[],int[],int[]);
void controlloGenerazioneMacchine(int p1[], int p2[], int p7[]);
void windowGeneration();
void initScreen(int*, int*);
void mostraVita(int);

// global variables
int offsetAutostrada = 0;
int offsetFiume=0;
int offsetTempo=0;
int offsetMarciapiede=0;
int idMacchine = 0;
int vite=0;
int maxX=0, maxY=0;
int addPoints=0;
int taneChiuse[NTANE]={0};
int offsetEndTane=0;
int iterazione=0;
int iterazioneMoment=0;
int offsetTane=0;
int offsetPunteggio=0;
int offsetPrato=0;
int offsetFinale=0;
int count=0;
int lvlvite=0;
bool gioca=true;
void proiettiliKillRana(elemento, elemento[], int[], int *,int*,int[]);
void stampaTronchiNemici(elemento[],elemento[], elemento);
void getTronchiBullets(elemento*,elemento[],elemento[]);
void stampaRanaBullets(elemento,elemento*);
void stampaMacchinaCamion(elemento[]);
void getDataFromPipe(int[],elemento*,elemento[],elemento*, elemento*);
void collisionRanaVehicles(int, int[],int[], int*,elemento*,elemento[],int*,int[]);
void processGeneration(int[],int[],int[],int[],int[],int[],int[],int[], int[],int[],int[],int[],int,int);
void ranaKillEnemy(elemento,elemento*, elemento [], int [], int*);
void collisioneProiettiliMacchine(elemento *, elemento [], elemento [], int []);
void collisioneProiettileRanaProiettiliNemici(elemento *, elemento[], int[]);
void mostraPunteggio(int);
void chiudiTana(int);
void tempo(int[],int[],int[]);
void riprova(int*,int[]);
void displayTime(int);
void fineMancheThxTime(int[],int[],int,bool,int*,int,int,int[]);
void checkTaneOccupate(int taneChiuse[],int* totaleTaneChiusePtr,int*);
void checkRanaInTana(int* frogCollisionPtr,elemento rana,int restartTime[],int timeRestart,int* punteggioPtr, int p4[],int[]);
void displayTanaChiusa(int taneChiuse[]);
void checkRanaFiume(int,int[],int vecchioWoody[],elemento woody[],int* punteggioPtr, elemento* ranaPtr, int* frogCollisionChangeLogScore, int* ranaSuLTroncoPtr, int p5[],elemento* vecchiaRanaPtr,int p9[],int[]);
void playStartingGame();
void playOpenGame();
void playKilled();
void playWinner();
void playEndGame();
void playProiettile();