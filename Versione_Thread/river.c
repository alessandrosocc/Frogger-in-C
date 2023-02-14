#include "river.h"


void* log(void*id){
    srand(id);
    int identifier = *((int *) id);
    int direzione = 1, bulletLimit = 50, x = 0,counter = 0, enemyLimit = 0;
    tronchi[identifier].c = identifier;
    enemyLimit = 1+rand()%200;
    pthread_mutex_lock(&mutex);
    tronchi[identifier].x = (1+rand()%maxX-10);
    tronchi[identifier].y = (identifier*2)+offsetFiume-1;
    fprintf(fp, "y tronchi %d\n", tronchi[identifier].y);
    tronchi[identifier].enemy = false;
    pthread_mutex_unlock(&mutex);

    while(gioca){
        // genero un enemy se il counter == enemyLimit ed il tronco non ha un nemico sopra
        if (counter == enemyLimit && tronchi[identifier].enemy == false && tronchi[identifier].killed == false){
            pthread_mutex_lock(&mutex);
            tronchi[identifier].enemy = true;
            pthread_mutex_unlock(&mutex);
            counter = 0;
        }
        // se c'è un tronco un nemico
        if (tronchi[identifier].enemy){
            if (counter == bulletLimit){
                pthread_mutex_lock(&mutex);
                tronchiProiettili[identifier].sparato = true;
                pthread_mutex_unlock(&mutex);
                counter = 0;
            }
        }
        // se il nemico sul tronco è stato ucciso, reimposto .killed a false
        if (tronchi[identifier].killed){
            pthread_mutex_lock(&mutex);
            tronchi[identifier].killed = false;
            pthread_mutex_unlock(&mutex);
            counter = 0;
        }
        pthread_mutex_lock(&mutex);
        x = tronchi[identifier].x + direzione;
        // faccio spostare il tronco
        if(x > maxX-7 || x < 0){
            direzione *= -1;
        }
        else{
            tronchi[identifier].x += direzione;
        }
        // la rana segue il tronco se la rana è sopra il tronco
        if (rana.y == tronchi[identifier].y && rana.x >= tronchi[identifier].x && rana.x+2 <= tronchi[identifier].x+7){
            rana.x += direzione;
        }
        pthread_mutex_unlock(&mutex);
        usleep(speedLegnetto);
        counter ++;
    }
    pthread_exit(0);
}

void* logBullets(void*id){
    int identifier = *((int *)id);
    while(gioca){
        //se il tronco ha un proiettile sparato
        if (tronchiProiettili[identifier].sparato){
            tronchiProiettili[identifier].x = tronchi[identifier].x+3;
            tronchiProiettili[identifier].y = tronchi[identifier].y;
            // il proiettile del tronco continua a spostarsi finchè non arriva all'offset del marciapiede
            while(tronchiProiettili[identifier].y < offsetMarciapiede){
                pthread_mutex_lock(&mutex);
                tronchiProiettili[identifier].y++;
                pthread_mutex_unlock(&mutex);
                usleep(80000);
            }

            pthread_mutex_lock(&mutex);
            tronchiProiettili[identifier].sparato = false;
            pthread_mutex_unlock(&mutex);
        }
        usleep(20000);
    }
}

