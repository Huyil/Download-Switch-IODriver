#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "string.h"



UART_BUF uart;

//void Uart_Config(void){
//  RCC_APB_Peripheral_Clock_Enable(RCC_APB_PERIPH_UART2);
//  
//  UART_InitType UART_InitStructure;
//  /* UARTy and UARTz configuration */
//  UART_InitStructure.BaudRate   = 115200;
//  UART_InitStructure.WordLength = UART_WL_8B;
//  UART_InitStructure.StopBits   = UART_STPB_1;
//  UART_InitStructure.Parity     = UART_PE_NO;
//  UART_InitStructure.Mode       = UART_MODE_RX | UART_MODE_TX;

//  /* Configure UART2 */
//  UART_Initializes(UART2, &UART_InitStructure);
//  
//  /* Enable UARTy Receive and Transmit interrupts */
//  UART_Interrput_Enable(UART2, UART_INT_RXDNE);
//  //UART_Interrput_Enable(UART2, UART_INT_TXDE);
//   
//  /* Enable the UART2 */
//  
//  UART_Enable(UART2);
//  
////  Uart_SWD_Switch(MODE_UART);
////  printf("This is a Printf demo of UART.\n\r");
////  Uart_SWD_Switch(MODE_SWD);
//  
//}

//void Uart_SWD_Switch(uint8_t swd)
//{
//  return;
//  if(swd)
//  {
//    GPIO_GPIO2SWD();
//    PA13_AFx_GPIO();
//    PA14_AFx_GPIO();
//  }else{
//    GPIO_SWD2GPIO();
//    PA14_AFx_UART1TXD();
//    PA13_AFx_UART1RXD();
//  }
//}

/////**
////*\*\name    fputc.
////*\*\fun     retarget the C library printf function to the USART
////*\*\param   ch
////*\*\param   f
////*\*\return  none
////**/
////int fputc(int ch, FILE* f)
////{
////    UART_Data_Send(UART2, (uint8_t)ch);
////    while (UART_Flag_Status_Get(UART2, UART_FLAG_TXDE) == RESET)
////        ;

////    return (ch);
////}

////uint8_t printBuf(uint8_t* buf,uint8_t len)
////{  
////  if(uart.txBusy == 0){
////    UART_Interrput_Disable(UART1, UART_INT_TXDE);
////    
////    uart.txBusy = 1;
////    uart.tx.count = 1;

////    uart.tx.len = len>=MAX_LEN?MAX_LEN:len;
////    memcpy(uart.tx.buf,buf,uart.tx.len);
////    
////    UART_Data_Send(UART1, uart.tx.buf[0]);
////    UART_Interrput_Enable(UART1, UART_INT_TXDE);
////    return 0;
////  }
////  return 1;
////}

////void UART2_IRQHandler(void)
////{
////  if (UART_Interrupt_Status_Get(UART1, UART_INT_TXDE) != RESET)
////  {
////      /* Write one byte to the transmit data register */
////      UART_Data_Send(UART1, uart.tx.buf[uart.tx.count++]);

////      if (uart.tx.count == uart.tx.len)
////      {
////        uart.txBusy = 0;
////        /* Disable the UARTy Transmit interrupt */
////        UART_Interrput_Disable(UART1, UART_INT_TXDE);
////      }
////  }
////  if (UART_Interrupt_Status_Get(UART2, UART_INT_RXDNE) != RESET)
////  {
////    char temp_data  =  UART_Data_Receive(UART2);
////    uart.rx.buf[uart.rx.count++] = temp_data;
////    
////    if(uart.rx.buf[0] == '{')
////    {
////      if(uart.rx.count > 5 )
////      {
//////          if(uart.rx.count == uart.rx.buf[2]+5u){
//////            uart.modbus.len = uart.rx.count;
//////            memcpy(uart.modbus.buf,uart.rx.buf,uart.modbus.len);
//////            
//////            uart.rx.buf[0] = 0;
//////            uart.rx.count = 0;
//////            uart.okflag = 1; 
//////          }
////        
////        if(uart.rx.count == 8){
////          uart.com.len = 8;
////          memcpy(uart.com.buf,uart.rx.buf,8);
////          
////          uart.rx.buf[0] = 0;
////          uart.rx.count = 0;
////          uart.okflag = 1; 
////          //uart_CB(0);
////        }
////        
////      }
////    }else{
////      uart.rx.count = 0;
////    }
////  }else if (UART_Interrupt_Status_Get(UART2, UART_INT_OREF) != RESET)
////  {
////    uart.rx.count = 0;
////    uart.rx.buf[0] = 0;
////  }

////}

