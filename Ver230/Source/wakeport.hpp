//----------------------------------------------------------------------------

//Wake protocol based port class, header file

//----------------------------------------------------------------------------

#ifndef WAKEPORT_HPP
#define WAKEPORT_HPP

//----------------------------------------------------------------------------

#include "wake.hpp"

//----------------------------------------------------------------------------
//--------------------------- TWakePort class: -------------------------------
//----------------------------------------------------------------------------

extern "C" void USART1_IRQHandler(void);

class TWakePort : public TWake
{
private:
  Pin_TXD1_t Pin_TXD;
  Pin_RXD1_t Pin_RXD;
  static TWakePort *Wp;
  friend void USART1_IRQHandler(void);
protected:
public:
  TWakePort(uint32_t baud, uint8_t frame);
  void StartTx(uint8_t cmd);
};

//----------------------------------------------------------------------------

#endif
