#ifndef _general_h_
#define _general_h_

#define TIM4_UPD_OVF_IRQ
#define ADC1_IRQ
#define UART1_RX_IRQ
#define SAMPLES 200
//#define DEBUG 

#include "stm8s_conf.h"
#include "stm8s_uart1.h"
#include "stm8s_adc1.h"
#include "stm8s_flash.h"
#include <cmath>
//Definitions EEPROM regions for constants
extern uint32_t CONFIGURATION;
extern uint32_t OUTPUT;
extern uint32_t PRESCALER_1;
extern uint32_t PRESCALER_2;
extern uint32_t PRESCALER_3;
//User constant
extern uint8_t u8CountMeasure;
extern uint8_t u8BuffMeasure[SAMPLES];
//extern uint8_t u8CurrentChannel;
extern uint8_t u8CurrentConfigurateADC;
extern uint8_t u8LastChannel;
extern uint8_t u8UsedChannel_1;
extern uint8_t u8UsedChannel_2;
extern uint8_t u8UsedChannel_3;
extern double CH1_Last, CH2_Last, CH3_Last;
struct ChannelSet{
  uint8_t u8SubChannel;
  double u8Prescaler_1;
  double u8Prescaler_2;
};
typedef struct ChannelSet CS;
extern CS CH1, CH2, CH3;
//Calibrate
extern double bCalibratingCoefficient[6];
extern bool shortOut;
//FSM for rules device
enum FSM{
  select_mode = 0,
  wait, 
  calibrate, 
  subprescaler, 
  help, 
  out, 
  prescalers
};
enum OutMode{
  short_out, 
  long_out, 
  full_out
};
extern enum OutMode OutModeVar;
extern enum FSM MAIN;
/*enum action{
  prescaler = 0,
  select = 1
};*/
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
void getBackup(uint8_t* w_mode, enum OutMode* o_mode, uint8_t* SCH1, uint8_t* SCH2, uint8_t* SCH3);
void getRestore(uint8_t* w_mode, enum OutMode* o_mode, uint8_t* SCH1, uint8_t* SCH2, uint8_t* SCH3);
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
//Input data
uint8_t u8GetDigit(void);
void vPrintHelp(void);
#endif 

