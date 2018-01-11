/* 
 * File:   main.c
 * Author: Deived William
 *
 * Created on 5 de Janeiro de 2018, 08:05
 */

#include <stdio.h>
#include <stdlib.h>
#include "System Libs/Config.h"
#include "System Libs/DisplayDrive.h"
#include "System Libs/Graphics2D.h"
#include "System Libs/nRF24L01.h"
#include <stdbool.h>
#include <plib.h>
#include "System Libs/M25P16.h"
#include "System Libs/Hardware.h"
#include "System Libs/Bluetooth.h"

#pragma config FPLLIDIV = DIV_1
#pragma config FPLLMUL = MUL_20
#pragma config UPLLIDIV = DIV_1
#pragma config UPLLEN = OFF
#pragma config FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL
#pragma config FSOSCEN = ON
#pragma config IESO = OFF
#pragma config POSCMOD = XT
#pragma config OSCIOFNC = OFF
#pragma config FPBDIV = DIV_1
#pragma config FCKSM = CSDCMD
#pragma config WDTPS = PS1
#pragma config FWDTEN = OFF
#pragma config DEBUG = OFF
#pragma config ICESEL = ICS_PGx1
#pragma config PWP = OFF
#pragma config BWP = OFF
#pragma config CP = OFF

#define SYS_FREQ 80000000UL
#define MAXIT 128

void mandelBrot(float xx0, float yy0, float w);

void mandelBrot(float xx0, float yy0, float w)
{
    float x;
    float y;
    float d;
    float x0;
    float y0;
    float x2;
    float y2;
    short i,j;
    int k;
   
    d = w/DISP_VER_RESOLUTION;
    
    y0 = yy0;
    
    for(i = 0; i < DISP_VER_RESOLUTION; i++)
    {
        x0 = xx0;
        for(j = 0; j < DISP_VER_RESOLUTION; j++)
        {
            x = x0;
            y = y0;
            k = 0;
            
            do
            {
                x2 = x*x;
                y2 = y*y;
                y = 2*x*y+y0;
                x = x2-y2+x0;
                k++;
            }while((x2+y2 < 4) && (k < MAXIT));
            
            
            if(k == MAXIT)
            {  
                Graphics2D_SetColor((WORD)0xFFFF);    
                Graphics2D_PutPixel(j,i);
            }
           
            x0 += d;
        }
        y0 += d;
    }
}

int main(int argc, char** argv) 
{
    unsigned char text[32];
    unsigned char buffer;
    
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    SYSTEMConfigWaitStatesAndPB(SYS_FREQ);
    CheKseg0CacheOn();
    mCheConfigure(CHECON|0x30);
    mBMXDisableDRMWaitState();
    
    DisplayInterfaceInit();
    DisplayDriveInit();
    DisplaySetOrientation(DEGREES_270);
    Graphics2D_SetColor(0x0000);
    Graphics2D_fillDisplay();
    TRISAbits.TRISA6 = 0;
    PORTAbits.RA6 = 0;
   
    BluetoothInit();
   
    mandelBrot(-2.0,-2.0,4.0);
    
    while(1)
    {  
        LED_STATUS = !LED_STATUS;
        DelayMs(100);
    }
    return (EXIT_SUCCESS);
}

