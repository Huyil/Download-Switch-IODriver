#ifndef __APP_BATTERY_H
#define __APP_BATTERY_H

#include "stdint.h"

#define MAX_BAT_ADC 3500
#define MIN_BAT_ADC 1900

typedef struct {
  uint16_t value;
  int16_t  level;
}THRESHOLD_ONE;

typedef struct{
  //uint16_t buf[10];
  uint8_t  count;
  
  uint16_t value;
  int16_t  level;
  int16_t  lastlv;
  int16_t  savelevel;
  int16_t tempLv;
  int8_t valueBigNum;
}BAT_BUF;

extern BAT_BUF bat;

uint8_t bat_Ref(void);
void bat_handler(void* arg);
int16_t bat_value2LevelDefault(uint16_t value);
uint16_t bat_middAverage(uint16_t buf[], uint8_t len);
#endif

