#ifndef _general_h_
#define _general_h_

#define TIM4_UPD_OVF_IRQ
#define ADC1_IRQ
#define UART1_RX_IRQ
//#define DEBUG 

#include "stm8s_conf.h"
#include "stm8s_uart1.h"
#include "stm8s_adc1.h"
#include "stm8s_flash.h"
#include <cmath>
//Definitions EEPROM regions for constants
extern uint32_t PRESCALER_1;
extern uint32_t PRESCALER_2;
extern uint32_t PRESCALER_3;
extern uint32_t CONFIGURATION;
//User constant
extern uint8_t u8CountMeasure;
extern uint8_t u8BuffMeasure[100U];
//extern uint8_t u8CurrentChannel;
extern uint8_t u8CurrentConfigurateADC;
extern uint8_t u8LastChannel;
extern uint8_t u8UsedChannel_1;
extern uint8_t u8UsedChannel_2;
extern uint8_t u8UsedChannel_3;
struct ChannelSet{
  uint8_t u8SubChannel;
  double u8Prescaler_1;
  double u8Prescaler_2;
};
typedef struct ChannelSet CS;
extern CS CH1, CH2, CH3;
//Calibrate
extern double bCalibratingCoefficient[6];
//FSM for rules device
enum FSM{
  select_mode = 0,
  prescaler_mode = 1,
  wait = 2, 
  save = 3,
  calibrate = 4, 
  subprescaler = 5
};
extern enum FSM MAIN;
enum action{
  prescaler = 0,
  select = 1
};
//User procedures
//Init functions
void vInitCLK(void);
void vInitADC(void);
void vInitUART(void);
void vInitTIM4(void);
void vInitGPIO(void);
//ADC functions
void vSelectChannel(uint8_t channel);
uint8_t u8GetMean(uint8_t* data, uint8_t u8Channel);
//UART functions
bool vUART_Transmit(uint8_t data);
bool vUART_ArrayTransmit(uint8_t* data, uint8_t size);
uint8_t u8UART_Recieve(void);
void vUART_SendResult(uint8_t u8Channel,uint8_t u8Result);
//EEPROM
void    vUpdateEEPROMConfig(uint8_t u8NumberConfig);
void    vUpdateEEPROMChannel(uint8_t u8Channel, uint8_t u8Prescaler);
uint8_t u8GetPrescaler(uint8_t u8Channel);
void    vUploadValueEEPROM(uint8_t* pPrescaler_1, uint8_t* pPrescaler_2, uint8_t* pPrescaler_3, uint8_t* pMode);
//General logic functions
enum action eGetAction(void);
void vSelectChannel(uint8_t);
//Multiplexing
void vGetMultiplex(uint8_t* u8LastChannel, uint8_t u8MultiplexRule);
void vSetRulesMul(uint8_t* pu8RulseVariable);
//Prescaling
void vSetPrescaler(uint8_t u8Channel, uint8_t u8Prescaler);
//Calibrated
void vGetCalibrate(uint8_t u8ChannelNum);
double dGetCalibratingCoefficient(uint8_t u8Channel, uint8_t u8Subnumber);
void   vExportData(void);
void vSelechSub(uint8_t u8Channel, uint8_t u8Subchannel);
#endif 

