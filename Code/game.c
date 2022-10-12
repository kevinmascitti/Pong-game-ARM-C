#include "game.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

	// coordinate del centroo della palla
	int prevX;
	int prevY;
	int currX;
	int currY;
	int nextX;
	int nextY;
	
	// coordinate del centro del paddle
	int X;
	int Y;
	
	// coordinate del centro del paddle2 superiore
	int X2;
	int Y2;

	int flagPlay = 0;			// 0 - il gioco deve iniziare. 1 - il gioco è in play. 2 - il gioco è in pausa. -1 - gameover.
	int score1;
	int score2;
	char num1[6];
	char num2[6];
	
	char strReverse[9];
	
	static int i = 0;
	static int dirX;
	static int dirY;
	static int dirP;
	static int incr;

void start(void){
	LCD_Clear(Blue);
	flagPlay=0;
	GUI_Text(40, R/2, (uint8_t *) "Press KEY1 to start", Red, Blue);
	score1=0;
	score2=0;
	dirP=1;
}

void play(void){
	LCD_Clear(Blue);
	for(i=0; i<W; i++){									// disegno i muri
		LCD_DrawLine(i,0,i,R,Red);
		LCD_DrawLine(C-i,0,C-i,R,Red);
	}
	prevX=C-W-1;
	prevY=(R+1)/2-2;
	currX=C-W-B-1;
	currY=(R+1)/2;
	nextX=C-W-B-1-2;
	nextY=(R+1)/2+2;
	drawBall(currX,currY);
	X=C/2;
	Y=R-H;
	drawPaddle(X,Y);
	X2=C/2;
	Y2=H-S-1;
	drawPaddle(X2,Y2);
	
	sprintf(num1, "%d", score1);
	GUI_Text(20, R/2, (uint8_t *) num1, White, Blue);
	sprintf(num2, "%d", score2);
	GUI_TextReverse(C-25, R/2+15, (uint8_t *) num2, White, Blue);
	
	flagPlay=1;
	enable_timer(0);
}

void pause(void){
	disable_timer(0);
	flagPlay=2;																								// gli do il valore 2 per indicare che il gioco è in pausa
	GUI_Text(40, R/2-20, (uint8_t *) "PAUSE", Red, Blue);
	GUI_Text(40, R/2+10, (uint8_t *) "Press KEY1 to resume", Red, Blue);
}

void resume(void){
	GUI_Text(40, R/2-20, (uint8_t *) "PAUSE", Blue, Blue);
	GUI_Text(40, R/2+10, (uint8_t *) "Press KEY1 to resume", Blue, Blue);
	drawBall(currX,currY);
	flagPlay=1;
	enable_timer(0);
}

void lostBall(int loser){
	disable_timer(0);
	
	sprintf(num1, "%d", score1);
	GUI_Text(20, R/2, (uint8_t *) num1, Blue, Blue);
	sprintf(num2, "%d", score2);
	GUI_TextReverse(C-25, R/2+15, (uint8_t *) num2, Blue, Blue);
	
	deleteBall(currX,currY);
	if(loser==1)
		score2++;
	else if(loser==2)
		score1++;

	sprintf(num1, "%d", score1);
	GUI_Text(20, R/2, (uint8_t *) num1, White, Blue);
	sprintf(num2, "%d", score2);
	GUI_TextReverse(C-25, R/2+15, (uint8_t *) num2, White, Blue);
	
	if(score1==THRESHOLD)
		gameover(1);
	else if(score2==THRESHOLD)
		gameover(2);
	else{
		prevX=C-W-1;
		prevY=(R+1)/2-2;
		currX=C-W-B-1;
		currY=(R+1)/2;
		nextX=C-W-B-1-2;
		nextY=(R+1)/2+2;
		drawBall(currX,currY);
		enable_timer(0);
	}
}

void gameover(int winner){
	LCD_Clear(Blue);
	flagPlay=-1;
	
	if(winner==1){
		char temp;
		int i = 0, j =0;
		strcpy(strReverse, "You Lose");
		j = strlen (strReverse) - 1;
		while ( i < j){
			temp = strReverse[j];
			strReverse[j] = strReverse[i];
			strReverse[i] = temp;
			i++;
			j--;
		}  
		GUI_Text(C/2-20, R/2+40, (uint8_t *) "You Win", Red, Blue);
		GUI_TextReverse(C/2-30, R/2-40, (uint8_t *) strReverse, Red, Blue);
	}
	else if(winner==2){
		char temp;
		int i = 0, j =0;
		strcpy(strReverse, "You Win");
		j = strlen (strReverse) - 1;
		while ( i < j){
			temp = strReverse[j];
			strReverse[j] = strReverse[i];
			strReverse[i] = temp;
			i++;
			j--;
		}
		GUI_Text(C/2-30, R/2+40, (uint8_t *) "You Lose", Red, Blue);
		GUI_TextReverse(C/2-20, R/2-40, (uint8_t *) strReverse, Red, Blue);
	}
	GUI_Text(20, C, (uint8_t *) "Press INT0 for a new game", Green, Blue);
	
}

char* reverse(char* str){
	int i,j;
	char temp;
	for(i=0,j=strlen(str)-1;i<strlen(str)/2;i++,j--){
		temp=str[i];
		str[i]=str[j];
		str[j]=temp;
	}
	return str;
}

void drawBall(int currX, int currY){
	for(i=currX-B; i<=currX+B; i++)
		LCD_DrawLine(i,currY-B,i,currY+B,Green);
}

void deleteBall(int currX, int currY){
	for(i=currX-B; i<=currX+B; i++)
		LCD_DrawLine(i,currY-B,i,currY+B,Blue);
}

void moveBall(void){
	
	deleteBall(currX,currY);
	
	prevX=currX;
	prevY=currY;
	currX=nextX;
	currY=nextY;
	dirX=currX-prevX;
	dirY=currY-prevY;
	
	if( !hitPaddle() && !hitWallLR() && !hitPaddle2() ){
		nextX=currX+currX-prevX;
		nextY=currY+currY-prevY;
	}
	
	else if( hitPaddle()==1 && hitWallLR() ){						// colpisce il paddle e il muro contemporaneamente
		if(currX==W+B){																		// muro sx
			nextX+=2;
			nextY+=-1;
		}
		else{																							// muro dx
			nextX+=-2;	
			nextY+=-1;
		}
	}
	
	else if( hitPaddle()==1 ){													// colpisce solo il paddle e lo fa correttamente
		enable_timer(1);

		if     (currX<X-L-B+(L*2+1+2*B)/7)   {  nextX+=-2; nextY+=-1; }
		else if(currX<X-L-B+2*(L*2+1+2*B)/7) {  nextX+=-2; nextY+=-2; }
		else if(currX<X-L-B+3*(L*2+1+2*B)/7) {  nextX+=-1; nextY+=-2; }
		else if(currX<X-L-B+4*(L*2+1+2*B)/7) {	nextX+=0;	 nextY+=-2; }	
		else if(currX<X-L-B+5*(L*2+1+2*B)/7) {	nextX+=1;  nextY+=-2; }
		else if(currX<X-L-B+6*(L*2+1+2*B)/7) {	nextX+=2;	 nextY+=-2; }
		else if(currX<X-L-B+7*(L*2+1+2*B)/7) {	nextX+=2;	 nextY+=-1; }
		
	}
	
	else if( hitWallLR() && hitPaddle2() ){							// colpisce il paddle2 e il muro contemporaneamente
		if(currX==W+B){																		// muro sx
			nextX+=2;
			nextY+=1;
		}
		else{																							// muro dx
			nextX+=-2;	
			nextY+=1;
		}
	}
	
	else if( hitPaddle2() ){							// colpisce il paddle2 e lo fa correttamente
		enable_timer(1);

		if     (currX<X2-L-B+(L*2+1+2*B)/7)   {  nextX+=-2; nextY+=1; }
		else if(currX<X2-L-B+2*(L*2+1+2*B)/7) {  nextX+=-2; nextY+=2; }
		else if(currX<X2-L-B+3*(L*2+1+2*B)/7) {  nextX+=-1; nextY+=2; }
		else if(currX<X2-L-B+4*(L*2+1+2*B)/7) {	nextX+=0;	 nextY+=2; }	
		else if(currX<X2-L-B+5*(L*2+1+2*B)/7) {	nextX+=1;  nextY+=2; }
		else if(currX<X2-L-B+6*(L*2+1+2*B)/7) {	nextX+=2;	 nextY+=2; }
		else if(currX<X2-L-B+7*(L*2+1+2*B)/7) {	nextX+=2;	 nextY+=1; }
		
	}
	
	else if( hitWallLR() ){							// colpisce il muro a destra o sinistra
		enable_timer(2);
		nextX=prevX;
		nextY=currY+currY-prevY;
	}
	
	if( currY>R-H+S+B || hitPaddle()==2 ) 	// palla non più recuperabile o colpisce male il paddle
		lostBall(1);
	else if( currY<H-S-B || hitPaddle2()==2 ) // palla non più recuperabile o colpisce male il paddle2 superiore
		lostBall(2);
	else
		drawBall(currX,currY);
}

void movePaddle2(void){
	if(dirP==1){
		incr=V;
		if(X2+incr>=C-L-W){
			incr=C-L-W-X2;
			dirP=-1;
		}
		for(i=0; i<incr; i++){
			LCD_DrawLine(X2-L+i, Y2, X2-L+i, Y2+S, Blue);
			LCD_DrawLine(X2+L+i, Y2, X2+L+i, Y2+S, Green);
		}
		X2+=incr;
	}
	else if(dirP==-1){
		incr=-V;
		if(X2+incr<=W+L){
			incr=W+L-X2;
			dirP=1;
		}
		for(i=0; i<abs(incr); i++){
			LCD_DrawLine(X2+L-i, Y2, X2+L-i, Y2+S, Blue);
			LCD_DrawLine(X2-L-i, Y2, X2-L-i, Y2+S, Green);
		}
		X2+=incr;
	}
}

void drawPaddle(int X, int Y){
	for(i=X-L;i<=X+L;i++)											// paddle largo 2*L+1=51 pixel con (X,Y) come punto centrale
		LCD_DrawLine(i,Y,i,Y+S,Green);
}

void deletePaddle(int X, int Y){
	for(i=X-L;i<=X+L;i++)
		LCD_DrawLine(i,Y,i,Y+S,Blue);
}

int hitPaddle(void){
	if( (currY==R-H-B-1 || (currY==R-H-B-1-1 && abs(dirY)==2) ) && currX<=X+L+B && currX>=X-L-B )
		return 1;																// colpisce il paddle sulla superficie corretta, anche con un solo pixel della palla
	else if( currY>R-H-B-1 && currY<=R-H+S+B+1 && currX<=X+L+B+1 && currX>=X-L-B-1 )
		return 2;																// colpisce il paddle al suo interno o sui bordi esterni quindi ritorna 2 per gameover
	return 0;
}

int hitPaddle2(void){
	if( (currY==H+B || (currY==H+B-1 && abs(dirY)==2) ) && currX<=X2+L+B && currX>=X2-L-B )
		return 1;																// colpisce il paddle sulla superficie corretta, anche con un solo pixel della palla
	else if( currY<H+B && currY>=H-S-B-1 && currX<=X2+L+B+1 && currX>=X2-L-B-1 )
		return 2;																// colpisce il paddle al suo interno o sui bordi esterni quindi ritorna 2 per gameover
	return 0;
}

int hitWallLR(void){
	if( ( currX==W+B || currX==C-W-B-1 || (currX==W+B+1 && abs(dirX)==2) || (currX==C-W-B-1-1 && abs(dirX)==2) ) && currY<R-H+B )
		return 1;
	return 0; }
