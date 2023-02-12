// inclusione dei file esterni
#include "home.h"

int main(){
    time(NULL);
    srand(time(NULL));
    fp = fopen("log.txt", "w");
    initScreen();
    //windowGeneration();
    char* choicesDifficulty[]={"Principiante","Intermedio","Esperto","Impossibile"};
    char* choices[]={"Inizia a Giocare","Credits"};
    char* choicesCredits[]={""};
    pthread_t open,reopen;
    int choice=menu("Frogger 2023","Benvenuto in Frogger, un gioco creato con processi, threads e lacrime",choices,2,true,true);
    pthread_create(&open,NULL,&playOpenGame,NULL);
    pthread_join(open,NULL);
    while(choice){
        if(choice==2){
            pthread_t credits;
            choice=menu("Credits","Alessandro Soccol 60/79/00057, Marco Cosseddu 60/79/00010",choicesCredits,0,false,true);
            pthread_create(&credits,NULL,&playOpenGame,NULL);
            pthread_join(credits,NULL);
        }
        else if(choice==1){
            choice=menu("Difficoltà","Scegli la Difficoltà",choicesDifficulty,4,true,false);
            if(choice){
                switch(choice){
                    case 1: //principiante
                        #define LVLVITE 10 // mi serve quando il giocatore perde e vuole rigiocare, devo reimpostare le vite
                        vite=LVLVITE;
                        #define REMAININGTIME 800000
                        speedVehicles=30000;
                        speedLegnetto=50000;
                        break;
                    case 2: //intermedio
                        #define LVLVITE 5
                        vite=LVLVITE;
                        #define REMAININGTIME 600000
                        speedVehicles=30000;
                        speedLegnetto=50000;
                    case 3: //difficile
                        #define LVLVITE 3
                        vite=LVLVITE;
                        #define REMAININGTIME 200000
                        speedVehicles=10000;
                        speedLegnetto=30000;
                        break;
                    case 4: //impossibile
                        #define LVLVITE 2
                        vite=LVLVITE;
                        #define REMAININGTIME 150000
                        speedVehicles=5000;
                        speedLegnetto=10000;
                        break;
                }
            }
            pthread_t startingGame;
            pthread_create(&startingGame,NULL,&playStartingGame,NULL);
            pthread_join(startingGame,NULL);
            //sleep(4);
            clear();
            refresh();
            windowGeneration(); // genero la window
            // variabili intere
            int carId[NUMACCHINE];
            int logId[NUMTRONCHI];
            int logBulletsId[NUMTRONCHI];
            for (int i = 0;i<NUMACCHINE; i++){
                carId[i] = i;
            }
            for (int i = 0;i<NUMTRONCHI; i++){
                logId[i] = i;
            }
            for (int i = 0; i<NUMTRONCHI; i++){
                logBulletsId[i] = i;
            }

                // id thread
                pthread_t ranaId, ranaProiettileId;
                pthread_t macchineId[NUMACCHINE];
                pthread_t tronchiId[NUMTRONCHI];
                pthread_t proiettiliId[NUMTRONCHI];
                pthread_t tempoThread;
                pthread_t musica;
                // creazione dei thread
                pthread_create(&tempoThread,NULL,&calculateResidualTime,NULL);
                pthread_create(&ranaId, NULL, &ffrog, NULL);
                pthread_create(&ranaProiettileId, NULL, &bullet, NULL);
                for (int i = 0; i<NUMACCHINE; i++){
                    pthread_create(&macchineId[i], NULL, &car,(void*)&carId[i]);
                }
                for (int i = 0; i<NUMTRONCHI; i++){
                    pthread_create(&tronchiId[i], NULL, &log, (void*)&logId[i]);
                }
                for (int i = 0;i<NUMTRONCHI; i++){
                    pthread_create(&proiettiliId[i],NULL,&logBullets, (void*)&logBulletsId[i]);
                }
                areaDiGioco();
                pthread_join(tempoThread,NULL);
                pthread_join(ranaId,NULL);
                pthread_join(ranaProiettileId,NULL);
                
                for (int i = 0;i<NUMACCHINE; i++){
                    pthread_join(macchineId[i],NULL);
                }
                for (int i = 0;i<NUMTRONCHI; i++){
                    pthread_join(tronchiId[i],NULL);
                }
                for (int i = 0;i<NUMTRONCHI; i++){
                    pthread_join(proiettiliId[i],NULL);
                }
                wait(NULL);
                }else{
                    exit(0);
                }
            }
            choice=menu("Frogger 2023","Benvenuto in Frogger, un gioco creato con processi, threads e lacrime",choices,2,true,true);
    }
    endwin();
    fclose(fp);
    return 0;
}
void* playMusic(void* X){
    char command[256];
    while(gioca){
        #ifdef __linux__
        sprintf(command,"aplay %s","../musica/sound.wav");
        system(strcat(command," 1>/dev/null 2>/dev/null"));
        #endif
        #ifdef __APPLE__ || __MACH__
            sprintf(command,"afplay %s","../musica/sound.wav");
            system(command);
        #endif
        
    }
    pthread_exit(0);
}
void* playKilled(void* X){
    char command[256];
    #ifdef __linux__
        sprintf(command,"aplay %s","../musica/killed.wav");
        system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/killed.wav");
        system(command);
    #endif
    pthread_exit(0);
}
void* playProiettile(void* X){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/proiettile.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/proiettile.wav");
        system(command);
    #endif
    
    pthread_exit(0);
}
void* playEndGame(void* X){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/endGame.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/endGame.wav");
        system(command);
    #endif
    
    pthread_exit(0);
    
}
void* playOpenGame(void* X){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/openSong.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));

    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/openSong.wav");
        system(command);

    #endif
    pthread_exit(0);
    
}
void* playWinner(void* X){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/winner.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/winner.wav");
        system(command);
    #endif
    
    pthread_exit(0);
    
}
void* playStartingGame(void* X){
    char command[256];
    #ifdef __linux__
    sprintf(command,"aplay %s","../musica/startingGame.wav");
    system(strcat(command," 1>/dev/null 2>/dev/null"));
    #endif
    #ifdef __APPLE__ || __MACH__
        sprintf(command,"afplay %s","../musica/startingGame.wav");
        system(command);
    #endif
    
    pthread_exit(0);
    
}
void initScreen(){
    initscr(); 
    curs_set(0); // settiamo il cursore per essere invisibile
    noecho();
    keypad(stdscr,1);
    pthread_mutex_lock(&mutex);
    getmaxyx(stdscr,maxY,maxX);    
    pthread_mutex_unlock(&mutex);

}

void windowGeneration(){
    int nCorsie=3, nFiume = 3, yBox = 0, xBox=0, incYBox = 0, corsie = 1, correnti = 1, righe = 1;
    pthread_mutex_lock(&mutex);
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_RED);
    init_pair(3,COLOR_WHITE,COLOR_YELLOW);
    init_pair(4,COLOR_WHITE,COLOR_CYAN);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);//antiglitch
    init_pair(6,COLOR_WHITE, COLOR_MAGENTA);
    init_pair(7,COLOR_WHITE,COLOR_BLUE);
    init_pair(8,COLOR_BLACK,COLOR_RED);
    init_pair(9,COLOR_YELLOW,COLOR_BLUE);
    init_pair(10,COLOR_BLACK, COLOR_YELLOW);
    //TEMPO text 
    init_pair(11,COLOR_GREEN,COLOR_BLACK);
    init_pair(12,COLOR_YELLOW,COLOR_BLACK);
    init_pair(13,COLOR_RED,COLOR_BLACK);
    init_pair(14,COLOR_BLACK,COLOR_MAGENTA);
    init_pair(15,COLOR_BLACK,COLOR_RED);
    box(stdscr,0,0);
    int offsetSum=1;
    //punteggio
    for (size_t i = 1; i<= PUNTEGGIOVITE; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(3));
    }
    offsetPunteggio=offsetSum;
    offsetSum+=PUNTEGGIOVITE;
    offsetTane=offsetSum;
    attron(COLOR_PAIR(3));
    mvhline(offsetSum,1,' ',maxX-2);
    attroff(COLOR_PAIR(3));
    offsetSum+=1;
    //tane
    for (size_t i=1;i<=maxX-2;i++){
        
        if (i<=3||(i%(maxX/NTANE)==0) || i>=maxX-3){
            attron(COLOR_PAIR(3));
            mvvline(offsetSum,i,' ',TANE);
            attroff(COLOR_PAIR(3));
        }
        else{
            attron(COLOR_PAIR(9));
            mvvline(offsetSum,i,' ',TANE);
            attroff(COLOR_PAIR(9));
        }
        
    }
    offsetSum+=TANE;
    offsetEndTane=offsetSum;
    //fiume
    for (size_t i = offsetSum; i<= FIUME+offsetSum; i++){
        attron(COLOR_PAIR(9));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(9));
    }
    offsetFiume=offsetSum+1;
    offsetSum+=FIUME;

    //prato
    for (size_t i = offsetSum; i<= offsetSum+PRATO; i++){
        attron(COLOR_PAIR(1));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(1));
    }
    offsetPrato=offsetSum;
    offsetSum+=PRATO;

    // autostrada
    for (size_t i = offsetSum; i<= offsetSum+AUTOSTRADA; i++){
        attron(COLOR_PAIR(15));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(15));
    }
    offsetAutostrada=offsetSum-1; //non so perchè ma vuole quel -1 altrimenti non è ben formattato
    offsetSum+=AUTOSTRADA;
    
    offsetMarciapiede=offsetSum;
    //marciapiede
    for (size_t i = offsetSum; i< MARCIAPIEDE+offsetSum; i++){
        attron(COLOR_PAIR(14));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(14));
    }
    offsetSum+=MARCIAPIEDE;
    offsetTempo=offsetSum;
    pthread_mutex_unlock(&mutex);
}

void areaDiGioco(){
    pthread_mutex_lock(&mutex);
    secondiRimanenti=maxX-10;
    pthread_mutex_unlock(&mutex);
    while(gioca){
        erase();
        macchineGenerateCorrettamente = true;
        windowGeneration();
        mostraVita();
        mostraPunteggio();
        checkTane();
        checkRanaInTana();
        collisioneProiettileRanaTronco();
        showTime();
        for (int i = 0; i<NUMACCHINE; i++){
            if (!(macchine[i].generatoCorrettamente)){
                macchineGenerateCorrettamente = false;
                fprintf(fp, "le macchine non sono ancora state generate correttamente\n");
                fflush(fp);
            }
        }
        if (macchineGenerateCorrettamente){
            printMacchine();
            ranaCollideConMacchine();
            proiettileRanaCollideConMacchine();
            ranaKillTronchi();
            enemyKillRana();
            ranaSulFiume();
            printTronchi();
            printRana();
        }
        usleep(500);
        refresh();
    }
}
void ranaSulFiume(){
    for (int i = 0; i<NUMTRONCHI; i++){
        pthread_mutex_lock(&mutex);
        if (tronchi[i].y == rana.y){
            if((rana.y==tronchi[i].y && (rana.x<tronchi[i].x || rana.x>=tronchi[i].x+8))||(tronchi[i].enemy)){
                rana.x = maxX/2;
                rana.y = offsetMarciapiede;
                if (vite>0){
                    vite--;
                    secondiRimanenti=maxX-10;
                    pthread_t killato;
                    pthread_create(&killato,NULL,&playKilled,NULL);
                    pthread_join(killato,NULL);
                }
                else if(gioca){
                    riprova();
                }
                
            }
            else if(bakIDTroncoPunteggio!=i){ // la rana è su un tronco, non può scendere e salire sullo stesso tronco altrimenti potrebbe ottenere un punteggio alto pur facendo niente
                punteggio+=500;
                bakIDTroncoPunteggio=i;
            }
        }
        pthread_mutex_unlock(&mutex);
    }
}
void chiudiTana(int n){
    for (size_t i=1;i<=maxX-2;i++){
        attron(COLOR_PAIR(8));
        if ((i>n*(maxX/NTANE)) && (i<(n+1)*(maxX/NTANE))){
            mvvline(offsetTane+1,i,'x',TANE);
        }
        attroff(COLOR_PAIR(8));
    }
}
void checkRanaInTana(){
    pthread_mutex_lock(&mutex);
    for (size_t i=0;i<NTANE;i++){ 
        if (rana.y<10 && (rana.x>i*(maxX/NTANE)) && (rana.x<(i+1)*(maxX/NTANE))){
            if (!taneChiuse[i]){ // se la tana è aperta
                taneChiuse[i]=1; // chiude la tana
                punteggio+=500;
                flagTime=1;
                usleep(2000); // altrimenti la timesignal legge sempre 0 e la barra del tempo impiega troppo a reiniziare
            }
            else if(taneChiuse[i]){
                if (vite>0){
                    pthread_t killato;
                    pthread_create(&killato,NULL,&playKilled,NULL);
                    pthread_join(killato,NULL);
                    vite--;
                }
                else{
                    riprova();
                }
            }
            rana.x=maxX/2;
            rana.y=offsetMarciapiede;
        }
    }

    // CHIUDI TANA se è chiusa
    for (size_t i=0;i<NTANE;i++){
        if(taneChiuse[i]==1){
            chiudiTana(i);
        }
    }
    pthread_mutex_unlock(&mutex);
}
void checkTane(){
    //TANE TUTTE OCCUPATE? -> GIOCATORE HA VINTO
    pthread_mutex_lock(&mutex);
    for (size_t i=0;i<NTANE;i++){
        if(taneChiuse[i]==1){
            totaleTaneChiuse+=1;
        }
    }
    if (totaleTaneChiuse==NTANE){
        clear();
        refresh();
        pthread_t win;
        if(gioca){
            pthread_create(&win,NULL,&playWinner,NULL);
            pthread_join(win,NULL);
        }
        mvprintw(maxY/2,maxX/2,"HAI VINTO!");
        mvprintw(maxY/2+1,maxX/2,"Il tuo score finale è %d",punteggio);
        refresh();
        sleep(5);
        pthread_mutex_lock(&mutex);
        gioca=false;
        pthread_mutex_unlock(&mutex);
        exit(0);
    }
    else{
        totaleTaneChiuse=0;
    }
    pthread_mutex_unlock(&mutex);
}
void* calculateResidualTime(void* X){
    while(gioca){
        pthread_mutex_lock(&mutex);
        if(flagTime){
        flagTime=0;
        secondiRimanenti=maxX-10;
        }
        secondiRimanenti--;
        if(secondiRimanenti==0 && !flagTime){
            if(vite>0){
                vite--;
                secondiRimanenti=maxX-10;
                rana.y=offsetMarciapiede;
                rana.x=maxX/2;
                pthread_t killato;
                pthread_create(&killato,NULL,&playKilled,NULL);
                pthread_join(killato,NULL);
            }
            else if(gioca){
                riprova();
            }
        }
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    
}
void showTime(){
    pthread_mutex_lock(&mutex);
    if (secondiRimanenti>=((maxX-2)/2)){
        attron(COLOR_PAIR(1));
        mvhline(offsetTempo,1,' ',secondiRimanenti);
        mvhline(offsetTempo+1,1,' ',secondiRimanenti);
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(11));
        mvprintw(offsetTempo,maxX-8,"TEMPO");
        attroff(COLOR_PAIR(11));
    }
    else if(secondiRimanenti<((maxX-2)/2) && secondiRimanenti>=((maxX-2)/4)){
        attron(COLOR_PAIR(3));
        mvhline(offsetTempo,1,' ',secondiRimanenti);
        mvhline(offsetTempo+1,1,' ',secondiRimanenti);
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(12));
        mvprintw(offsetTempo,maxX-8,"TEMPO");
        attroff(COLOR_PAIR(12));
    }
    else{
        attron(COLOR_PAIR(2));
        mvhline(offsetTempo,1,' ',secondiRimanenti);
        mvhline(offsetTempo+1,1,' ',secondiRimanenti);
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(13));
        mvprintw(offsetTempo,maxX-8,"TEMPO");
        attron(COLOR_PAIR(13));
    }
    pthread_mutex_unlock(&mutex);
}
void riprova(){
    char* choices[]={"Si","NO"};
    clear();
    refresh();
    mvprintw(maxY/2,maxX/2,"Hai Perso!");
    mvprintw(maxY/2+1,maxX/2,"Il tuo score finale è %d",punteggio);
    refresh();
    sleep(4);
    clear();
    pthread_t musicaEndGame;
    //gioca
    pthread_mutex_lock(&mutex);
    int choice=menu("HAI PERSO","Vuoi Riprovare?",choices,2,true,true);
    if(gioca){
        pthread_create(&musicaEndGame,NULL,&playEndGame,NULL);
        pthread_join(musicaEndGame,NULL);
    }
    
    if(choice==0 || choice==2){
        gioca=false;
    }else{
        // resetto tutto
        vite=LVLVITE;
        punteggio=0;
        for(size_t i=0;i<NTANE;i++){
            taneChiuse[i]=0;
        }
        secondiRimanenti=maxX-10;
    }
    pthread_mutex_unlock(&mutex);
}
void mostraVita(){
    attron(COLOR_PAIR(3));
    attron(A_BOLD);
    mvprintw(offsetPunteggio,2,"Vite : ");
    // VITE CON SIMBOLO
    // int x=8;
    // for (int i=0;i<vite;i++){
    //     mvaddch(offsetPunteggio,x+=2,'#');
    // }
    mvprintw(offsetPunteggio,9,"%d",vite);
    attroff(COLOR_PAIR(3));
    attroff(A_BOLD);
    return;
}
void mostraPunteggio(){
    attron(COLOR_PAIR(3));
    attron(A_BOLD);
    mvprintw(offsetPunteggio,maxX-17,"Punteggio : %d",punteggio);
    attroff(A_BOLD);
    attron(COLOR_PAIR(3));
    return;
}
void proiettileRanaCollideConMacchine(){
    for (int i = 0; i<NUMACCHINE; i++){
        if (macchine[i].y == ranaProiettile.y){
            if(macchine[i].type==1){// camion
                if (ranaProiettile.x>=macchine[i].x && ranaProiettile.x<=macchine[i].x+7){
                    pthread_mutex_lock(&mutex);
                    ranaProiettile.y = -1;
                    pthread_mutex_unlock(&mutex);
                }
            }
            else{
                if (ranaProiettile.x>= macchine[i].x && ranaProiettile.x<=macchine[i].x+4){
                    pthread_mutex_lock(&mutex);
                    ranaProiettile.y = -1;
                    pthread_mutex_unlock(&mutex);
                }
            }
        }
    }
}
void ranaCollideConMacchine(){
    for (int i = 0; i<NUMACCHINE; i++){
        if (macchine[i].y == rana.y){
            if(macchine[i].type==1){// camion
                if (rana.x>=macchine[i].x && rana.x<=macchine[i].x+7){
                    pthread_mutex_lock(&mutex);
                    rana.x = maxX/2;
                    rana.y = offsetMarciapiede;
                    if (vite>0){
                        vite--;
                        secondiRimanenti=maxX-10;
                        pthread_t killato;
                        pthread_create(&killato,NULL,&playKilled,NULL);
                        pthread_join(killato,NULL);
                    }
                    else if(gioca){
                        riprova();
                    }
                    pthread_mutex_unlock(&mutex);
                }
            }
            else{
                if (rana.x>= macchine[i].x && rana.x<=macchine[i].x+4){
                    pthread_mutex_lock(&mutex);
                    rana.x = maxX/2;
                    rana.y = offsetMarciapiede;
                    if (vite>0){
                        vite--;
                        secondiRimanenti=maxX-10;
                        pthread_t killato;
                        pthread_create(&killato,NULL,&playKilled,NULL);
                        pthread_join(killato,NULL);
                    }
                    else if(gioca){
                        riprova();
                    }
                    pthread_mutex_unlock(&mutex);
                }
            }
        }
    }
}
void ranaKillTronchi(){
    for (int i = NUMTRONCHI-1; i>= 0;i--){
        if (tronchi[i].enemy){
            if (ranaProiettile.y == tronchi[i].y && ranaProiettile.sparato){
                if (ranaProiettile.x == tronchi[i].x+3 || ranaProiettile.x == tronchi[i].x+4){
                    pthread_mutex_lock(&mutex);
                    tronchi[i].enemy = false;
                    tronchi[i].killed = true;
                    ranaProiettile.y = -1;
                    i = NUMTRONCHI;
                    pthread_mutex_unlock(&mutex);
                }
            }
        }
    }
}

void enemyKillRana(){
    for (int i = NUMTRONCHI-1; i>= 0;i--){
        if (tronchiProiettili[i].sparato){
            if (rana.y == tronchiProiettili[i].y){
                if (rana.x == tronchiProiettili[i].x ){
                    pthread_mutex_lock(&mutex);
                    rana.x = maxX/2;
                    rana.y = offsetMarciapiede;
                    if (vite>0){
                        vite--;
                        secondiRimanenti=maxX-10;
                        pthread_t killato;
                        pthread_create(&killato,NULL,&playKilled,NULL);
                        pthread_join(killato,NULL);
                    }
                    else if(gioca){
                        riprova();
                    }
                    i = NUMTRONCHI;
                    pthread_mutex_unlock(&mutex);
                }
            }
        }
    }
}
void printRana(){
    pthread_t musicaProiettile;
    pthread_mutex_lock(&mutex);
    mvprintw(rana.y,rana.x,"\\/");
    mvprintw(rana.y+1,rana.x,"/\\");
    ranaProiettile.sparato?mvaddch(ranaProiettile.y, ranaProiettile.x, '*'):1;
    if(ranaProiettile.sparato && !count){ //count serve per avviare la musica una volta e basta
        pthread_create(&musicaProiettile,NULL,&playProiettile,NULL);
        pthread_join(musicaProiettile,NULL);
        count++;
    }
    if(ranaProiettile.y==offsetEndTane && ranaProiettile.sparato==0){ // playProiettile può ricominciare
        count=0;
    }
    pthread_mutex_unlock(&mutex);
}
void printMacchine(){
    for (int i = 0; i<NUMACCHINE; i++){
            attron(COLOR_PAIR(15)| A_BOLD);
            
            if (macchine[i].type == 1){
                pthread_mutex_lock(&mutex);
                mvprintw(macchine[i].y,macchine[i].x,"/-----\\");
                mvprintw(macchine[i].y+1,macchine[i].x,"O-----O");
                pthread_mutex_unlock(&mutex);
            }
            else{
                pthread_mutex_lock(&mutex);
                mvprintw(macchine[i].y,macchine[i].x,"/--\\");
                mvprintw(macchine[i].y+1,macchine[i].x,"O--O");
                pthread_mutex_unlock(&mutex);
            }
            
            attroff(COLOR_PAIR(15) | A_BOLD);
        }
}
void printTronchi(){
    attron(COLOR_PAIR(9) | A_BOLD);
    for(size_t i = 0; i<NUMTRONCHI; i++){ 
        if(tronchi[i].enemy==false ){
            pthread_mutex_lock(&mutex);
            mvprintw(tronchi[i].y,tronchi[i].x,"|------|");
            mvprintw(tronchi[i].y+1,tronchi[i].x,"|------|");
            pthread_mutex_unlock(&mutex);
        }
        else if(tronchi[i].enemy){//C'è un nemico
            pthread_mutex_lock(&mutex);
            mvprintw(tronchi[i].y,tronchi[i].x,"|--00--|");
            mvprintw(tronchi[i].y+1,tronchi[i].x,"|--||--|");
            pthread_mutex_unlock(&mutex);
        }
    }
    //stampa proiettili dei nemici sui tronchi \addindex sparati
    for (size_t i = 0; i< NUMTRONCHI; i++){
        if (tronchiProiettili[i].sparato){
            pthread_mutex_lock(&mutex);
            mvaddch(tronchiProiettili[i].y, tronchiProiettili[i].x, '*');
            pthread_mutex_unlock(&mutex);
        }
    }
    attroff(COLOR_PAIR(9) | A_BOLD);
}
void collisioneProiettileRanaTronco(){
    for (int i = NUMTRONCHI-1; i>= 0;i--){
        pthread_mutex_lock(&mutex);
        if(tronchiProiettili[i].y==ranaProiettile.y && tronchiProiettili[i].x==ranaProiettile.x){
            ranaProiettile.sparato=false;
            tronchiProiettili[i].sparato=false;
        }
        pthread_mutex_unlock(&mutex);
    }
}