#include "main.h"
#include "bsp_gpio.h"
#include "bsp_adc.h"

#include "multi_timer.h"
#include "multi_button.h"

#include "app_battery.h"
#include "app_sleep.h"
#include "app_key.h"
#include "app_io.h"
#include "bsp_i2c.h"
#include "debug.h"

struct Timer system_Task;
struct Timer key_Task;
struct Timer adc_batt_Task;
struct Timer DEBUG_Task;

void Key_CB(void *arg)
{
  button_ticks();
}

void BATadcCB(void *arg)
{
  static uint8_t once;
  if(once < 3)
  {
    once++;
    adc_Avbg2vref(adc.VBG,10);
    CW_ADC->CR_f.ENS = 3;
  }
  bat.value = ADC_average(adc.BAT,10, 900, 1500);
  //bat.value = bat_middAverage(adc.BAT,10);

  if(bat.value == 0)
  {
    adc.errtick = 10;
  }
  
  if(adc.errtick)
  {
    adc.errtick--;
  }else
  {
    bat_handler(0);
  }
  
  io.batAdc = bat.value>>2;
  io.batLv = bat.level<0?8:bat.level;
  io.chrg = CHGE_READ();
  io.done = DONE_READ();
}


//获取刷新状态显示
void SystemStateCB(void *arg)
{
  System_Handle();  
}


void TaskAll_Config(void)
{
  //初始化定时器对象，注册定时器回调处理函数，设置延迟启动时间（ms），循环定时触发时间
  timer_init(&key_Task,     Key_CB,         101, 10,               0);
  timer_init(&system_Task,  SystemStateCB,   22, SysStatePeriod,   0);
  timer_init(&adc_batt_Task,BATadcCB,        50, 100,     0);

  timer_start(&key_Task);     //按键10ms
  timer_start(&system_Task);  //系统状态显示(充电显示)
  timer_start(&adc_batt_Task);//电量检测

  timer_init(&DEBUG_Task,   DEBUGCB,          8, 200,      0);
  timer_start(&DEBUG_Task);
  
  I2C_Configuration();
  IO_Refresh();
  Key_Init();       //按键
  Sleep_Config();   //休眠
}

void BTIM1_IRQHandler(void)
{
  if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_UPDATE))
  {
    BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_UPDATE); 
    timer_ticks(); 
  }
}

void SysTick_Handler(void)
{


}


