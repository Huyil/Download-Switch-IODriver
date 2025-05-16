#include "bsp_adc.h"
#include "cw32l011_sysctrl.h"


// 电阻分压公式相关的常量
#define R_PULLUP 1000  // 上拉电阻 1000KΩ
#define R_SAMPLE 499   // 采样电阻 499K

ADCBUF adc;

/**
 * @brief ADC 单次转换，转换结束后触发中断
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
  ADC_ITConfig(ADC_IT_EOS, ENABLE);   //EOC:单次;  EOS:序列
  ADC_Enable();
  //ADC_SoftwareStartConvCmd(ENABLE);
  //(1200*4095)/1228=4000mv
  adc.bgr = (*((uint16_t*)0x001007D2));
  adc.FVBG = (uint32_t)(R_SAMPLE + R_PULLUP) * adc.bgr / R_SAMPLE; // 单位为 mV 预估值为 2502.128
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
  // 用第一个有效元素初始化 max 和 min
  uint16_t max = 0;
  uint16_t min = 0;
  uint32_t sum = 0;
  uint16_t ave_value;

  if(len == 0)
  {
    // 处理元素数量不足的情况，根据应用需求选择适当的操作
    ave_value = 0;  // 将 BAT_value 默认为 500，可以根据需要更改
  }
  else if(len < 2)
  {
    ave_value = *bat;
  }
  else
  {
    // 有效值（>= 1000）的计数
    uint8_t validCount = 0;
    // 标志以检查是否找到至少一个有效值
    uint8_t foundValidValue = 0;

    for(uint8_t i = 0; i < len; i++)
    {
      // 过滤掉小于 500 的值
      if(bat[i] >= minValue && bat[i] <= maxValue)
      {
        // 只有在找到第一个有效值之前不更新 max 和 min
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

    // 检查是否找到至少两个有效值
    if(validCount <= 2)
    {
      // 处理有效值不足的情况，根据应用需求选择适当的操作
      //return 500;  // 将 ave_value 默认为 500，可以根据需要更改
      ave_value = max > minValue ? max : *bat;
    }
    else
    {
      // 从 sum 中减去 max 和 min
      sum = sum - max - min;
      // 计算平均值
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
  // 确保输入值在原始范围内
  if(x <= in_min)
  {
    return out_min;
  }
  else if(x >= in_max)
  {
    return out_max;
  }

  // 进行映射计算，并转换为 uint16_t 类型
  return (uint16_t)(((uint32_t)(x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min);
}
