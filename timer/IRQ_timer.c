/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../game.h"
#include "../RIT/RIT.h"
#include "../ADC/ADC.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int flagPlay;
extern int key;
extern char num1[6];
extern char num2[6];

extern int currX;
extern int currY;


volatile uint16_t SinTable[45]=
{
		410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

void TIMER0_IRQHandler (void)
{
	if(flagPlay == 1){
		movePaddle2();
		moveBall();																					// movimento della pallina
		if(currX<50 && currY>R/2-10 && currY<R/2+30)				// riscrivo lo score1 perché viene sovrascritto dalla palla
			GUI_Text(20, R/2, (uint8_t *) num1, White, Blue);
		else if(currX>C-30 && currY>R/2-30 && currY<R/2+30)					// riscrivo lo score2 perché viene sovrascritto dalla palla
			GUI_TextReverse(C-25, R/2+15, (uint8_t *) num2, White, Blue);
		ADC_start_conversion();
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	static int ticks=0;
	LPC_DAC->DACR=SinTable[ticks]<<6;
	ticks++;
	if(ticks==45) {
		ticks=0;
		disable_timer(1);
	}
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	static int ticks=0;
	LPC_DAC->DACR=SinTable[ticks]<<6;
	ticks++;
	if(ticks==45) {
		ticks=0;
		disable_timer(2);
	}
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
