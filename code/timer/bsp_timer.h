#ifndef __BSP_TIMER_H_
#define __BSP_TIMER_H_

#include "cw32l011_atim.h"
#include "cw32l011_btim.h"
#include "cw32l011_gtim.h"
#include "cw32l011_sysctrl.h"

#define PWM_MODE (1)  //1:碳刷; 0:空心杯
#if(PWM_MODE == 0)
  #define PWM_MAX (1600)
#else
  #define PWM_MAX (2400)
#endif

void ATIM_Configuration(void);
void BTIM_Configuration(void);
void GTIM_Configuration(void);

typedef union {
    __IOM uint32_t CCMRxCMP;                  /*!< (@ 0x00000018) capture compare mode register 1                            */

    struct {
      __IOM uint32_t CCxS     : 2;            /*!< [1..0] desc CC1S                                                          */
      __IOM uint32_t OCxFE    : 1;            /*!< [2..2] desc OC1FE                                                         */
      __IOM uint32_t OCxPE    : 1;            /*!< [3..3] desc OC1PE                                                         */
      __IOM uint32_t OCxM     : 3;            /*!< [6..4] desc OC1M                                                          */
      __IOM uint32_t OCxCE    : 1;            /*!< [7..7] desc OC1CE                                                         */
      __IOM uint32_t CCyS     : 2;            /*!< [9..8] desc CC2S                                                          */
      __IOM uint32_t OCyFE    : 1;            /*!< [10..10] desc OC2FE                                                       */
      __IOM uint32_t OCyPE    : 1;            /*!< [11..11] desc OC2PE                                                       */
      __IOM uint32_t OCyM     : 3;            /*!< [14..12] desc OC2M                                                        */
      __IOM uint32_t OCyCE    : 1;            /*!< [15..15] desc OC2CE                                                       */
      __IOM uint32_t OCxMH    : 1;            /*!< [16..16] desc OC1MH                                                       */
      __IM  uint32_t          : 7;
      __IOM uint32_t OCyMH    : 1;            /*!< [24..24] desc OC2MH                                                       */
    } CCMRxCMP_f;
}OCInit_TypeDef;

typedef struct
{
  uint16_t Pulse;                   /*!< 比较值 */
  uint16_t OCMode;                  /*!< 比较模式配置 */
  FunctionalState OCFastMode;       /*!< 快速比较模式 */
  FunctionalState OCInterruptState; /*!< 比较匹配触发中断使能状态 */
  FunctionalState BufferState;      /*!< 比较缓存使能状态 */

  FunctionalState OCOutput;    /*!< 比较互补输出 */
  FunctionalState OCPolarity;       /*!< 端口极性选择：正向、反向 */
  
  FunctionalState OCNOutput;   /*!< 反向通道 比较互补输出 */
  FunctionalState OCNPolarity;      /*!< 反向通道 端口极性选择：正向、反向 */
} MY_ATIM_OCInitTypeDef;
#endif


