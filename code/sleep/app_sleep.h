#ifndef __BSP_SLEEP___
#define __BSP_SLEEP___

#include "main.h"

#define CHARGE_FULL     1
#define CHARGE_NOT_FULL 0

#define POWER_CONNECTED   	1
#define POWER_DISCONNECTED 	0

#define POWER_ON  1
#define POWER_OFF 0


#define PULSESLEEP_Period  (1170000/SysStatePeriod)     //ms 19.5*6
// 定义设备状态枚举
enum DeviceState {
    POWER_OFF_NO_CHARGE 			= 0,  // 关机不充电
    POWER_OFF_NO_CHARGE_FULLY = 1,	// 关机不充电
		POWER_OFF_CHARGING 				= 2,  // 关机充电未充满
		POWER_OFF_FULLY_CHARGED 	= 3,  // 关机充电充满
    POWER_ON_NO_CHARGE 				= 4,  // 开机不充电
		POWER_ON_NO_CHARGE_FULLY	= 5,	// 开机不充电
    POWER_ON_CHARGING 				= 6,  // 开机充电未充满
    POWER_ON_FULLY_CHARGED 		= 7,  // 开机充电充满
};

#pragma anon_unions
typedef struct {
  union{
    struct {
      uint8_t cf : 1;       // Charge_FULL，1 bit[0]
      uint8_t ch : 1;       // POWER_IN，1 bit[1]	
      uint8_t sw : 1;       // POWER_BUTTON，1 bit[2]  
      //uint8_t : 5;          // 2 bits padding	[3-7]
    }cfg;
    struct {
      uint8_t state : 3;    // state，3 bit[0-2]
      uint8_t lpLock : 1;   // 
      uint8_t chargeOK : 1; // 
      uint8_t lcdStart : 1;
    };
  };                         //开关机充电状态
  uint16_t lowPower_tick;    //低电量关机计数器
  uint16_t psTick;           //暂停20分钟休眠
  int8_t BATLevel;           //电池电量等级
  int8_t batLv;
}SYS_STATE;

extern SYS_STATE sys;

void LCD_DIR(uint8_t dir);
void Sleep_Stop(void);
void Sleep_Start(void);
void Sleep_Config(void);
void System_Handle(void);
void Start_system(void);
#endif


