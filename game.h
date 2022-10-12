#include "GLCD_TP/GLCD.h"

// VALORI VARIABILI A PIACERE
// gestione completa di tutte le dimensioni possibili di 
// dimensioni LCD, paddle, palla e relativa pos iniziale, muri, punteggi, stampe a video
#define R 319						// ultimo pixel in altezza
#define C 239						// ultimo pixel in larghezza
#define H 32						// pixel da lasciare dal bordo inferiore
#define W 5							// pixel di spessore del muro
#define B 2 						// raggio della palla (escluso il centro)
#define L 25						// lunghezza di un braccio del paddle (escluso il centro)
#define S 10						// spessore del paddle
#define V 5							// velocità del paddle2 superiore
#define THRESHOLD 5			// soglia per finire la partita

void start(void);
void play(void);
void pause(void);
void resume(void);
void lostBall(int loser);
void gameover(int winner);

char* reverse(char* str);
void drawBall(int currx, int currY);
void deleteBall(int currX, int currY);
void moveBall(void);
void movePaddle2(void);

void drawPaddle(int X, int Y);
void deletePaddle(int X, int Y);

int hitPaddle(void);
int hitPaddle2(void);
int hitWallLR(void);
