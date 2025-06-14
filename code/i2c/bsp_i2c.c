#include "bsp_i2c.h"
#include "app_io.h"
#include "cw32l011_sysctrl.h"

#define I2C_REG_KEY      0x00
#define I2C_REG_BAT      0x01
#define I2C_REG_IOSET    0x02

#define I2C_REG_CH1      0x10  // 可写通道配置
#define I2C_REG_CH2      0x11
#define I2C_REG_CH3      0x12
#define I2C_REG_CH4      0x13

#define MAX_SEND_LEN     8

uint8_t u8RecvCnt = 0;
uint8_t u8SendLen = 0;
uint8_t u8RegAddr = 0;
uint8_t u8RecvPhase = 0;

uint8_t u8Recdata[8];
uint8_t u8Senddata[8];



void I2C_IRQHandler(void)
{
    static uint8_t u8Cnt = 0;
    static uint8_t u8RegAddr = 0xFF;
    static uint8_t addrMatched = 0xFF;

    switch (CW_I2C->STAT)
    {
    // 主机写入请求
    case 0x60:   //接收到SLA+W，ACK已回应
    case 0x68:   //主机丢失仲裁后被SLA+W寻址，ACK已回应
    case 0x70:   //接收到SLA（广播地址）+W，ACK已回应
    case 0x78:   //主机丢失仲裁后被SLA（广播地址）+W寻址，ACK已回应
        // 匹配地址判断（仅在 START 阶段有效）
        if (CW_I2C->MATCH_f.ADDR0) {
            addrMatched = 0;
        } else if (CW_I2C->MATCH_f.ADDR1) {
            addrMatched = 1;
        } else if (CW_I2C->MATCH_f.ADDR2) {
            addrMatched = 2;
        } else {
            addrMatched = 0xFF;
        }
        u8RecvCnt = 0;
        break;

    case 0x80: case 0x90:  // 收到1字节写数据
        if (u8RecvCnt == 0) {
            u8RegAddr = CW_I2C->DR;  // 第1个字节是寄存器地址
        } else {
            uint8_t data = CW_I2C->DR;
            
            if (addrMatched == 0) {  // 仅处理ADDR0地址的写入
              if(data<MODE_num)
              {
                enum CHxMode _mode = (enum CHxMode)data;
                switch (u8RegAddr) {
                  case 0x10: IO_set(CH1,_mode);break;   //io.ch[0].mode = _mode; 
                  case 0x11: IO_set(CH2,_mode);break;   //io.ch[1].mode = _mode; 
                  case 0x12: IO_set(CH3,_mode);break;   //io.ch[2].mode = _mode; 
                  case 0x13: IO_set(CH4,_mode);break;   //io.ch[3].mode = _mode; 
                  default: break;
                }
              } 
            } else if (addrMatched == 1) {
                // ADDR1 预留接口
            } else if (addrMatched == 2) {
                // ADDR2 预留接口
            }
        }
        u8RecvCnt++;
        break;

    case 0x88: case 0x98:  // 写数据 + NACK
        (void)CW_I2C->DR;
        CW_I2C->CR_f.STO = 1;
        CW_I2C->CR_f.SI = 0;
        return;

    case 0xA0:  // STOP or Repeated START received
        u8RecvCnt = 0;
        break;

    // 主机读取请求
    case 0xA8: case 0xB0:
        if (CW_I2C->MATCH_f.ADDR0) {
            addrMatched = 0;
        } else if (CW_I2C->MATCH_f.ADDR1) {
            addrMatched = 1;
        } else if (CW_I2C->MATCH_f.ADDR2) {
            addrMatched = 2;
        } else {
            addrMatched = 0xFF;
        }
        u8Cnt = 0;
        u8SendLen = 0;

        if (addrMatched == 0) {
            switch (u8RegAddr) {
            case 0x00:
                u8Senddata[0] = io.key;
                u8SendLen = 1;
                break;
            case 0x01:
                u8Senddata[0] = io.bat & 0xFF;
                u8Senddata[1] = (io.bat >> 8) & 0xFF;
                u8SendLen = 2;
                break;
            case 0x02:
                u8Senddata[0] = *(uint8_t*)&io.ch[0];
                u8Senddata[1] = *(uint8_t*)&io.ch[1];
                u8Senddata[2] = *(uint8_t*)&io.ch[2];
                u8Senddata[3] = *(uint8_t*)&io.ch[3];
                u8SendLen = 4;
                break;
            default:
                u8Senddata[0] = 0xFF;
                u8SendLen = 1;
                break;
            }
            CW_I2C->DR = u8Senddata[u8Cnt++];
        }  else if (addrMatched == 1) {
            u8Senddata[0] = io.key;
            u8Senddata[1] = io.bat & 0xFF;
            u8Senddata[2] = (io.bat >> 8) & 0xFF;
            u8Senddata[3] = *(uint8_t*)&io.ch[0];
            u8Senddata[4] = *(uint8_t*)&io.ch[1];
            u8Senddata[5] = *(uint8_t*)&io.ch[2];
            u8Senddata[6] = *(uint8_t*)&io.ch[3];
            u8SendLen = 7;
          
            CW_I2C->DR = u8Senddata[u8Cnt++];
        } else if (addrMatched == 2) {
            // ADDR2 预留接口
          
          CW_I2C->DR = 0;
        }

        break;

    case 0xB8:  // 已发送数据，ACK
        if (u8Cnt < u8SendLen) {
            CW_I2C->DR = u8Senddata[u8Cnt++];
        } else {
            CW_I2C->CR_f.AA = 0;  // 不再应答
        }
        break;

    case 0xC8:  // 最后一个字节ACK
        CW_I2C->CR_f.AA = 1;
        break;

    case 0xC0:  // NACK
    default:
        break;
    }

    CW_I2C->CR_f.SI = 0;
}


void I2C_Configuration(void)
{
  I2C_InitTypeDef I2C_InitStruct;

  __SYSCTRL_I2C_CLK_ENABLE();
  
  //I2C初始化
  I2C_InitStruct.PCLK_Freq = 48000000;
  I2C_InitStruct.I2C_SCL_Freq = 1000000;
  I2C_InitStruct.I2C_OwnSlaveAddr0 = I2C_SLAVEADDRESS;         //TEST ADDRESS0
  I2C_InitStruct.I2C_OwnSlaveAddr1 = I2C_SLAVEADDRESS1;  //TEST ADDRESS1
  I2C_InitStruct.I2C_OwnSlaveAddr2 = I2C_SLAVEADDRESS2;  //TEST ADDRESS2

  I2C_InitStruct.I2C_OwnGc = DISABLE;

  I2C_DeInit();
  I2C_Slave_Init(&I2C_InitStruct);//初始化模块
  CW_I2C->CR_f.FLT = 1;
  
  I2C_Cmd(ENABLE);//模块使能
  I2C_AcknowledgeConfig(ENABLE);//ACK打开
}
