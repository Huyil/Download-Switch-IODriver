#ifndef __APP_POTE_H__
#define __APP_POTE_H__

#include "bsp_adc.h"

typedef struct {
    int32_t Target;     // Ŀ���ѹ����λ��mV��
    int32_t Kp;         // Q15��ʽ�ı���ϵ��
    int32_t Ki;         // Q15��ʽ�Ļ���ϵ��
    int32_t Kd;         // Q15��ʽ��΢��ϵ��
    int32_t Integral;   // Q15��ʽ������
    int32_t LastError;  // �ϴ���Q15��
    int32_t Output;     // PWMռ�ձȣ�ʵ��ֵ��
    int32_t IntegralMax;//������ ���ֵ
    int32_t IntegralMin;//������ ��Сֵ
    int32_t OutputMax;  // PWM���ֵ
    int32_t OutputMin;  // PWM��Сֵ
} PID_Controller;

uint8_t POT_Handle(uint16_t *pot,uint8_t len,uint8_t maxLv);
void POT_CB(void *arg);
void PID_Init(void);
void Control_Loop(void);
void integralClear(void);
#endif 

