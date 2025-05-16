/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "../inc/main.h"
#include "multi_timer.h"
#include "bsp_timer.h"
#include "app_sleep.h"
#include "bsp_gpio.h"
#include "bsp_adc.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t value;
/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void ADC_Configuration(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


int32_t main(void)
{
  /* System Clocks Configuration */
  SYSCTRL_Configuration();
  /* ADC Configuration */
  ADC_Configuration();     //需要先打开ADC时钟才能接入ATIM信号触发
  /* GPIO Configuration */
  GPIO_Configuration();
  /* NVIC Configuration */
  NVIC_Configuration();    //中断配置
  
  //cpLED_Show_Level(25);
  //GTIM_Configuration();
  BTIM_Configuration();   //初始化定时器
  ATIM_Configuration();   //初始化定时器(PWM脉冲)
  
  ADC_ExtTrigCfg(ADC_TRIG_ATIMTRGO, ENABLE);
  
  TaskAll_Config();        //创建任务循环
  
  while(1)
  {
    timer_loop();         //任务循环
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
uint8_t te;
void SYSCTRL_Configuration(void)
{
  SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV2);
  SYSCTRL_SYSCLKSRC_Config(SYSCTRL_SYSCLKSRC_HSI);
  SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);    //  配置时钟96MHz
  SystemCoreClock = 96000000ul;
  te = SysTick_Config(0xFFFFFFUL+1);  //96000 ms

  
  CW_SYSCTRL->DEBUG_f.ATIM = 0;    //调试时不关闭ATIM
  CW_SYSCTRL->DEBUG_f.GTIM2 = 0;   //调试时不关闭GTIM
  
  CW_SYSCTRL->CR2_f.WAKEUPCLK = 1;    //唤醒使用内部高速时钟HSI	
  CW_SYSCTRL->CR2_f.LOCKUP = 1;       //遇到严重故障自动重启
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  __disable_irq();
  
  __NVIC_SetPriority(ADC_IRQn,3);
  __NVIC_SetPriority(ATIM_IRQn,2);
  __NVIC_SetPriority(BTIM1_IRQn,0);
  __NVIC_SetPriority(I2C_IRQn,1);
//  __NVIC_SetPriority(BTIM3_IRQn,1);
//  __NVIC_SetPriority(BTIM2_IRQn,1);
//  __NVIC_SetPriority(GTIM2_IRQn,1);
  
  NVIC_EnableIRQ(ADC_IRQn);
  NVIC_EnableIRQ(ATIM_IRQn);
  NVIC_EnableIRQ(BTIM1_IRQn);
  NVIC_EnableIRQ(I2C_IRQn);
  //NVIC_EnableIRQ(BTIM2_IRQn);
  //NVIC_EnableIRQ(BTIM3_IRQn);
  //NVIC_EnableIRQ(GTIM2_IRQn);
  NVIC_EnableIRQ(GPIOA_IRQn);
  NVIC_EnableIRQ(GPIOB_IRQn);
  
  __enable_irq();
}




/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

