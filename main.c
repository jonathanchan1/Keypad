/* 
 * File:   main.c
 * Author: Chan
 *
 * Created on November 8, 2017, 10:04 AM
 */

/*________________________________KEYPAD_______________________________*/

#include <p18f452.h>
#include "xlcd.h"fw
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000

//Global Vars
char k[1];

/*---------------------------Delays---------------------------*/
void DelayFor18TCY(void)
{
    Delay1TCY();
 	Delay1TCY();
    Delay1TCY();
    Delay1TCY();
 	Delay1TCY();
    Delay1TCY();
 	Delay1TCY();
 	Delay1TCY();
 	Delay10TCYx(1);
}
 
void DelayXLCD(void)     // minimum 5ms
{
    Delay1KTCYx(5); 		// Delay of 5ms
                            // Cycles = (TimeDelay * Fosc) / 4
                            // Cycles = (5ms * 4MHz) / 4
                            // Cycles = 5,000

}

void DelayPORXLCD(void)   // minimum 15ms
{
    Delay1KTCYx(15);		// Delay of 15ms
                            // Cycles = (TimeDelay * Fosc) / 4
                            // Cycles = (15ms * 4MHz) / 4
                            // Cycles = 15,000

}

/*---------------------------LCD---------------------------*/

 void LCD_setup(void){
    PORTD = 0X00;
    TRISD = 0x00;
    
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    SetDDRamAddr(0x00);              //Start writing at top left hand side of screen
    WriteCmdXLCD( SHIFT_DISP_LEFT );
    while(BusyXLCD());
    WriteCmdXLCD( BLINK_ON );
    while(BusyXLCD());
 }
 
 char readKey(void){

    int key = 0x00;
    char display[1];
    
    //put keypad value into variable "key"
    key = key | PORTCbits.RC7;
    key = key << 1;
    key = key | PORTCbits.RC6;
    key = key << 1;
    key = key | PORTCbits.RC5;
    key = key << 1;
    key = key | PORTCbits.RC4;
  
    switch (key){       //interpret the key pressed
        case 0x00:
          sprintf(display,"1");
          break;
        case 0x01:
          sprintf(display,"2");
          break;
        case 0x02:
          sprintf(display,"3");
          break;
        case 0x03:
          sprintf(display,"A");
          break;

        case 0x04:
          sprintf(display,"4");
          break;
        case 0x05:
          sprintf(display,"5");
          break;
        case 0x06:
          sprintf(display,"6");
          break;
        case 0x07:
          sprintf(display,"B");
          break;

        case 0x08:
          sprintf(display,"7");
          break;
        case 0x09:
          sprintf(display,"8");
          break;
        case 0x0A:
          sprintf(display,"9");
          break;
        case 0x0B:
          sprintf(display,"C");
          break;

        case 0x0C:
          sprintf(display,"*");
          break;
        case 0x0D:
          sprintf(display,"0");
          break;
        case 0x0E:
          sprintf(display,"#");
          break;
        case 0x0F:
          sprintf(display,"D");
          break;
    }
    return display;
 }
 
 void displayKey(char display[1]){
     
        char outputMSG[20];
        
        while(BusyXLCD());
        SetDDRamAddr(0x00);         //Start writing at top left hand side of screen
        while(BusyXLCD());
        sprintf(outputMSG,"Key Pressed: %s",display);
        putsXLCD(outputMSG);        //Print message to screen 
        while(BusyXLCD());
        
 }
 
 /*------------------------------INTERRUPT---------------------------*/
void high_ISR(void);
 
#pragma code high_vector = 0x08
 void high_interrupt_vector(void){
     _asm 
     GOTO high_ISR
     _endasm
 }
#pragma code 
 
#pragma interrupt high_ISR
void high_ISR(void){
    INTCONbits.GIE = 0;
    
    if(INTCONbits.INT0F==1){
     
        k = readKey();
        displayKey(k);
     
        INTCONbits.INT0E = 0;
        INTCONbits.INT0F = 0;
    }
    INTCONbits.INT0E = 1;
    INTCONbits.GIE = 1;
}
 
//____________________MAIN CODE_____________________//
 
 void main(){
    //system config
     
    //Setup interrupts
    INTCONbits.INT0IF = 0;           //Clear INT1 Interrupt Flag             
    INTCONbits.INT0IE = 1;           //Clear INT1 Interrupts  
    INTCON2bits.INTEDG0 = 0;         //Set interrupts to occur on falling edges
    INTCONbits.GIE = 1;              //Enable Global Interrupts last to prevent unwanted interrupts. 
    
    //Set PORT C pins to inputs
    TRISCbits.RC7=1;
    TRISCbits.RC6=1;
    TRISCbits.RC5=1;
    TRISCbits.RC4=1;
    
    //setup LCD
    LCD_setup();
    while(1){}
 }

 
    