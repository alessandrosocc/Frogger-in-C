#include "HighWay.h"

void* car(void*id){
    srand(id);
    int identifier = *((int *) id);
    macchine[identifier].c = identifier;
    int counter = 0;
    int possibleStartY[CORSIE] = {0};
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
    if (1+rand()%6 > 4){
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

    while(flag){
        bandiera = true;
        for (int i = 0; i<NUMACCHINE; i++){
            for (int j = 0; j<NUMACCHINE; j++){
                if (macchine[i].y == macchine[j].y && i != j){
                    if (!(macchine[j].x > macchine[i].x+7 || macchine[j].x+7 < macchine[i].x))
                    {
                        //fprintf(fp, "collisione\n");
                        pthread_mutex_lock(&mutex);
                        macchine[identifier].y=possibleStartY[(rand()%5)]+offsetAutostrada;
                        macchine[identifier].x=(1+rand()%maxX);
                        pthread_mutex_unlock(&mutex);
                        bandiera = false;
                        
                    }
                }
            }
        }
        if (bandiera){
            flag = false;
            // pthread_mutex_lock(&mutex);
            // macchine[identifier].generatoCorrettamente = true;
            // pthread_mutex_unlock(&mutex);
        }
    }

    pthread_mutex_lock(&mutex);
    macchine[identifier].generatoCorrettamente = true;
    pthread_mutex_unlock(&mutex);
    fprintf(fp, "identifier %d\n", identifier); 
    
    bandiera = true;
    while(bandiera){
        if (macchineGenerateCorrettamente == true){
            bandiera = false;
        }
    }

    while(true){
        flag = true;
        if (macchine[identifier].x < 0 || macchine[identifier].x > maxX || macchine[identifier].y < 0 || macchine[identifier].y >= maxY){ // verifico se c'è stata una collisione con i bordi dell'area di gioco 
            
            while(flag){
                
                pthread_mutex_lock(&mutex);
                macchine[identifier].y=possibleStartY[rand()%5]+offsetAutostrada;
                if(macchine[identifier].y == 3+offsetAutostrada || macchine[identifier].y == 7+offsetAutostrada){ //se la macchina sta nella corsia 29 o 35 il dovrà spownare a ridosso del bordo destro dello schermo 
                    macchine[identifier].x = maxX;
                }
                else{
                    macchine[identifier].x = 0;
                }

                for (int i = 0; i < NUMACCHINE; i++){
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

        else{
            pthread_mutex_lock(&mutex);
            if (macchine[identifier].y == 3+offsetAutostrada || macchine[identifier].y == 7+offsetAutostrada){// se la macchina sta nella corsia 29 o 35 il movimento sarà da destra verso sinistra
                macchine[identifier].x -= 1;
            }
            else{
                macchine[identifier].x += 1;
            }
            pthread_mutex_unlock(&mutex);
                    
            usleep(50000);
        }
    }
}