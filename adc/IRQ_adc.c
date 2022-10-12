/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../game.h"
#include "stdlib.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

extern int flagPlay;
extern int X;
extern int Y;

void ADC_IRQHandler(void) {
	AD_current = ( (LPC_ADC->ADGDR>>4) & 0xFFF );
	if(abs(AD_current-AD_last)>70){											// 70 è il valore di sicurezza per non ridisegnare ogni volta il paddle
		deletePaddle(X,Y);																// a causa dell'incertezza del potenziometro
		X=AD_current*(C+1)/0xFFF;													// 240 posizioni possibili del centro del paddle
		if(X<L+W)	X=L+W;																			// gestione centro del paddle troppo a sx 
		if(X>C-L-W)	X=C-L-W;																	// gestione centro del paddle troppo a dx
		drawPaddle(X,Y);
		AD_last=AD_current;
	}
}
