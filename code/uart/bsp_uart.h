#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "cw32l011_uart.h"
#include "stdio.h"

#define MODE_SWD  0
#define MODE_UART 1
#define MODE_I2C  2

#define MAX_LEN 64

typedef struct {
  uint8_t  buf[MAX_LEN];
  uint8_t  count;
  uint8_t  len;
}UART_OneBUF;

typedef struct {
  UART_OneBUF tx;
  UART_OneBUF rx;
  UART_OneBUF com; 
  uint8_t okflag;
  uint8_t txBusy;
  uint8_t writeflag;
}UART_BUF;

extern UART_BUF uart;
void Uart_Config(void);
void Uart_SWD_Switch(uint8_t swd);

#endif
