typedef struct{
    int x;
    int y;
    char c;
}elemento;
//extern WINDOW *vite, *tempo, *marciapiede, *autostrada, *prato, *fiume, *tane, *punteggio; //va a cercare le var in un altro file
extern int offsetAutostrada;
//extern int possibleStartY[] = {1,3,5,7,9};

//global variables
#define CORSIE 5
#define DIMCORSIE 2
#define MACCHINE 2

#define DELAY 10000


bool ControlloCollisione(elemento);
void car(int descriptor[]);
