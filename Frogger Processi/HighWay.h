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
extern int offsetAutostrada;

//global variables
#define CORSIE 5
#define DIMCORSIE 2
#define MACCHINE 2
#define NUMMACCHINE 10
#define ATTESACAMBIOCORSIA 25000
//#define DELAYM 30000 


bool ControlloCollisione(elemento);
void funzioneMacchina(int p1[], int p2[], int p3[],int stopGame[],int speedVehicles, int id);
//bool carCollision(elemento, int p[]);


extern bool gioca;

