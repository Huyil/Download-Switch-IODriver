#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "cw32l011_gpio.h"
#include "cw32l011_sysctrl.h"

#define GPIOA ((GPIO_TypeDef*) GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef*) GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef*) GPIOC_BASE)

#define LED_ON  (1u)
#define LED_OFF (0)
#define LED_POWEROFF (-1)


#define CHRG_GPIO  GPIOB
#define CHRG_Pin   GPIO_PIN_1

#define DONE_GPIO  GPIOB
#define DONE_Pin   GPIO_PIN_0

#define KEY1_GPIO  GPIOA
#define KEY1_Pin   GPIO_PIN_2

#define KEY2_GPIO  GPIOA
#define KEY2_Pin   GPIO_PIN_1

#define KEY3_GPIO  GPIOA
#define KEY3_Pin   GPIO_PIN_4

#define KEY4_GPIO  GPIOA
#define KEY4_Pin   GPIO_PIN_0

#define KEY5_GPIO  GPIOA
#define KEY5_Pin   GPIO_PIN_3


#define SDA_GPIO  GPIOA
#define SDA_Pin   GPIO_PIN_13

#define SCL_GPIO  GPIOA
#define SCL_Pin   GPIO_PIN_14


#define ESP_EN_GPIO  GPIOA
#define ESP_EN_Pin   GPIO_PIN_5

#define VOUT_EN_GPIO  GPIOA
#define VOUT_EN_Pin   GPIO_PIN_6

#define DIO_EN_GPIO  GPIOC
#define DIO_EN_Pin   GPIO_PIN_14

#define DIO_A_GPIO  GPIOA
#define DIO_A_Pin   GPIO_PIN_11

#define DIO_B_GPIO  GPIOA
#define DIO_B_Pin   GPIO_PIN_10

#define SCK_EN_GPIO  GPIOC
#define SCK_EN_Pin   GPIO_PIN_15

#define SCK_A_GPIO  GPIOA
#define SCK_A_Pin   GPIO_PIN_9

#define SCK_B_GPIO  GPIOA
#define SCK_B_Pin   GPIO_PIN_8

#define CH1P_GPIO  GPIOA
#define CH1P_Pin   GPIO_PIN_12
#define CH1N_GPIO  GPIOA
#define CH1N_Pin   GPIO_PIN_15

#define CH2P_GPIO  GPIOB
#define CH2P_Pin   GPIO_PIN_3
#define CH2N_GPIO  GPIOB
#define CH2N_Pin   GPIO_PIN_4

#define CH3P_GPIO  GPIOB
#define CH3P_Pin   GPIO_PIN_5
#define CH3N_GPIO  GPIOB
#define CH3N_Pin   GPIO_PIN_6

#define CH4P_GPIO  GPIOB
#define CH4P_Pin   GPIO_PIN_7
#define CH4N_GPIO  GPIOC
#define CH4N_Pin   GPIO_PIN_13


#define LEDX_GPIO     GPIOA
#define LEDX_Pin      (0)

#define ADC_Pin         (0|GPIO_PIN_7)
#define ADC_BPin        (0)

#define OUT_PP_Pin      (0|CH1P_Pin|CH1N_Pin|SCK_A_Pin|SCK_B_Pin|DIO_A_Pin|DIO_B_Pin|ESP_EN_Pin|VOUT_EN_Pin)
#define OUT_PP_BPin     (0|CH2P_Pin|CH2N_Pin|CH3P_Pin|CH2N_Pin|CH4P_Pin)
#define OUT_PP_CPin     (0|CH4N_Pin|DIO_EN_Pin|SCK_EN_Pin)

#define OUT_OD_Pin      (0|SDA_Pin|SCL_Pin)
#define OUT_OD_BPin     (0)
#define OUT_OD_Up_Pin   (0)

#define IN_UP_Pin       (0|KEY1_Pin|KEY2_Pin|KEY3_Pin|KEY4_Pin|KEY5_Pin)
#define IN_UP_BPin      (0|CHRG_Pin|DONE_Pin)
#define IN_UP_CPin      (0)

#define IN_Pin          (0)
#define IN_BPin         (0)


#define VIN_GPIO   CHRG_GPIO
#define VIN_Pin    (DONE_Pin|CHRG_Pin)
#define VI_DET_READ()     (((VIN_GPIO ->IDR & VIN_Pin)==3)?0:1u)
#define DONE_READ()       ((DONE_GPIO ->IDR & DONE_Pin)?1u:0)
#define CHGE_READ()       ((CHRG_GPIO ->IDR & CHRG_Pin)?0:1u)

#define KEY1_READ()     ((KEY1_GPIO ->IDR & KEY1_Pin)?1u:0)
#define KEY2_READ()     ((KEY2_GPIO ->IDR & KEY2_Pin)?1u:0)
#define KEY3_READ()     ((KEY3_GPIO ->IDR & KEY3_Pin)?1u:0)
#define KEY4_READ()     ((KEY4_GPIO ->IDR & KEY4_Pin)?1u:0)
#define KEY5_READ()     ((KEY5_GPIO ->IDR & KEY5_Pin)?1u:0)
#define KEY_ALL_Pin      (KEY1_Pin|KEY2_Pin|KEY3_Pin|KEY4_Pin|KEY5_Pin)
#define KEY_READ()      (KEY1_GPIO ->IDR & KEY_ALL_Pin)

#define KEY_PW_READ() KEY1_READ()
#define KEY_PW_GPIO   KEY1_GPIO
#define KEY_PW_Pin    KEY1_Pin

#define ESP_EN(i)    (i)?(ESP_EN_GPIO->BSRR = ESP_EN_Pin):(ESP_EN_GPIO->BRR = ESP_EN_Pin)
#define VOUT_EN(i)   (i)?(VOUT_EN_GPIO->BSRR = VOUT_EN_Pin):(VOUT_EN_GPIO->BRR = VOUT_EN_Pin)

#define DIO_EN(i)    (i)?(DIO_EN_GPIO->BSRR = DIO_EN_Pin):(DIO_EN_GPIO->BRR = DIO_EN_Pin)
#define DIO_A(i)     (i)?(DIO_A_GPIO->BSRR = DIO_A_Pin):(DIO_A_GPIO->BRR = DIO_A_Pin)
#define DIO_B(i)     (i)?(DIO_B_GPIO->BSRR = DIO_B_Pin):(DIO_B_GPIO->BRR = DIO_B_Pin)

#define SCK_EN(i)    (i)?(SCK_EN_GPIO->BSRR = SCK_EN_Pin):(SCK_EN_GPIO->BRR = SCK_EN_Pin)
#define SCK_A(i)     (i)?(SCK_A_GPIO->BSRR = SCK_A_Pin):(SCK_A_GPIO->BRR = SCK_A_Pin)
#define SCK_B(i)     (i)?(SCK_B_GPIO->BSRR = SCK_B_Pin):(SCK_B_GPIO->BRR = SCK_B_Pin)


#define CH1P(i)        (i)?(CH1P_GPIO->BSRR = CH1P_Pin):(CH1P_GPIO->BRR = CH1P_Pin)
#define CH1N(i)        (i)?(CH1N_GPIO->BSRR = CH1N_Pin):(CH1N_GPIO->BRR = CH1N_Pin)
#define CH2P(i)        (i)?(CH2P_GPIO->BSRR = CH2P_Pin):(CH2P_GPIO->BRR = CH2P_Pin)
#define CH2N(i)        (i)?(CH2N_GPIO->BSRR = CH2N_Pin):(CH2N_GPIO->BRR = CH2N_Pin)
#define CH3P(i)        (i)?(CH3P_GPIO->BSRR = CH3P_Pin):(CH3P_GPIO->BRR = CH3P_Pin)
#define CH3N(i)        (i)?(CH3N_GPIO->BSRR = CH3N_Pin):(CH3N_GPIO->BRR = CH3N_Pin)
#define CH4P(i)        (i)?(CH4P_GPIO->BSRR = CH4P_Pin):(CH4P_GPIO->BRR = CH4P_Pin)
#define CH4N(i)        (i)?(CH4N_GPIO->BSRR = CH4N_Pin):(CH4N_GPIO->BRR = CH4N_Pin)

#define ESP_EN_READ()     ((ESP_EN_GPIO->ODR & ESP_EN_Pin) ? 1u : 0)
#define VOUT_EN_READ()    ((VOUT_EN_GPIO->ODR & VOUT_EN_Pin) ? 1u : 0)

#define DIO_EN_READ()     ((DIO_EN_GPIO->ODR & DIO_EN_Pin) ? 1u : 0)
#define DIO_A_READ()      ((DIO_A_GPIO->ODR & DIO_A_Pin) ? 1u : 0)
#define DIO_B_READ()      ((DIO_B_GPIO->ODR & DIO_B_Pin) ? 1u : 0)

#define SCK_EN_READ()     ((SCK_EN_GPIO->ODR & SCK_EN_Pin) ? 1u : 0)
#define SCK_A_READ()      ((SCK_A_GPIO->ODR & SCK_A_Pin) ? 1u : 0)
#define SCK_B_READ()      ((SCK_B_GPIO->ODR & SCK_B_Pin) ? 1u : 0)

#define CH1P_READ()       ((CH1P_GPIO->ODR & CH1P_Pin) ? 1u : 0)
#define CH1N_READ()       ((CH1N_GPIO->ODR & CH1N_Pin) ? 1u : 0)
#define CH2P_READ()       ((CH2P_GPIO->ODR & CH2P_Pin) ? 1u : 0)
#define CH2N_READ()       ((CH2N_GPIO->ODR & CH2N_Pin) ? 1u : 0)
#define CH3P_READ()       ((CH3P_GPIO->ODR & CH3P_Pin) ? 1u : 0)
#define CH3N_READ()       ((CH3N_GPIO->ODR & CH3N_Pin) ? 1u : 0)
#define CH4P_READ()       ((CH4P_GPIO->ODR & CH4P_Pin) ? 1u : 0)
#define CH4N_READ()       ((CH4N_GPIO->ODR & CH4N_Pin) ? 1u : 0)


#define MODE_SWD   0
#define MODE_UART  1
#define MODE_I2C   2
#define MODE_UART3 3
#define MODE_ATIM  4


void GPIO_Configuration(void);
void PinMode_Set(uint8_t mode);

#endif


