/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/timer.h"
#include "../adc/adc.h"
#include "../game.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int flagPlay;
extern int key;

void RIT_IRQHandler (void)
{
	static int down=0;
	down++;

	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0 && key==0 && flagPlay==-1){												// EINT0 premuto - reset dopo gameover
		reset_RIT();
		switch(down){
			case 1:
				start();
			break;
			default:
			break;
		}
	}
	
	else if((LPC_GPIO2->FIOPIN & (1<<11)) == 0 && key==1 && (flagPlay==0 || flagPlay==2)){	// KEY1 premuto - inizia a giocare
			reset_RIT();
			switch(down){
				case 1:
					if(flagPlay==0)
						play();
					if(flagPlay==2)
						resume();
				break;
				default:
				break;
			}
	}
	
	else if((LPC_GPIO2->FIOPIN & (1<<12)) == 0 &&  key==2 && flagPlay==1){									// KEY2 premuto - pausa
		reset_RIT();
		switch(down){
			case 1:
				pause();
			break;
			default:
			break;
		}
	}
	
	else{																			// rilasciato
		down=0;
		key=-1;
		disable_RIT();
		reset_RIT();
		NVIC_EnableIRQ(EINT0_IRQn);
		LPC_PINCON->PINSEL4 |= (1<<20);
		NVIC_EnableIRQ(EINT1_IRQn);
		LPC_PINCON->PINSEL4 |= (1<<22);
		NVIC_EnableIRQ(EINT2_IRQn);
		LPC_PINCON->PINSEL4 |= (1<<24);
	}
	LPC_RIT->RICTRL |= 0x1;										// pulisco l'interrupt
}

/******************************************************************************
**                            End Of File
******************************************************************************/
