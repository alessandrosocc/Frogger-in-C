typedef struct{
    int x;
    int y;
    char c;
}elemento;


//global variables
#define CORSIE 5
#define DIMCORSIE 2

//prot
bool ControlloCollisione(elemento, WINDOW*);
void car1(int descriptor[],WINDOW*);
void car2(int descriptor[],WINDOW*);
void car3(int descriptor[],WINDOW*);
void car4(int descriptor[],WINDOW*);
void car5(int descriptor[],WINDOW*);