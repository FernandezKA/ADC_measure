#ifndef _general_h_
#define _general_h_
//#include <stdint.h>
//#include <stdbool.h>
#include "stm8s_conf.h"
#include "stm8s_uart1.h"
//User procedures
void vInitCLK(void);
void vInitADC(void);
void vInitUART(void);
void vInitTIM4(void);
#endif