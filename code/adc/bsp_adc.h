
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__
/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "cw32l011_adc.h"


typedef struct{
  uint8_t  conts;
  uint16_t RES[1];              //电机电流ADC值
  uint16_t BAT[10];
  uint16_t VBG[10];          //1.2v参考源
  uint16_t POT[1];
  
  uint16_t newVol;
  uint16_t aveVol;
  
  uint8_t  errtick;
  uint16_t aveVBG;
  uint32_t vref;
  uint32_t bgr;
  uint32_t FVBG;               //电池电压系数
}ADCBUF;

extern ADCBUF adc;

void ADC_Configuration(void);
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
uint16_t ADC_average(uint16_t *bat,uint8_t len,uint16_t minValue,uint16_t maxValue);

uint8_t  adc_Avbg2vref(uint16_t buf[10], uint8_t len);
uint16_t adc_Abat2bat(uint16_t Abat);


#endif /* __DEMO_ADC_H__ */



