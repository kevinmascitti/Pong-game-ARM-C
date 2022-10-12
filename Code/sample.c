/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT and joystick buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "adc/adc.h"
#include "GLCD_TP/GLCD.h"
#include "game.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	LCD_Initialization();
  BUTTON_init();												/* BUTTON Initialization              */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	init_timer(0,0x000186A0);							/* timer 0 per palla e paddle a 1 ms				*/
	init_timer(1,1062);										/* Inizializzo timer 1 per paddle	 - acuto 	*/
	init_timer(2,3000);										/* Inizializzo timer 2 per muro - grave 		*/
	enable_timer(1);											/* Altrimenti la prima volta che lo uso non funziona a causa del simulatore */
	ADC_init();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	start();
  while (1) {                           /* Loop forever                       */
		__ASM("wfi");
  }

}
