#include "app_key.h"
#include "multi_button.h"

#include "app_sleep.h"
#include "app_battery.h"
#include "app_io.h"
#include "debug.h"

struct Button PowerKey;     // 电源
struct Button AddKey;       // add
struct Button SubKey;       // dec

io_one gei_io;
void KeyPower_CB(void* btu)
{
  if(sys.cfg.sw == POWER_OFF)
  {
    if(sys.cfg.ch == POWER_CONNECTED)
    {
      sys.lowPower_tick = 0;
    }else if(bat_Ref() > 0)
    {
      sys.lowPower_tick = 0;
//    }else {
//      sys.lowPower_tick = 4000;
//      return;
    }
    
    PinMode_Set(MODE_I2C);
    IO_get(CH1,&gei_io);
    ESP_EN(1);
    
    sys.cfg.sw = POWER_ON;
  }else
  {
    if(sys.cfg.ch == POWER_DISCONNECTED)
    {
      ESP_EN(0);
      PinMode_Set(MODE_SWD);
    }
    sys.cfg.sw = POWER_OFF;
  }
}

uint8_t readKeyStatus(uint8_t id)
{
  io.key5 = KEY5_READ();
  io.key4 = KEY4_READ();
  io.key3 = KEY3_READ();
  io.key2 = KEY2_READ();
  io.key1 = KEY1_READ();
  
  switch(id)
  {
    case 0:return io.key5;
    case 1:return io.key4;
    case 2:return io.key3;
    case 3:return io.key2;
    case 4:return io.key1;
  }
  return 1; 
}

void KeyEnter_CB(void* btu)
{
  enum CHx ch = CH1;
  enum CHxMode mode = GND;
  IO_set(ch,mode);

}


/*初始化并注册菜单事件*/
void Key_Init(void)
{
  button_init(&PowerKey,readKeyStatus, 0,  0,SHORT_TICKS,LONG_TICKS);
  
  button_attach(&PowerKey, SINGLE_CLICK,     KeyEnter_CB);
  button_attach(&PowerKey, LONG_PRESS_START, KeyPower_CB);

  button_start(&PowerKey);
}

