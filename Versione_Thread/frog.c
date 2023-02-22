#include "frog.h"

void* bullet(){
    while(gioca){ // condizione per continuare l'esecuzione del thread
        if (ranaProiettile.sparato == true){ // se la rana spara il proiettile premento la space bar allora la posizione dell'oggetto proiettile vienee messa uguale a quella della rana e inizia a spostarsi verso l'alto sino a che non arriva al limite delle tane, oppure sino a che non incontra un ostacolo
            pthread_mutex_lock(&mutex);
            ranaProiettile.x = rana.x;
            ranaProiettile.y = rana.y;
            pthread_mutex_unlock(&mutex);
        
            while(ranaProiettile.y > offsetEndTane){
                pthread_mutex_lock(&mutex);
                ranaProiettile.y--; // continua a spostare il proiettile verso l'alto
                pthread_mutex_unlock(&mutex);
                usleep(4000);
            }
            pthread_mutex_lock(&mutex);
            ranaProiettile.sparato = false; // dopo essere arrivato al limite, il proiettile si mette a false e aspetta di essere sparato nuovamente dalla rana
            ranaProiettile.sparato2=false;
            pthread_mutex_unlock(&mutex);
        }
        else if (ranaProiettile.sparato2 == true){ // se la rana spara il proiettile premento la space bar allora la posizione dell'oggetto proiettile vienee messa uguale a quella della rana e inizia a spostarsi verso l'alto sino a che non arriva al limite delle tane, oppure sino a che non incontra un ostacolo
            pthread_mutex_lock(&mutex);
            ranaProiettile.x = rana.x;
            ranaProiettile.y = rana.y;
            pthread_mutex_unlock(&mutex);
        
            while(ranaProiettile.y > offsetEndTane){
                pthread_mutex_lock(&mutex);
                ranaProiettile.y--; // continua a spostare il proiettile verso l'alto
                pthread_mutex_unlock(&mutex);
                usleep(4000);
            }
            pthread_mutex_lock(&mutex);
            ranaProiettile.sparato = false; // dopo essere arrivato al limite, il proiettile si mette a false e aspetta di essere sparato nuovamente dalla rana
            ranaProiettile.sparato2=false;
            pthread_mutex_unlock(&mutex);
        }
    }
    
}