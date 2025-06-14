#include "app_battery.h"
#include "bsp_gpio.h"
#include "bsp_adc.h"
#include "app_sleep.h"


BAT_BUF bat;

void bat_level2BATLv(void)
{
  static int lastBATLevel = -2;
  
  int newLevel = lastBATLevel;
  switch (lastBATLevel)
  {
    case -1:
      if (bat.level > 305) newLevel = 0;
      break;
    case 0:
      if (bat.level < 295) newLevel = -1;
      else if (bat.level > 335) newLevel = 1;
      break;
    case 1:
      if (bat.level < 325) newLevel = 0;
      else if (bat.level > 365) newLevel = 2;
      break;
    case 2:
      if (bat.level < 355) newLevel = 1;
      else if (bat.level > 385) newLevel = 3;
      break;
    case 3:
      if (bat.level < 375) newLevel = 2;
      else if (bat.level > 405) newLevel = 4;
      break;
    case 4:
      if (bat.level < 395) newLevel = 3;
      break;
    default: // 初始状态或未知
      if (bat.level < 300)
        newLevel = -1;
      else if (bat.level < 330)
        newLevel = 0;
      else if (bat.level < 360)
        newLevel = 1;
      else if (bat.level < 380)
        newLevel = 2;
      else if (bat.level < 400)
        newLevel = 3;
      else
        newLevel = 4;
      break;
    }
  
    if (newLevel != lastBATLevel)
    {
      sys.BATLevel = newLevel;
      lastBATLevel = newLevel;
    }
}

inline uint16_t abs_int16(int16_t x1,int16_t x2)
{
  if(x1 > x2)
    return x1-x2;
  else return x2-x1;
}

int16_t bat_changesmooth(uint16_t nowLv, int8_t charge)
{
  static int8_t lastcharge;
  static uint8_t changeTick,maxTick;
  
  
  if(bat.lastlv == -1)
  {
    if(charge>0)
      bat.lastlv = nowLv-12;
    else bat.lastlv = nowLv;
    lastcharge = charge;
  }
  
  //uint16_t subtractLv = abs_int16(nowLv,lastlv);
  
  if(lastcharge != charge)
  {
    maxTick = 6;
    changeTick = maxTick/2;
    lastcharge  = charge;
  }
  
  if(bat.lastlv > nowLv + 12)
  {
    maxTick = 80;
  }
  
  if(charge < 0)
  {
    bat.lastlv = nowLv;
  }else if(charge >0)
  {
    if(changeTick < maxTick)
    {
      changeTick++;
      bat.lastlv = nowLv-12;
    }
    else if(bat.lastlv < nowLv-12)
    {
      changeTick = 0;
      bat.lastlv++;
    }
  }
  else
  {
    if(changeTick)
    {
      changeTick--;
    }else {
      changeTick = 0;
      if(bat.lastlv > nowLv)
      {
        bat.lastlv--;
      }else if(bat.lastlv  + 50 < nowLv)
        bat.lastlv = nowLv;
    }
  }

  return bat.lastlv;
}



int8_t bat_chargrStateGet(uint8_t init)
{
  static uint8_t chtick,cftick;
  
  if(init==1)
  {
    chtick = VI_DET_READ()?40:0;
    cftick = CHGE_READ()?0:20;
  }else{
    if(VI_DET_READ() == POWER_CONNECTED)
    {
      if(chtick < 40)
        chtick+=4;
    }else 
    {
      if(chtick)
        chtick--;
    }
    if(CHGE_READ() == 1)  //低电平充电
    {
      if(cftick < 20)
        cftick++;
    }else 
    {
      if(cftick)
        cftick--;
    }
  }

  if((chtick >= 40 || chtick == 0)&&(cftick==0 ||  cftick>=20))
  {
    if((chtick >= 40 && cftick == 0))
    {
      return 1;
    }else
    {
      return 0;
    }
  }else
  {
    return -1;
  }

}

void bat_handler1(void* arg)
{
  static uint16_t oldValue;
  int8_t charge;
  
  bat.tempLv = adc_Abat2bat(bat.value);
  if(oldValue == 0)
  {
    charge = bat_chargrStateGet(1);
    if(bat.tempLv >= 0)
    {
      bat.level = bat.tempLv;
      oldValue = bat.value;
      bat_level2BATLv();
    }
    return;
  }

  charge = bat_chargrStateGet(0);
  if(charge < 0)
    return;
  
  if(bat.tempLv >= 0) 
  {
    if((sys.state & 0xFE) == POWER_OFF_NO_CHARGE)
    {
      bat.level = bat.tempLv;
    }else
    {
      bat.level = bat_changesmooth(bat.tempLv, charge);
    }
  }
  bat_level2BATLv();
}


void bat_handler(void* arg)
{
  int charge = bat_chargrStateGet(0);
  if (charge < 0) return;

  bat.tempLv = adc_Abat2bat(bat.value);
  if (bat.tempLv < 0) return;

  bat.level = bat.tempLv;

  bat_level2BATLv();
  
}

uint8_t bat_Ref()
{
  bat_chargrStateGet(1);
  bat.value = ADC_average(adc.BAT,10, 100, 3500);
  uint16_t battemp = adc_Abat2bat(bat.value);
  if(battemp > bat.tempLv)
  {
    bat.tempLv = battemp;
  }
  bat.lastlv = bat.tempLv;

  bat_level2BATLv();
  if(sys.BATLevel > 1)
    sys.lpLock = 0;
  
  if(bat.tempLv > 300)
    return 1;
  else return 0;
}

