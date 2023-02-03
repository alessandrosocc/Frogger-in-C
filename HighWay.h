typedef struct{
    int x;
    int y;
    int c; //0-9 identificativo macchina ; 20 = rana ; 21 = proiettile ; 30-35 legnetto ; 40 - 45 proiettili nemici su tronco
    int type; //camion o auto?
    bool sparato;
    bool enemy; //il tronco ha un nemico sopra? gestito in river.c
}elemento;
extern int offsetAutostrada;

//global variables
#define CORSIE 5
#define DIMCORSIE 2
#define MACCHINE 2
#define DELAY 50000

bool ControlloCollisione(elemento);
void car(int descriptor[], int connection[], int id);
//bool carCollision(elemento, int p[]);


