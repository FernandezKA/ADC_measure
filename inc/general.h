#ifndef _general_h_
#define _general_h_

#define TIM4_UPD_OVF_IRQ
#define ADC1_IRQ
//#include <stdint.h>
//#include <stdbool.h>
#include "stm8s_conf.h"
#include "stm8s_uart1.h"
#include "stm8s_adc1.h"
//User constant
extern uint8_t u8CountMeasure;
extern uint8_t u8BuffMeasure[100U];
extern uint8_t u8CurrentChannel;
//User procedures
//Init functions
void vInitCLK(void);
void vInitADC(void);
void vInitUART(void);
void vInitTIM4(void);
void vInitGPIO(void);
//ADC functions
void vSelectChannel(uint8_t channel);
uint8_t u8GetMean(uint8_t* data);
//UART functions
bool vUART_Transmit(uint8_t data);
bool vUART_ArrayTransmit(uint8_t* data, uint8_t size);
#endif

