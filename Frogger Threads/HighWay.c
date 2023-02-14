#include "HighWay.h"

void* car(void*id){
    srand(id);
    int identifier = *((int *) id); // identificativo rana
    macchine[identifier].c = identifier;
    int counter = 0;
    int possibleStartY[CORSIE] = {0}; // possibili corsie in cui spawnare
    // bandiera = esco dal while nel caos in cui non ci siano collisioni
    //            Lo riuso per aspettare che tutte le macchine siano uscite dal while di controllo
    // flag = condizione del while
    bool bandiera = true, flag = true;

    // inizializzazione array corsie
    for(int i=0;i<NUMACCHINE;i++){
        if (i%2!=0 && i != 0){ 
            pthread_mutex_lock(&mutex);
            possibleStartY[counter]=i;
            counter++;
            pthread_mutex_unlock(&mutex);
        }
    }
    // inizializzazione tipologia del veicolo
    if (1+rand()%DADI > DADI-PARI){
        pthread_mutex_lock(&mutex);
        macchine[identifier].type=0; //macchina
        pthread_mutex_unlock(&mutex);
    }
    else{
        pthread_mutex_lock(&mutex);
        macchine[identifier].type=1;
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    macchine[identifier].y=possibleStartY[(rand()%5)]+offsetAutostrada;
    macchine[identifier].x=(1+rand()%maxX);
    pthread_mutex_unlock(&mutex);

    while(flag){ // per ogni macchina scorriamo le altre macchine per controllare eventuali collisioni in generazione
        bandiera = true;
        for (int i = 0; i<NUMACCHINE; i++){
            for (int j = 0; j<NUMACCHINE; j++){
                if (macchine[i].y == macchine[j].y && i != j){
                    if (!(macchine[j].x > macchine[i].x+7 || macchine[j].x+7 < macchine[i].x))
                    { // nel caso in cui viene trovata una collisione allora tutte la macchina appartenente alla funzione che ha trovato questa collisione verrà rinizializzata
                        pthread_mutex_lock(&mutex);
                        macchine[identifier].y=possibleStartY[(rand()%5)]+offsetAutostrada;
                        macchine[identifier].x=(1+rand()%maxX);
                        pthread_mutex_unlock(&mutex);
                        bandiera = false;
                        
                    }
                }
            }
        }
        // nel caso in non siano state trovate collisioni tra le macchine la bandiera verrà settata a false così uscendo dal while e procedendo con la stampa
        if (bandiera){
            flag = false;
        }
    }

    pthread_mutex_lock(&mutex);
    macchine[identifier].generatoCorrettamente = true;
    pthread_mutex_unlock(&mutex);
    
    // aspettiamo che tutte le macchine escano dal while di controllo 
    bandiera = true;
    while(bandiera){
        if (macchineGenerateCorrettamente == true){
            bandiera = false;
        }
    }

    while(gioca){
        flag = true;
        if (macchine[identifier].x < 0 || macchine[identifier].x > maxX || macchine[identifier].y < 0 || macchine[identifier].y >= maxY){ // verifico se c'è stata una collisione con i bordi dell'area di gioco 
            // c'è stata una collisione 
            while(flag){
                pthread_mutex_lock(&mutex);
                // genero casualmente una nuova corsia
                macchine[identifier].y=possibleStartY[rand()%NUMTRONCHI]+offsetAutostrada;
                if(macchine[identifier].y == 3+offsetAutostrada || macchine[identifier].y == 7+offsetAutostrada){ //se la macchina sta nella corsia 7 o 3 il dovrà spownare a ridosso del bordo destro dello schermo 
                    macchine[identifier].x = maxX;
                }
                else{
                    macchine[identifier].x = 0;
                }
                // con questo ciclo controllo se la nuova posizione assegnata alla macchina, successivamente al cambio corsia è o non è libera da possibili collisioni con altre macchine
                for (int i = 0; i < NUMACCHINE; i++){
                    //se c'è una collisione, rimango dentro il while altrimenti la nuova posizione è ok e usciamo dal while impostando flag=false
                    if(macchine[identifier].y == macchine[i].y && identifier != i && !(macchine[identifier].x+10 < macchine[i].x || macchine[identifier].x > macchine[i].x+10)){
                        flag = true;
                        break;
                    }
                    else{
                        flag = false;
                    }
                }
                pthread_mutex_unlock(&mutex);
            }
        }
        else{ // se non c'è stata alcuna collsione, allora la macchina si muove linearmente sino a che non incontra nuovamente un bordo
            pthread_mutex_lock(&mutex);
            if (macchine[identifier].y == 3+offsetAutostrada || macchine[identifier].y == 7+offsetAutostrada){// se la macchina si trova nella corsia 3 o 7 il movimento sarà da destra verso sinistra
                macchine[identifier].x -= 1;
            }
            else{
                macchine[identifier].x += 1;
            }
            pthread_mutex_unlock(&mutex);
                    
            usleep(speedVehicles);
        }
    }
    pthread_exit(0);
}