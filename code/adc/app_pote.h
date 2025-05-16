#ifndef __APP_POTE_H__
#define __APP_POTE_H__

#include "bsp_adc.h"

typedef struct {
    int32_t Target;     // 目标电压（单位：mV）
    int32_t Kp;         // Q15格式的比例系数
    int32_t Ki;         // Q15格式的积分系数
    int32_t Kd;         // Q15格式的微分系数
    int32_t Integral;   // Q15格式积分项
    int32_t LastError;  // 上次误差（Q15）
    int32_t Output;     // PWM占空比（实际值）
    int32_t IntegralMax;//积分项 最大值
    int32_t IntegralMin;//积分项 最小值
    int32_t OutputMax;  // PWM最大值
    int32_t OutputMin;  // PWM最小值
} PID_Controller;

uint8_t POT_Handle(uint16_t *pot,uint8_t len,uint8_t maxLv);
void POT_CB(void *arg);
void PID_Init(void);
void Control_Loop(void);
void integralClear(void);
#endif 

