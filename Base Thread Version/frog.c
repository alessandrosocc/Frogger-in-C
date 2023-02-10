#include "frog.h"
void* ffrog(){
    
    rana.c=20;
    rana.x = maxX/2;
    rana.y = offsetMarciapiede;
    while(gioca){
        int c = getch();
        pthread_mutex_lock(&mutex);
        switch(c) {
            case KEY_UP: 
                if(rana.y > 0)
                    rana.y -= 2; 
                    break;
            case KEY_DOWN:
                if(rana.y < maxY - 1)
                    rana.y += 2; 
                    break;
            case KEY_LEFT: 
                if(rana.x> 0)
                    rana.x -= 1; 
                break;
            case KEY_RIGHT:
                if(rana.x < maxX - 1)
                    rana.x += 1; 
                break;
            case 32: // barra spaziatrice
                ranaProiettile.sparato = true;
                break;
                // genero il thread che genererà a sua volta il proiettile
        }
        rana.y>offsetMarciapiede?rana.y=offsetMarciapiede:1;
        pthread_mutex_unlock(&mutex);
        usleep(10000);
    }
}
void* bullet(){
    while(gioca){
        if (ranaProiettile.sparato == true){
            pthread_mutex_lock(&mutex);
            ranaProiettile.x = rana.x;
            ranaProiettile.y = rana.y;
            pthread_mutex_unlock(&mutex);
        
            while(ranaProiettile.y >= 0){
                pthread_mutex_lock(&mutex);
                ranaProiettile.y--;
                pthread_mutex_unlock(&mutex);
                usleep(50000);
            }
            pthread_mutex_lock(&mutex);
            ranaProiettile.sparato = false;
            pthread_mutex_unlock(&mutex);
        }
    }
    
}