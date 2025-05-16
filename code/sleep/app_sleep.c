#include "app_sleep.h"
#include "bsp_adc.h"
#include "bsp_gpio.h"

#include "app_key.h"


SYS_STATE sys;
static uint16_t sleep_tick;//休眠计时
static uint8_t chTick,cfTick = 10;

#define LOWPOWER_Period 4000/SysStatePeriod //ms
#define SLEEP_Period 4000/SysStatePeriod    //ms
uint8_t CHGE_STABLE_GET()
{
  sys.chargeOK = (cfTick >= 20 || cfTick==0) && (chTick >= 6 || chTick == 0);
  return sys.chargeOK;
}

void read_Pinstate(void)
{
  if(VI_DET_READ() == 1)
  {
    if(chTick < 6)
    {
      chTick++;
      
    }
    else{
      sys.cfg.ch = POWER_CONNECTED;
      
    }
  }else if(sys.cfg.sw == POWER_OFF)
  {
    cfTick = 10;
    chTick = 0;
    sys.cfg.ch = POWER_DISCONNECTED;
  }
  else
  {
    cfTick = 10;
    if(chTick){
      chTick--;
    }else{
      sys.cfg.ch = POWER_DISCONNECTED; 
    }
  }
 
  if(CHGE_READ() == 1)
  {
    if(cfTick)
      cfTick--;
    else{
      sys.cfg.cf = CHARGE_NOT_FULL;
    }
    
  }else{
    if(cfTick<20)
      cfTick+=5;
    else{
      sys.cfg.cf = CHARGE_FULL;
    }
  }
  
  if(sys.cfg.ch == POWER_CONNECTED)
  {
    if(sys.cfg.cf == CHARGE_FULL)
    {
      if(sys.lowPower_tick>12)
      {
        sys.lowPower_tick-=12;
      }else {
        sys.lowPower_tick = 0;
        sys.lpLock = 0;
      }
      sys.batLv = 4; 
    }else 
    {
      if(sys.lowPower_tick)
      {
        sys.lowPower_tick--;
        if(sys.BATLevel > 1)
        {
          sys.lpLock = 0;
        }
      }
      
      sys.batLv = (sys.BATLevel>4)?4:sys.BATLevel; 
    }
  }else if(sys.BATLevel == 0 || sys.lpLock)  
  {
    sys.batLv = 0;
    if(sys.lowPower_tick < 2000/SysStatePeriod)
    {
      sys.lowPower_tick++;
    }
    else
    {
      sys.lpLock = 1;
    }
  }else if(sys.BATLevel == -1)  
  {
    sys.batLv = 0;
    if(sys.lowPower_tick < LOWPOWER_Period)
    {
      sys.lowPower_tick++;
      if(sys.lowPower_tick >= LOWPOWER_Period)
      {
        sleep_tick = SLEEP_Period-1;
      }
    }
    else
    {
//      motor.power = DIR_OFF;
//      sys.cfg.sw = POWER_OFF;
    }
  }else {
    sys.batLv = sys.BATLevel;
    if(sys.BATLevel > 1)
    {
      if(sys.lowPower_tick)
      {
        sys.lowPower_tick--;
        if(sys.BATLevel > 1)
        {
          sys.lpLock = 0;
        }
      }
    }
  }
}


void Sleep_Config(void)
{
  sys.state = POWER_OFF_NO_CHARGE;
	read_Pinstate();
  if(VI_DET_READ() == 1){
    sys.cfg.ch = POWER_CONNECTED;
  }else{
    sys.cfg.ch = POWER_DISCONNECTED;
  }
	sys.BATLevel = -2;
}

void System_Handle()
{
	read_Pinstate();
  
  
  switch(sys.state)
  {
    case POWER_OFF_FULLY_CHARGED: //关机充满
    case POWER_OFF_CHARGING:      //关机充电
      sleep_tick = 0;
      break;
    
    case POWER_OFF_NO_CHARGE://关机不充电
    case POWER_OFF_NO_CHARGE_FULLY:
      if(sleep_tick < SLEEP_Period)
      {
        sleep_tick++;
      }else{
        if(KEY_PW_READ() == 0)
        {
          sleep_tick = 0;
          sys.lowPower_tick = 0;
          if(sys.cfg.ch == POWER_DISCONNECTED)
          {
            ESP_EN(0);
            VOUT_EN(0);
            sys.lcdStart = 0;
            //Sleep_Start();
            sys.psTick = 0;
          }
        }
      }
      break;
    case POWER_ON_NO_CHARGE:      
    case POWER_ON_NO_CHARGE_FULLY://开机不充电
    case POWER_ON_FULLY_CHARGED:  //开机充满
    case POWER_ON_CHARGING:       //开机充电
      sleep_tick = 0;
      if(sys.lowPower_tick >= LOWPOWER_Period)
      { 

      }else
      {
        if(sys.cfg.ch == POWER_CONNECTED)
          sys.psTick = 0;
        else if(PULSESLEEP_Period > sys.psTick)
        {
          sys.psTick++;
        }else{
          sys.cfg.sw = POWER_OFF;
          sys.lcdStart = 0;
        }
      }
      break;
    default:
      sys.lowPower_tick = 0;
      sleep_tick = 0;
      break;
  }
}

void Sleep_Start(void)
{
  SYSCTRL_LSI_Enable();
  ADC_Disable();
  GPIO_SWD2GPIO();
  
  
  REGBITS_CLR(KEY1_GPIO->PUR, KEY_ALL_Pin);

//  GTIM_Cmd(CW_GTIM1, DISABLE);

  
  //开启中断和滤波  
  __disable_irq();
  
  REGBITS_SET(KEY_PW_GPIO->FALLIE, KEY_PW_Pin);
  REGBITS_SET(VIN_GPIO->RISEIE, VIN_Pin);
  
  GPIO_ConfigFilter(GPIOA,  KEY_PW_Pin, GPIO_FLTCLK_LSI);
  GPIOA_INTFLAG_CLR(KEY_PW_Pin);          //清除中断
  
  GPIO_ConfigFilter(GPIOB,  VIN_Pin, GPIO_FLTCLK_LSI);
  GPIOB_INTFLAG_CLR(VIN_Pin);          //清除中断
  
  SCB->SCR = 0x04;    //DeepSleep
  __WFI();
  
  SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV2);
  SYSCTRL_SYSCLKSRC_Config(SYSCTRL_SYSCLKSRC_HSI);
  SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);    //  配置时钟48MHz
  SYSCTRL_LSI_Disable();                      //关闭低速时钟
  
  __enable_irq();
  
  ADC_Enable();
  GPIO_GPIO2SWD();
  
//  GTIM_Cmd(CW_GTIM1, ENABLE);
  
  //关闭中断和滤波
  REGBITS_CLR(KEY_PW_GPIO->FALLIE, KEY_PW_Pin);
  REGBITS_CLR(VIN_GPIO->RISEIE, VIN_Pin);
  
  REGBITS_SET(KEY1_GPIO->PUR, KEY_ALL_Pin);

  //GPIO_ConfigFilter(GPIOA,  0, GPIO_FLTCLK_LSI);
}
