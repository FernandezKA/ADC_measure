#include "general.h"
//User variables
uint8_t u8BuffMeasure[64U];
//Main function
//TODO: Set frequency of sampling, get value each IRQ for OVF Tim4
void SysInit(void){
    vInitCLK();
    vInitUART();
    vInitADC();
    vInitTIM4();
}

void main(void)
{
        SysInit();
	while (1){
        UART1->DR = 0x64;
        };
}

