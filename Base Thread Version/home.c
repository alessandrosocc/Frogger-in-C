// inclusione dei file esterni
#include "home.h"

int main(){
    time(NULL);
    srand(time(NULL));
    fp = fopen("log.txt", "w");
    initScreen();
    windowGeneration();
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
    // creazione dei thread
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
    sleep(2);
    areaDiGioco();

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
    endwin();
    fclose(fp);
    return 0;
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
    
    box(stdscr,0,0);
    int offsetSum=1;
    //punteggio
    for (size_t i = 1; i<= PUNTEGGIO; i++){
        attron(COLOR_PAIR(7));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(7));
    }
    offsetPunteggio=offsetSum;
    offsetSum+=3;
    offsetTane=offsetSum;
    //tane
    for (size_t i=1;i<=maxX-2;i++){
        attron(COLOR_PAIR(3));
        if (i<=3||(i%(maxX/NTANE)==0) || i>=maxX-3){
            mvvline(offsetSum,i,' ',TANE);
        }
        attroff(COLOR_PAIR(3));
    }
    offsetSum+=TANE;

    //fiume
    for (size_t i = offsetSum; i<= FIUME+offsetSum; i++){
        attron(COLOR_PAIR(6));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(6));
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
        attron(COLOR_PAIR(4));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(4));
    }
    offsetAutostrada=offsetSum-1; //non so perchè ma vuole quel -1 altrimenti non è ben formattato
    offsetSum+=AUTOSTRADA;
    
    offsetMarciapiede=offsetSum;
    //marciapiede
    for (size_t i = offsetSum; i< MARCIAPIEDE+offsetSum; i++){
        attron(COLOR_PAIR(2));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(2));
    }
    offsetSum+=MARCIAPIEDE;
    offsetVite=offsetSum;
    //vite
    for (size_t i = offsetSum; i<= TEMPO+offsetSum; i++){
        attron(COLOR_PAIR(3));
        mvhline(i, 1, ' ', 20);
        attroff(COLOR_PAIR(3));
    }

    offsetSum+=VITE;
    offsetTempo=offsetSum+1;

    //tempo
    for (size_t i = offsetSum; i<= offsetSum+3; i++){
        attron(COLOR_PAIR(4));
        mvhline(i, 1, ' ', maxX-2);
        attroff(COLOR_PAIR(4));
    }
    pthread_mutex_unlock(&mutex);
}

void areaDiGioco(){
    pthread_mutex_lock(&mutex);
    secondiRimanenti=maxX-2;
    pthread_mutex_unlock(&mutex);

    while(true){
        erase();
        macchineGenerateCorrettamente = true;
        windowGeneration();
        
        mostraVita();
        mostraPunteggio();
        displayTime();
        checkTane();
        checkRanaInTana();
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
        if (tronchi[i].y == rana.y){
            if((rana.y==tronchi[i].y && (rana.x<tronchi[i].x || rana.x>=tronchi[i].x+8))||(tronchi[i].enemy)){
                pthread_mutex_lock(&mutex);
                rana.x = maxX/2;
                rana.y = offsetMarciapiede;
                pthread_mutex_unlock(&mutex);

            }
        }
    }
}
void chiudiTana(int n){
    for (size_t i=1;i<=maxX-2;i++){
        attron(COLOR_PAIR(8));
        if ((i>n*(maxX/NTANE)) && (i<(n+1)*(maxX/NTANE))){
            mvvline(offsetTane,i,'x',TANE);
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
                    vite--;
                }
                else{
                    clear();
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
        mvprintw(maxY/2,maxX/2,"HAI VINTO!");
        refresh();
        sleep(5);
        exit(0);
    }
    else{
        totaleTaneChiuse=0;
    }
    pthread_mutex_unlock(&mutex);
}
void displayTime(){
    pthread_mutex_lock(&mutex);
    if(flagTime){
        flagTime=0;
        secondiRimanenti=maxX-2;
    }
    secondiRimanenti--;
    if(secondiRimanenti==0 && !flagTime){
        if(vite>0){
            vite--;
            secondiRimanenti=maxX-2;
            rana.y=offsetMarciapiede;
            rana.x=maxX/2;
        }
        else{
            riprova();
        }
        
    }
    
    if (secondiRimanenti>=((maxX-2)/2)){
        attron(COLOR_PAIR(1));
        mvhline(offsetTempo,1,' ',secondiRimanenti);
        mvhline(offsetTempo+1,1,' ',secondiRimanenti);
        attroff(COLOR_PAIR(1));
    }
    else if(secondiRimanenti<((maxX-2)/2) && secondiRimanenti>=((maxX-2)/4)){
        attron(COLOR_PAIR(3));
        mvhline(offsetTempo,1,' ',secondiRimanenti);
        mvhline(offsetTempo+1,1,' ',secondiRimanenti);
        attroff(COLOR_PAIR(3));
    }
    else{
        attron(COLOR_PAIR(2));
        mvhline(offsetTempo,1,' ',secondiRimanenti);
        mvhline(offsetTempo+1,1,' ',secondiRimanenti);
        attroff(COLOR_PAIR(2));
    }
    usleep(90000);
    pthread_mutex_unlock(&mutex);
}

void riprova(){
    clear();
    mvprintw(50,50,"hai perso manche");
    refresh();
    sleep(5);
    
}
void mostraVita(){
    int x=3;
    for (int i=0;i<vite;i++){
        mvaddch(offsetVite+1,x+=2,'#');
    }
    return;
}
void mostraPunteggio(){
    mvprintw(2,maxX/2-2,"%d",punteggio);
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
                    vite--;
                    pthread_mutex_unlock(&mutex);
                }
            }
            else{
                if (rana.x>= macchine[i].x && rana.x<=macchine[i].x+4){
                    pthread_mutex_lock(&mutex);
                    rana.x = maxX/2;
                    rana.y = offsetMarciapiede;
                    vite--;
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
                    vite--;
                    i = NUMTRONCHI;
                    pthread_mutex_unlock(&mutex);
                }
            }
        }
    }
}
void printRana(){
    pthread_mutex_lock(&mutex);
    mvprintw(rana.y,rana.x,"\\/");
    mvprintw(rana.y+1,rana.x,"/\\");
    mvaddch(ranaProiettile.y, ranaProiettile.x, '*');
    pthread_mutex_unlock(&mutex);
}
void printMacchine(){
    for (int i = 0; i<NUMACCHINE; i++){
            attron(COLOR_PAIR(4));
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
            attroff(COLOR_PAIR(4));
        }
}
void printTronchi(){
    attron(COLOR_PAIR(6));
            for(size_t i = 0; i<NUMTRONCHI; i++){ 
                //fprintf(fp, "log %d pos:%d %d\n", woody[i].c, woody[i].x, woody[i].y);

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
                attroff(COLOR_PAIR(6));
}