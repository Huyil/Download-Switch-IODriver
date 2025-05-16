#ifndef __app_io_H__
#define __app_io_H__

#include "bsp_gpio.h"

enum CHxMode {
   _NC = 0,
   GND = 1,
   SCK = 2,
   SDA = 3,
   VCC = 4,
   MODE_num,
};

enum CHx {
   CH1 = 0,
   CH2,
   CH3,
   CH4,
   CH_num,
};

typedef struct {
  enum CHxMode mode : 3;
  uint8_t pin   : 2;
}io_one;

#pragma anon_unions
typedef struct {
  union{
    struct{
      uint32_t ch1 : 3;
      uint32_t io1 : 2;
      uint32_t dio : 3;
      
      uint32_t ch2 : 3;
      uint32_t io2 : 2;
      uint32_t clk : 3; 
      
      uint32_t ch3 : 3;
      uint32_t io3 : 2;
      uint32_t : 3;
      
      uint32_t ch4 : 3;
      uint32_t io4 : 2;
      uint32_t en_out:1;
      uint32_t en_5V:1;
    };
    
    struct{
      uint32_t  : 3;
      uint32_t P1 : 1;
      uint32_t N1 : 1;
      uint32_t DIO_EN : 1;
      uint32_t DIO_A : 1;
      uint32_t DIO_B : 1;
      
      uint32_t  : 3;
      uint32_t P2 : 1;
      uint32_t N2 : 1;
      uint32_t CLK_EN : 1;
      uint32_t CLK_A : 1;
      uint32_t CLK_B : 1;
      
      uint32_t  : 3;
      uint32_t P3 : 1;
      uint32_t N3 : 1;
      uint32_t : 3;
      
      uint32_t  : 3;
      uint32_t P4 : 1;
      uint32_t N4 : 1;
    }io;
    uint32_t ioset;
    io_one ch[4];
  };
  
  union{
    struct {
      uint8_t key1 : 1;
      uint8_t key2 : 1;
      uint8_t key3 : 1;
      uint8_t key4 : 1;
      uint8_t key5 : 1;

    };
    struct {
      uint8_t key : 5;
    };
  };
  
  union{
    struct {
      uint16_t batAdc : 10;
      uint16_t batLv: 4;
      uint16_t chrg : 1;
      uint16_t done : 1;
    };
    struct {
      uint16_t bat;
    };
  };
}SWITCH_STATE;

extern SWITCH_STATE io;

int8_t IO_Refresh(void);
int8_t IO_set(enum CHx _ch,enum CHxMode _mode);
int8_t IO_get(enum CHx _ch,io_one* _io);

#endif //__app_io_H__


