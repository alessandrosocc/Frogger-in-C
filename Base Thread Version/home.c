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
    for (int i = 0;i<NUMACCHINE; i++){
        carId[i] = i;
    }

    // id thread
    pthread_t ranaId, ranaProiettileId;
    pthread_t macchineId[NUMACCHINE];
    
    // creazione dei thread
    pthread_create(&ranaId, NULL, &ffrog, NULL);
    pthread_create(&ranaProiettileId, NULL, &bullet, NULL);
    for (int i = 0; i<NUMACCHINE; i++){
        pthread_create(&macchineId[i], NULL, &car,(void*)&carId[i]);
    }
    sleep(2);
    areaDiGioco();

    pthread_join(ranaId,NULL);
    pthread_join(ranaProiettileId,NULL);
    for (int i = 0;i<NUMACCHINE; i++){
        pthread_join(macchineId[i],NULL);
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
        mvprintw(rana.y,rana.x,"\\/");
        mvprintw(rana.y+1,rana.x,"/\\");
        mvaddch(ranaProiettile.y, ranaProiettile.x, '*');
        for (int i = 0; i<NUMACCHINE; i++){
            if (!(macchine[i].generatoCorrettamente)){
                macchineGenerateCorrettamente = false;
                fprintf(fp, "le macchine non sono ancora state generate correttamente\n");
                fflush(fp);
            }
        }
        fprintf(fp, "%d\n", cambioRigaLibero);
        if (macchineGenerateCorrettamente){
            for (int i = 0; i<NUMACCHINE; i++){
                    attron(COLOR_PAIR(4));
                    if (macchine[i].type == 1){
                        mvprintw(macchine[i].y,macchine[i].x,"/-----\\");
                        mvprintw(macchine[i].y+1,macchine[i].x,"O-----O");
                    }
                    else{
                        mvprintw(macchine[i].y,macchine[i].x,"/--\\");
                        mvprintw(macchine[i].y+1,macchine[i].x,"O--O");
                    }
                    attroff(COLOR_PAIR(4));
            }
            // for (int i = 0; i<NUMACCHINE; i++){
            // for (int j = 0; j<NUMACCHINE; j++){
            //     if (macchine[i].y == macchine[j].y && i != j){
            //         if (!(macchine[j].x > macchine[i].x+7 || macchine[j].x+7 < macchine[i].x))
            //         {
            //             //fprintf(fp, "la macchina %d e la macchina %d collidono durante il cambiamento di corsia\n", i, j);
            //         }
            //     }
            // }
        
        }
        
        usleep(500);
        refresh();
        
    }
}

void displayTime(){
    pthread_mutex_lock(&mutex);
    fprintf(fp,"secondi %d\n",secondiRimanenti);
    fflush(fp);
    secondiRimanenti--;
    if(secondiRimanenti==0){
        secondiRimanenti=maxX-2;
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
    usleep(20000);
    pthread_mutex_unlock(&mutex);
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