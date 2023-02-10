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
    tronchi[identifier].enemy = false;
    pthread_mutex_unlock(&mutex);

    while(true){
        if (counter == enemyLimit && tronchi[identifier].enemy == false && tronchi[identifier].killed == false){
            pthread_mutex_lock(&mutex);
            tronchi[identifier].enemy = true;
            pthread_mutex_unlock(&mutex);
            counter = 0;
        }
        if (tronchi[identifier].enemy){
            if (counter == bulletLimit){
                pthread_mutex_lock(&mutex);
                tronchiProiettili[identifier].sparato = true;
                pthread_mutex_unlock(&mutex);
                counter = 0;
            }
        }
        if (tronchi[identifier].killed){
            pthread_mutex_lock(&mutex);
            tronchi[identifier].killed = false;
            pthread_mutex_unlock(&mutex);
            counter = 0;
        }
        pthread_mutex_lock(&mutex);
        x = tronchi[identifier].x + direzione;
        if(x > maxX-8 || x < 0){
            direzione *= -1;
            //woody.y = 1 + rand()%(maxY-1);
        }
        else{
            tronchi[identifier].x += direzione;            
        }
        pthread_mutex_unlock(&mutex);
        usleep(30000);
        counter ++;
        fprintf(fp, "sono bloccato dentro i tronchi\n");
    }
}

void* logBullets(void*id){
    int identifier = *((int *)id);
    
    while(true){
        if (tronchiProiettili[identifier].sparato){

            tronchiProiettili[identifier].x = tronchi[identifier].x+3;
            tronchiProiettili[identifier].y = tronchi[identifier].y;

            while(tronchiProiettili[identifier].y < maxY-8){
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
        fprintf(fp,"sono bloccato qui dentro\n");
    }
    
}

