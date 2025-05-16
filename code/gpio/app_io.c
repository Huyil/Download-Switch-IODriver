#include "app_io.h"
#include "app_ioMAP.h"

SWITCH_STATE io;

static  int8_t GetLUTData(IOData* returnIO) {
  
  returnIO->address =  ( (io.ch1) << 0 ) | ( (io.ch2) << 3 )| ( (io.ch3) << 6 )| ( (io.ch4) << 9 );
  uint16_t len = sizeof(ioMap)/sizeof(IOData);
  for(uint16_t i=0;i < len;i++)
  {
    if(returnIO->address == ioMap[i].address)
    {
      returnIO->outPins = ioMap[i].outPins;
      return 0;
    }
  }
  return 1;
}

int8_t IO_Refresh(void)
{
  IOData nowIOState;
  if(GetLUTData(&nowIOState))
  {
    nowIOState.outPins = 0x3F00;
  }
  
  io.io.P1     = nowIOState.P1;
  io.io.N1     = nowIOState.N1;
  io.io.P2     = nowIOState.P2;
  io.io.N2     = nowIOState.N2;
  io.io.P3     = nowIOState.P3;
  io.io.N3     = nowIOState.N3;
  io.io.P4     = nowIOState.P4;
  io.io.N4     = nowIOState.N4;
  io.io.CLK_EN = nowIOState.CLK_EN;
  io.io.CLK_A  = nowIOState.CLK_A;
  io.io.CLK_B  = nowIOState.CLK_B;
  io.io.DIO_EN = nowIOState.DIO_EN;
  io.io.DIO_A  = nowIOState.DIO_A;
  io.io.DIO_B  = nowIOState.DIO_B;

  CH1P(io.io.P1);
  CH1N(io.io.N1);
  CH2P(io.io.P2);
  CH2N(io.io.N2);
  CH3P(io.io.P3);
  CH3N(io.io.N3);
  CH4P(io.io.P4);
  CH4N(io.io.N4);
  SCK_EN(io.io.CLK_EN);
  SCK_A(io.io.CLK_A);
  SCK_B(io.io.CLK_B);
  DIO_EN(io.io.DIO_EN);
  DIO_A(io.io.DIO_A);
  DIO_B(io.io.DIO_B);

  if(io.ch1 == VCC || io.ch2 == VCC || io.ch3 == VCC || io.ch4 == VCC)
  {
    VOUT_EN(1);
  }else VOUT_EN(0);

  return 0;
}

int8_t IO_set(enum CHx _ch,enum CHxMode _mode)
{
  if(_mode >= MODE_num || _ch >= CH_num)
  {
    //error 
    return 1;
  }
  
  io.ch[_ch].mode = _mode;
  
  IO_Refresh();
  return 0;
}


int8_t IO_get(enum CHx _ch,io_one* _io)
{
  if(_ch >= CH_num)
  {
    //error 
    return 1;
  }
  
  if(_io != 0)
  {
    _io->mode = io.ch[_ch].mode;
    _io->pin = io.ch[_ch].pin;
  }
  return 0;
}
