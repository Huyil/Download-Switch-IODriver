#include "bsp_gpio.h"
#include "app_sleep.h"

/*
* name    GPIO_Configuration.
* fun     Configures the different GPIO ports.
* return  none
*/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  __SYSCTRL_GPIOA_CLK_ENABLE();
  __SYSCTRL_GPIOB_CLK_ENABLE();
  __SYSCTRL_GPIOC_CLK_ENABLE();
  
  GPIO_Write(GPIOA, OUT_OD_Up_Pin);
  GPIO_Write(GPIOB, OUT_OD_BPin);
  GPIO_Write(GPIOC, 0);
  
  //ADC 电机反馈 电池电量 调速
  GPIO_InitStructure.IT = GPIO_IT_NONE;
  GPIO_InitStructure.Pins  = ADC_Pin;
  GPIO_InitStructure.Pull = GPIO_NO_PULL;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pins  = ADC_BPin;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //输入
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NO_PULL;
  GPIO_InitStructure.Pins  = IN_Pin;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.Pins  = IN_BPin;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //上拉输入
  GPIO_InitStructure.Pull = GPIO_PULL_UP;
  GPIO_InitStructure.Pins  = IN_UP_Pin;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.Pins  = IN_UP_BPin;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.Pins  = IN_UP_CPin;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
    
  //开漏上拉
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStructure.Pins  = OUT_OD_Up_Pin;
  GPIO_InitStructure.Pull = GPIO_PULL_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //推挽
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NO_PULL;
  GPIO_InitStructure.Pins  = OUT_PP_Pin;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.Pins  = OUT_PP_BPin;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.Pins  = OUT_PP_CPin;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //开漏
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStructure.Pins  = OUT_OD_Pin;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pins  = OUT_OD_BPin;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //SWD
  GPIO_InitStructure.Pins  = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStructure.Pull = GPIO_NO_PULL;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void PinMode_Set(uint8_t mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  if(mode == MODE_SWD)
  {
    if(CW_SYSCTRL->CR2_f.SWDIO == 1)
    {
      CW_SYSCTRL->CR2 = ((CW_SYSCTRL->CR2 & (SYSCTRL_KEY_MASK_INV-bv1)) | SYSCTRL_KEY); //SWD
    }
  }else{
    if(CW_SYSCTRL->CR2_f.SWDIO == 0)
    {
      CW_SYSCTRL->CR2 = ((CW_SYSCTRL->CR2 & (SYSCTRL_KEY_MASK_INV)) | SYSCTRL_KEY | SYSCTRL_SWDIODIS); //GPIO
    }
  }
  
  switch(mode)
  {
    case MODE_UART:
      PA13_AFx_UART1RXD();
      PA14_AFx_UART1TXD();
      GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStructure.Pins  = GPIO_PIN_14;
      GPIO_InitStructure.Pull = GPIO_NO_PULL;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    
      GPIO_InitStructure.Pins  = GPIO_PIN_13;
      GPIO_InitStructure.Pull = GPIO_NO_PULL;
      GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      break;
    case MODE_I2C:  
      PA13_AFx_I2CSDA();
      PA14_AFx_I2CSCL();
      GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
      GPIO_InitStructure.Pins  = GPIO_PIN_13|GPIO_PIN_14;
      GPIO_InitStructure.Pull = GPIO_PULL_UP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      CW_GPIOA->ODR |= GPIO_PIN_13|GPIO_PIN_14;
      break;
    case MODE_UART3:
      PA13_AFx_UART3TXD();
      PA14_AFx_UART3RXD();
      GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStructure.Pins  = GPIO_PIN_13;
      GPIO_InitStructure.Pull = GPIO_NO_PULL;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    
      GPIO_InitStructure.Pins  = GPIO_PIN_14;
      GPIO_InitStructure.Pull = GPIO_NO_PULL;
      GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      break;
    case MODE_ATIM:
      PA13_AFx_ATIMCH6();
      PA14_AFx_ATIMCH6N();
      break;
    case MODE_SWD:
    default:
      PA13_AFx_GPIO();
      PA14_AFx_GPIO();
      GPIO_InitStructure.Pins  = GPIO_PIN_13|GPIO_PIN_14;
      GPIO_InitStructure.Pull = GPIO_NO_PULL;
      GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      break;
  }
}

