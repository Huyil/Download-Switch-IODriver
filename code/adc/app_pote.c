#include "app_pote.h"
#include "app_motor.h"
#include "app_sleep.h"
#include "app_led.h"

uint16_t temp;
uint8_t levelDiff;
uint16_t adcMap[2]={20,4035};
uint8_t POT_Handle(uint16_t *pot,uint8_t len,uint8_t maxLv)
{
  #define SPACE 100
  static int8_t lastlevel = -1;
  uint8_t level;
  uint16_t middleValue;
  uint16_t minValue,maxValue;
  
  minValue = adcMap[0];
  maxValue = adcMap[1];
//  // ��pot�����������
//  if(len > 1){
//    for (uint8_t i = 0; i < len - 1; i++) {
//      for (uint8_t j = i + 1; j < len; j++) {
//        if (pot[j] < pot[i]) {
//          // ����ֵ
//          tempPot = pot[i];
//          pot[i] = pot[j];
//          pot[j] = tempPot;
//        }
//      }
//    }
//    middleValue = pot[len / 2];
//  }else{
//    middleValue = pot[0];
//  }
  middleValue = ADC_average(pot, len, minValue, maxValue);
  temp = map(middleValue,minValue,maxValue,0,SPACE*(maxLv));
  level = temp/SPACE;
  
  if(motor.speedlevel == 0)
  {
    levelDiff = 3;
  }else {
    if(level > motor.speedlevel)
    {
      levelDiff = level - motor.speedlevel;
    }else{
      levelDiff = motor.speedlevel - level;
    }
  }
  
   //��������Ч������߲ٿط�ʽ�����ı�ʱ�Ÿ��µ�λ
  if((temp % SPACE) < 65 || levelDiff > 2 || KEY_PW_READ() == 0)     
  {
    lastlevel = level;
    //motor_setLevel(level);
  }
  
  if(lastlevel <= 1)
  {
    lastlevel = 1;
  }else if(lastlevel > maxLv){
    lastlevel = maxLv;
  }
  return lastlevel;
}

#include "app_key.h"
void POT_CB(void *arg)
{
  static int32_t level;
  if(KEY_PW_READ() == 0)
  {
    level = 0;
    motor.speedlevel = 0;
    setSpeed(level);
    return;
  }
  
  level = POT_Handle(adc.POT,1,MAX_SPEED_LEVEL);
 
  
  if(motor.speedlevel != level){
    motor.speedlevel = level;
    KeyDir_Reload(1);
    LCD_level(SEG_REFRESH|level);
    setSpeed(level);
    Start_system();
    
  }
//  else {
//    setSpeed(level);
//  }
}




// �����ѹ��ʽ��صĳ���
#define R_PULLUP 51000  // �������� 51K��
#define R_SAMPLE 10000  // �������� 10K��
#define ADC_MAX_VALUE 4095  // 12λADC

// ��Ŀ���ѹת��Ϊ��Ӧ��ADC��ֵ
uint32_t VoltageToADC(uint32_t target_voltage_mV) {
    // ��ѹת����ʽ��Vadc = Vin * R_sample / (R_pullup + R_sample)
    uint32_t adc_value = (target_voltage_mV * R_SAMPLE) / (R_PULLUP + R_SAMPLE);
    
    // ȷ��ADCֵ���ᳬ�����ֵ
    if (adc_value > ADC_MAX_VALUE) {
        adc_value = ADC_MAX_VALUE;
    }
    
    return adc_value;
}

// ʹ��Q15��ʽ��16λ�з��Ŷ�������15λС����
#define Q15_SHIFT   15
#define Q15_MULT    (1 << Q15_SHIFT)

#define pidIntegral_MAX (200*Q15_MULT)
#define pidIntegral_MIN (-200*Q15_MULT)



PID_Controller pid;



// PID��ʼ����ʾ��������
void PID_Init(void) {
    pid.Target = VoltageToADC(5000);      // 5000mV = 5V
    pid.Kp = 0.0625 * Q15_MULT;   // Q15(0.5)=0x4000
    pid.Ki = 1;  // Q15(0.01)=0x0147
    pid.Kd = 0 * Q15_MULT;   // Q15(0.1)=0x0CCD
    pid.Integral = 0;
    pid.LastError = 0;
    pid.Output = 0;
    pid.IntegralMax = pidIntegral_MAX;
    pid.IntegralMin = pidIntegral_MIN;
    pid.OutputMax = CW_GTIM2->ARR-1;     // ��ӦARR=23999
    pid.OutputMin = 0;
}


void integralClear(void) 
{
  pid.Integral = 0;

}

// ����PID����
uint16_t PID_Calculate(int32_t current) 
{
    // �����㣨��λ����mV��
    int32_t error = pid.Target - current;

    // ������ P = Kp * error
    int32_t P = pid.Kp * error;

    // ������ I = Ki * ��error
    pid.Integral += error;
    // �����޷� �� ��ֹwindup ��
    if(pid.Integral > pidIntegral_MAX) pid.Integral = pidIntegral_MAX;
    if(pid.Integral < pidIntegral_MIN) pid.Integral = pidIntegral_MIN;
    int32_t I = pid.Ki*pid.Integral>> 8;

    // ΢���� D = Kd * (error - LastError)
    int32_t D = pid.Kd * (error - pid.LastError) ;
    pid.LastError = error;

    // �����
    pid.Output += (P + I + D)>> Q15_SHIFT;  // λ��ʽPID

    // ����޷�
    if(pid.Output > pid.OutputMax) pid.Output = pid.OutputMax;
    if(pid.Output < pid.OutputMin) pid.Output = pid.OutputMin;

    return pid.Output;
}


// ����ѭ��
void Control_Loop(void) 
{
  
  #if(PWM_MODE==0)
  static uint8_t once;
  if(sys.cfg.sw == POWER_OFF)
  {
    VimSet_Stop();
    once = 0;
  }else{
    if(once == 0)
    {
      once = 1;
      Vim_Pulse_Set(0);
      VimSet_PWM();
    }
    uint16_t pwm_duty = 0;
    if(motor.Scfg.vol == 0)
      pid.Target = 0;
    else
      pid.Target = motor.Scfg.lastvol;
    if(pid.Target)
    {
      // ����PID���
      if(adc.aveVol < 50)
      {
        if(motor.ovTick < 10)
        {
          motor.ovTick++;
          pwm_duty = PID_Calculate(adc.aveVol);
        }else {
          pwm_duty = 0;
        }
      }else 
      {
        motor.ovTick = 0;
        pwm_duty = PID_Calculate(adc.aveVol);
      }
    }
    // ����PWMռ�ձ�
    Vim_Pulse_Set(pwm_duty);
    
//    if(motor.Scfg.lastvol != motor.Scfg.vol)
//    {
//      if(pid.LastError > 0)
//      {
//        if(pid.LastError < 20)
//         motor.Scfg.lastvol = motor.Scfg.vol;
//      }else if(pid.LastError)
//      {
//        if(pid.LastError > -20)
//         motor.Scfg.lastvol = motor.Scfg.vol;
//      }
//    }
  }
  #endif
}

void GTIM2_IRQHandler(void)
{
  static uint8_t once;
  if(sys.cfg.sw == POWER_OFF)
  {
    VimSet_Stop();
    once = 0;
  }else{
    if(once == 0)
    {
      once = 1;
      Vim_Pulse_Set(0);
      VimSet_PWM();
    }
    Control_Loop();
  }
  
  CW_GTIM2->ICR = 0;
}
