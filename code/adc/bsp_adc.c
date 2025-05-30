#include "bsp_adc.h"
#include "cw32l011_sysctrl.h"


// �����ѹ��ʽ��صĳ���
#define R_PULLUP 1000  // �������� 1000K��
#define R_SAMPLE 499   // �������� 499K

ADCBUF adc;

/**
 * @brief ADC ����ת����ת�������󴥷��ж�
 *
 */
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  __SYSCTRL_ADC_CLK_ENABLE();
  ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div8;    //div8:1MSPS
  ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Once;
  ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to1;
  //
  ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputCH7;  //BAT
  ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime390Clk;
  ADC_InitStructure.ADC_IN1.ADC_InputChannel = ADC_InputVref1P2;  //BAT
  ADC_InitStructure.ADC_IN1.ADC_SampTime = ADC_SampTime390Clk;
  ADC_Init(&ADC_InitStructure);
  ADC_ExtTrigCfg(ADC_TRIG_ATIMTRGO, ENABLE);
  ADC_ClearITPendingAll();    //CW_ADC->ICR &= 0xFFF0;
  ADC_ITConfig(ADC_IT_EOS, ENABLE);   //EOC:����;  EOS:����
  ADC_Enable();
  //ADC_SoftwareStartConvCmd(ENABLE);
  //(1200*4095)/1228=4000mv
  adc.bgr = (*((uint16_t*)0x001007D2));
  adc.FVBG = (uint32_t)(R_SAMPLE + R_PULLUP) * adc.bgr / R_SAMPLE; // ��λΪ mV Ԥ��ֵΪ 2502.128
}

uint8_t ave = 4;


void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN */
  if(CW_ADC->ISR & ADC_ISR_EOS_Msk)
  {
    //ADC_SoftwareStartConvCmd(DISABLE);
    if(adc.conts < 9)
      adc.conts++;
    else adc.conts = 0;

    //adc.RES[adc.conts%5] = CW_ADC->RESULT0;
    adc.BAT[adc.conts] = CW_ADC->RESULT0;
    adc.VBG[adc.conts] = CW_ADC->RESULT1;
    //ADC_SoftwareStartConvCmd(ENABLE);
  }

  CW_ADC->ICR = 0;
  //    ADC_SoftwareStartConvCmd(ENABLE);
  /* USER CODE END */
}

uint16_t ADC_average(uint16_t* bat, uint8_t len, uint16_t minValue, uint16_t maxValue)
{
  // �õ�һ����ЧԪ�س�ʼ�� max �� min
  uint16_t max = 0;
  uint16_t min = 0;
  uint32_t sum = 0;
  uint16_t ave_value;

  if(len == 0)
  {
    // ����Ԫ��������������������Ӧ������ѡ���ʵ��Ĳ���
    ave_value = 0;  // �� BAT_value Ĭ��Ϊ 500�����Ը�����Ҫ����
  }
  else if(len < 2)
  {
    ave_value = *bat;
  }
  else
  {
    // ��Чֵ��>= 1000���ļ���
    uint8_t validCount = 0;
    // ��־�Լ���Ƿ��ҵ�����һ����Чֵ
    uint8_t foundValidValue = 0;

    for(uint8_t i = 0; i < len; i++)
    {
      // ���˵�С�� 500 ��ֵ
      if(bat[i] >= minValue && bat[i] <= maxValue)
      {
        // ֻ�����ҵ���һ����Чֵ֮ǰ������ max �� min
        if(!foundValidValue)
        {
          max = bat[i];
          min = bat[i];
          foundValidValue = 1;
        }
        else
        {
          if(bat[i] > max)
          {
            max = bat[i];
          }

          if(bat[i] < min)
          {
            min = bat[i];
          }
        }

        sum += bat[i];
        validCount++;
      }
    }

    // ����Ƿ��ҵ�����������Чֵ
    if(validCount <= 2)
    {
      // ������Чֵ��������������Ӧ������ѡ���ʵ��Ĳ���
      //return 500;  // �� ave_value Ĭ��Ϊ 500�����Ը�����Ҫ����
      ave_value = max > minValue ? max : *bat;
    }
    else
    {
      // �� sum �м�ȥ max �� min
      sum = sum - max - min;
      // ����ƽ��ֵ
      ave_value = (sum / (validCount - 2));
    }
  }

  return ave_value;
}


uint8_t adc_Avbg2vref(uint16_t buf[10], uint8_t len)
{
  adc.aveVBG = ADC_average(buf, len, 900, 2000);

  if(adc.aveVBG < 950 || adc.aveVBG > 1000)
  {
    if(adc.aveVBG > 1600)
    {
      adc.vref =  3000;
    }
    else if(adc.aveVBG > 1400)
    {
      adc.vref =  3300;
    }
    else
    {
      adc.vref =  5000;
    }

    return 1;
  }
  else
  {
    adc.vref = (adc.bgr << 12) / adc.aveVBG;
  }

  return 0;
}

uint16_t adc_Abat2bat(uint16_t Abat)
{
//  if(adc.aveVBG < 950 || adc.aveVBG > 1000)
//  {
//    return (Abat * adc.FVBG) / 9627;   //VREF 4980mV
//  }
//  else
  {
    return (Abat * adc.FVBG) / adc.aveVBG / 10;
  }
}


uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
  // ȷ������ֵ��ԭʼ��Χ��
  if(x <= in_min)
  {
    return out_min;
  }
  else if(x >= in_max)
  {
    return out_max;
  }

  // ����ӳ����㣬��ת��Ϊ uint16_t ����
  return (uint16_t)(((uint32_t)(x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min);
}
