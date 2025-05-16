#include "bsp_timer.h"
#include "bsp_gpio.h"



void ATIM_OCxInit(uint8_t ch, MY_ATIM_OCInitTypeDef *TIM_OCInitStruct)
{
  OCInit_TypeDef OCInit_Buf;

  assert_param(IS_ATIM_OC_MODE(TIM_OCInitStruct->OCMode));
  assert_param(IS_ATIM_OC_FAST_MODE(TIM_OCInitStruct->OCFastMode));
  assert_param(IS_FUNCTIONAL_STATE(TIM_OCInitStruct->BufferState));
  
  assert_param(IS_FUNCTIONAL_STATE(TIM_OCInitStruct->OCOutput));
  assert_param(IS_FUNCTIONAL_STATE(TIM_OCInitStruct->OCNOutput));
  assert_param(IS_ATIM_OC_POLARITY(TIM_OCInitStruct->OCPolarity));
  assert_param(IS_ATIM_OC_POLARITY(TIM_OCInitStruct->OCNPolarity));
  
  switch(ch)
  {
    case 1:case 2:OCInit_Buf.CCMRxCMP = CW_ATIM->CCMR1CMP;break;
    case 3:case 4:OCInit_Buf.CCMRxCMP = CW_ATIM->CCMR1CMP;break;
    case 5:case 6:OCInit_Buf.CCMRxCMP = CW_ATIM->CCMR1CMP;break;
  }
  
  if(ch&0x01)
  {
    OCInit_Buf.CCMRxCMP_f.CCxS = 0;
    OCInit_Buf.CCMRxCMP_f.OCxFE = TIM_OCInitStruct->OCFastMode;
    OCInit_Buf.CCMRxCMP_f.OCxPE = TIM_OCInitStruct->BufferState;    //输出比较1预装载使能
    OCInit_Buf.CCMRxCMP_f.OCxM =  TIM_OCInitStruct->OCMode & 0x07;
    OCInit_Buf.CCMRxCMP_f.OCxMH = TIM_OCInitStruct->OCMode >> 3;
  }else{
    OCInit_Buf.CCMRxCMP_f.CCyS = 0;
    OCInit_Buf.CCMRxCMP_f.OCyFE = TIM_OCInitStruct->OCFastMode;
    OCInit_Buf.CCMRxCMP_f.OCyPE = TIM_OCInitStruct->BufferState;
    OCInit_Buf.CCMRxCMP_f.OCyM =  TIM_OCInitStruct->OCMode & 0x07;
    OCInit_Buf.CCMRxCMP_f.OCyMH = TIM_OCInitStruct->OCMode >> 3;
  }

  switch(ch)
  {
    case 1:
      CW_ATIM->CCER_f.CC1P =  TIM_OCInitStruct->OCPolarity;
      CW_ATIM->CCER_f.CC1E = TIM_OCInitStruct->OCOutput;

      CW_ATIM->CCER_f.CC1NP =  TIM_OCInitStruct->OCNPolarity;
      CW_ATIM->CCER_f.CC1NE = TIM_OCInitStruct->OCNOutput;
      CW_ATIM->CCR1 = TIM_OCInitStruct->Pulse;
    
      CW_ATIM->CCMR1CAP = OCInit_Buf.CCMRxCMP;
    break;
    case 2:
      CW_ATIM->CCER_f.CC2P =  TIM_OCInitStruct->OCPolarity;
      CW_ATIM->CCER_f.CC2E = TIM_OCInitStruct->OCOutput;

      CW_ATIM->CCER_f.CC2NP =  TIM_OCInitStruct->OCNPolarity;
      CW_ATIM->CCER_f.CC2NE = TIM_OCInitStruct->OCNOutput;
      CW_ATIM->CCR2 = TIM_OCInitStruct->Pulse;
    
      CW_ATIM->CCMR1CAP = OCInit_Buf.CCMRxCMP;
    break;
    case 3:
      CW_ATIM->CCER_f.CC3P =  TIM_OCInitStruct->OCPolarity;
      CW_ATIM->CCER_f.CC3E = TIM_OCInitStruct->OCOutput;

      CW_ATIM->CCER_f.CC3NP =  TIM_OCInitStruct->OCNPolarity;
      CW_ATIM->CCER_f.CC3NE = TIM_OCInitStruct->OCNOutput;
      CW_ATIM->CCR3 = TIM_OCInitStruct->Pulse;
    
      CW_ATIM->CCMR2CAP = OCInit_Buf.CCMRxCMP;
    break;
    case 4:
      CW_ATIM->CCER_f.CC4P =  TIM_OCInitStruct->OCPolarity;
      CW_ATIM->CCER_f.CC4E = TIM_OCInitStruct->OCOutput;

      CW_ATIM->CCER_f.CC4NP =  TIM_OCInitStruct->OCNPolarity;
      CW_ATIM->CCER_f.CC4NE = TIM_OCInitStruct->OCNOutput;
      CW_ATIM->CCR4 = TIM_OCInitStruct->Pulse;
    
      CW_ATIM->CCMR2CAP = OCInit_Buf.CCMRxCMP;
    break;
    case 5:
      CW_ATIM->CCER_f.CC5P =  TIM_OCInitStruct->OCPolarity;
      CW_ATIM->CCER_f.CC5E = TIM_OCInitStruct->OCOutput;

      CW_ATIM->CCER_f.CC5NP =  TIM_OCInitStruct->OCNPolarity;
      CW_ATIM->CCER_f.CC5NE = TIM_OCInitStruct->OCNOutput;
      CW_ATIM->CCR5 = TIM_OCInitStruct->Pulse;
    
      CW_ATIM->CCMR3CAP = OCInit_Buf.CCMRxCMP;
    break;
    case 6:
      CW_ATIM->CCER_f.CC6P =  TIM_OCInitStruct->OCPolarity;
      CW_ATIM->CCER_f.CC6E = TIM_OCInitStruct->OCOutput;

      CW_ATIM->CCER_f.CC6NP =  TIM_OCInitStruct->OCNPolarity;
      CW_ATIM->CCER_f.CC6NE = TIM_OCInitStruct->OCNOutput;
      CW_ATIM->CCR6 = TIM_OCInitStruct->Pulse;
    
      CW_ATIM->CCMR3CAP = OCInit_Buf.CCMRxCMP;
    break;
  }
}


void ATIM_Configuration(void)
{
  __SYSCTRL_ATIM_CLK_ENABLE();
  
  ATIM_InitTypeDef ATIM_InitStruct;
  
  CW_ATIM->BDTR_f.BK2E = DISABLE;
  CW_ATIM->BDTR_f.BK2P = DISABLE;
  CW_ATIM->BDTR_f.BK2F = DISABLE;
  CW_ATIM->BDTR_f.OSSI = DISABLE;     //未运行时 0:高阻 1:拉低
  CW_ATIM->BDTR_f.OSSR = DISABLE;      //运行时, 0:未开启的高阻 1:拉低
  CW_ATIM->BDTR_f.BKE  = DISABLE;
  CW_ATIM->BDTR_f.BKP  = 0;
  CW_ATIM->BDTR_f.BKF  = 0;
  CW_ATIM->BDTR_f.LOCK = 0;
  CW_ATIM->BDTR_f.AOE  = DISABLE;
  CW_ATIM->BDTR_f.DTG  = 0;

  ATIM_InitStruct.BufferState = ENABLE;                               //使能缓存寄存器
  ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_ALIGN_MODE_EDGE;    //边沿对齐
#if (PWM_MODE)
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_DOWN;              //向下计数；
#else
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;              //向上计数；
#endif
  ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;            //连续运行模式
  ATIM_InitStruct.Prescaler = 0;                                      //不分频
  ATIM_InitStruct.ReloadValue = PWM_MAX;                              //重载周期2400+1
  ATIM_InitStruct.RepetitionCounter = 24;                             //重复周期(触发adc)
  ATIM_Init(&ATIM_InitStruct);
 
  /*
  
  MY_ATIM_OCInitTypeDef OCInit;
  OCInit.BufferState  = ENABLE;                     //比较缓存
  OCInit.OCFastMode   = DISABLE;   
  OCInit.OCInterruptState = DISABLE;                //比较匹配触发中断
  OCInit.OCMode = ATIM_OCMODE_PWM1;
  
  OCInit.OCOutput    = ENABLE;    //启动输出
  OCInit.OCPolarity  = DISABLE;   //不反转
  OCInit.OCNOutput   = DISABLE;   //关闭输出
  OCInit.OCNPolarity = DISABLE;   //不反转
  OCInit.Pulse        = 0;        //比较值
  ATIM_OCxInit(1,&OCInit);
  ATIM_OCxInit(2,&OCInit);
//  OCInit.Pulse        = 2300;        //比较值
//  ATIM_OCxInit(3,&OCInit);

//  ATIM_MasterModeTrgo1Config(ATIM_MASTER_OUTPUT_CH4);
//  OCInit.BufferState = DISABLE;                     //比较缓存
//  OCInit.OCMode      = ATIM_OCMODE_PWM1;
//  OCInit.Pulse       = 0;        //比较值
//  OCInit.OCOutput    = ENABLE;    //启动输出
//  OCInit.OCNOutput   = ENABLE;
//  OCInit.OCPolarity  = ENABLE;    //反转
//  OCInit.OCNPolarity = DISABLE;   //不反转
//  ATIM_OCxInit(1,&OCInit);
//  ATIM_OCxInit(2,&OCInit);


//  ATIM_OCInitTypeDef ATIM_OCInitStruct;
//  ATIM_OCInitStruct.BufferState = DISABLE;              //使能缓存寄存器
//  ATIM_OCInitStruct.OCComplement = DISABLE;             //关闭反向输出
//  ATIM_OCInitStruct.OCFastMode = DISABLE;
//  ATIM_OCInitStruct.OCInterruptState = DISABLE;
//  ATIM_OCInitStruct.OCMode = ATIM_OCMODE_PWM1;
//  ATIM_OCInitStruct.OCPolarity = ATIM_OCPOLARITY_NONINVERT;
//  ATIM_OC1Init(&ATIM_OCInitStruct);
//  ATIM_OC5Init(&ATIM_OCInitStruct);
  
  
//  ATIM_OCInitStruct.OCPolarity = ATIM_OCPOLARITY_INVERT;
//  ATIM_OCInitStruct.OCComplement = ENABLE;             //关闭反向输出
//  ATIM_OC2Init(&ATIM_OCInitStruct);
  
  CW_ATIM->CCER_f.CC1E = SET;     //输出
  CW_ATIM->CCER_f.CC2E = SET;     //输出
  CW_ATIM->CCER_f.CC3E = SET;     //输出
  
  CW_ATIM->CNT = 1899;
  CW_ATIM->ICR = 0;   //清中断
  //ATIM_ITConfig(ATIM_IT_UIE, ENABLE);

  //ATIM_CtrlPWMOutputs(ENABLE);
  */
  
  
  //ATIM_MasterModeTrgo1Config(ATIM_MASTER_OUTPUT_CH1);
  ATIM_MasterModeTrgo1Config(ATIM_MASTER_OUTPUT_UEV); //更新 ADC触发
  ATIM_Cmd(ENABLE);
  
}


void GTIM_Configuration(void)
{
  GTIM_InitTypeDef GTIM_InitStruct;
  GTIM_OCModeCfgTypeDef GTIM_OCModeCfgStruct;
  
  //__SYSCTRL_GTIM1_CLK_ENABLE();
#if (PWM_MODE==0)
  __SYSCTRL_GTIM2_CLK_ENABLE();

  GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;   //边沿计数(锯齿)
  GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_EN;    //开机缓冲
  GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;      //向上计数
  GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;    //仅计数值溢出 生成更新中断  
  GTIM_InitStruct.Prescaler = 0;                     //1分频
  GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;   //单脉冲关闭
  GTIM_InitStruct.ReloadValue = 48000/20;             //-1重载值 2K
  GTIM_InitStruct.UpdateOption = GTIM_UPDATE_EN;
  GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct);
  
  GTIM_OCModeCfgStruct.FastMode = ENABLE;
  GTIM_OCModeCfgStruct.OCMode = GTIM_OC_MODE_PWM1;
  GTIM_OCModeCfgStruct.OCPolarity = GTIM_OC_POLAR_NONINVERT;
  GTIM_OCModeCfgStruct.PreloadState = ENABLE;
  GTIM_OC4ModeCfg(CW_GTIM2, &GTIM_OCModeCfgStruct);
  
  GTIM_SetCompare4(CW_GTIM2, 0); 
  
  CW_GTIM2->CCER_f.CC4E = SET;     //输出
  CW_GTIM2->CNT = 0;
  CW_GTIM2->ICR = 0;
  //GTIM_ITConfig(CW_GTIM2,GTIM_IT_UPDATA,ENABLE);//|GTIM_IT_CC3
  GTIM_Cmd(CW_GTIM2, ENABLE);
  
  PB07_AFx_GTIM2CH4();
#endif
  
  //GTIM_OC1Cmd(CW_GTIM1, ENABLE);
  //GTIM_OC2Cmd(CW_GTIM1, ENABLE);
  //GTIM_Cmd(CW_GTIM1, ENABLE);

/*
  GTIM_ICModeCfgTypeDef GtimICCfg;
  GtimICCfg.ICPolarity = GTIM_ICPolarity_Rising;  //上升沿
  GtimICCfg.ICMap = GTIM_IC_MAP_TRC;              //映射到内部触发输入
  GtimICCfg.ICFilter = GTIM_IC_PRESCALER_DIV1;
  GtimICCfg.ICPrescaler = GTIM_IC_FILTER_PCLK_NONE;
  GTIM_IC1ModeCfg(CW_GTIM2,&GtimICCfg);

  GTIM_InitStruct.Prescaler = 0;                    //不分频
  GTIM_InitStruct.ReloadValue = 10;                 //0:重载值
  GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct);
  
  GTIM_SlaverCfgTypeDef GTIMsCfg;
  GTIMsCfg.SlaverMode = GTIM_SLAVER_MODE_EXTENAL;       // 设置从模式为触发模式
  GTIMsCfg.TrigInput = GTIM_TRIG_INPUT_ITR0;            // ATIMTRGO
  GTIMsCfg.PreloadSource = GTIM_PRELOAD_SOURCE_EVENT;   // 选择预加载源为事件
  GTIMsCfg.PreloadState = GTIM_PRELOAD_DIS;             // 不启用预加载
  GTIM_SlaverModeCfg(CW_GTIM2, &GTIMsCfg);              // 配置 GTIM2 定时器
  
  GTIM_OC2ModeCfg(CW_GTIM2, &GTIM_OCModeCfgStruct);
  GTIM_SetCompare2(CW_GTIM2, 50);
  GTIM_OC2Cmd(CW_GTIM2, ENABLE);
  GTIM_MasterTRGOCfg(CW_GTIM2,GTIM_MASTER_TRGO_UPDATE);
  GTIM_Cmd(CW_GTIM2, ENABLE);
*/
}

void BTIM_Configuration(void)
{
  __SYSCTRL_BTIM123_CLK_ENABLE();
  
  BTIM_TimeBaseInitTypeDef BTIM_Cfg;
  
  BTIM_Cfg.BTIM_Prescaler = 0;    // 7+1=8分频
  BTIM_Cfg.BTIM_Period = 48000-1;
  BTIM_Cfg.BTIM_CountMode = BTIM_COUNT_MODE_REPETITIVE;  //多次
  BTIM_Cfg.BTIM_Mode = BTIM_MODE_TIMER;
  
  BTIM_TimeBaseInit(CW_BTIM1, &BTIM_Cfg);
  BTIM_ITConfig(CW_BTIM1, BTIM_IT_UPDATE, ENABLE);
  CW_BTIM1->CNT = 40000;
  BTIM_Cmd(CW_BTIM1, ENABLE);
  
//  CW_BTIM3->CNT = 120;
//  BTIM_Cfg.BTIM_Prescaler = 0;    // 7+1=8分频
//  BTIM_Cfg.BTIM_Period = 1000-1;
//  BTIM_ITConfig(CW_BTIM3, BTIM_IT_UPDATE, ENABLE);
//  BTIM_Cmd(CW_BTIM3, ENABLE);
}


/* 呼吸灯效果  

uint16_t ledblinkbuf[255] = 
{   0,   0,    0,    1,    1,    2,    2,    3,    5,    6,    8,    9,   11,   14,
   16,  19,   21,   24,   28,   31,   35,   39,   43,   47,   52,   57,   62,   67,
   73,  79,   85,   91,   98,  105,  112,  119,  127,  135,  143,  152,  160,  169,
  178, 188,  198,  208,  218,  228,  239,  250,  262,  273,  285,  298,  310,  323,
  336, 349,  363,  377,  391,  405,  420,  435,  451,  466,  482,  498,  515,  532,
  549, 566,  584,  602,  620,  639,  658,  677,  696,  716,  736,  757,  777,  798,
  820, 841,  863,  885,  908,  931,  954,  977, 1001, 1025, 1050, 1075, 1100, 1125,
 1151,1177, 1203, 1229, 1256, 1284, 1311, 1339, 1367, 1396, 1425, 1454, 1483, 1513,
 1543,1574, 1605, 1636, 1667, 1699, 1731, 1764, 1796, 1830, 1863, 1897, 1931, 1965,
 2000,2000, 1965, 1931, 1897, 1863, 1830, 1796, 1764, 1731, 1699, 1667, 1636, 1605,
 1574,1543, 1513, 1483, 1454, 1425, 1396, 1367, 1339, 1311, 1284, 1256, 1229, 1203,
 1177,1151, 1125, 1100, 1075, 1050, 1025, 1001,  977,  954,  931,  908,  885,  863,
  841, 820,  798,  777,  757,  736,  716,  696,  677,  658,  639,  620,  602,  584,
  566, 549,  532,  515,  498,  482,  466,  451,  435,  420,  405,  391,  377,  363,
  349, 336,  323,  310,  298,  285,  273,  262,  250,  239,  228,  218,  208,  198,
  188, 178,  169,  160,  152,  143,  135,  127,  119,  112,  105,   98,   91,   85,
   79,  73,   67,   62,   57,   52,   47,   43,   39,   35,   31,   28,   24,   21,
   19,  16,   14,   11,    9,    8,    6,    5,    3,    2,    2,    1,    1,    0,
    0,   0,
};

#include "math.h"
#define GAMMA 2.2           // 定义GAMMA系数
#define STEPS 4000           // 呼吸灯周期的步数
#define MAX_BRIGHTNESS 2000 // 最大亮度

// 生成当前步数的呼吸灯亮度值
uint16_t get_breathing_light(uint16_t step) {
    double breath_curve;
    
    if (step < STEPS / 2) {
        // 上升部分 (从0到1)
        breath_curve = (double)step / (STEPS / 2 - 1);
    } else {
        // 下降部分 (从1到0)
        breath_curve = (double)(STEPS / 2 - (step - STEPS / 2) - 1) / (STEPS / 2 - 1);
    }
    
    // 应用GAMMA校正并返回亮度值
    return (uint16_t)(pow(breath_curve, GAMMA) * MAX_BRIGHTNESS);
}

void ATIM_IRQHandler2(void){
  static uint16_t lightValue;
  static uint32_t step;
  ATIM_SetCompare1(lightValue);
  
  lightValue = get_breathing_light(step++);
  
  if(step >= STEPS)step = 0;
  
  ATIM_ClearITPendingBit(ATIM_IT_UIE);
}
*/

