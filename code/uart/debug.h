#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "bsp_uart.h"
//
//#define _DEBUG_
#ifdef 	_DEBUG_

//#define debug(format, ...) printf("->"format, ##__VA_ARGS__)
#define debug(format, ...) printf(format, ##__VA_ARGS__)

#else
#define debug(format, ...) 
#endif

void DEBUGCB(void *arg);
#endif

