//----------------------------------------------------------------------------

//Wake protocol class

//----------------------------------------------------------------------------

#include "main.hpp"
#include "wake.hpp"

//------------------------ Compilation Options: ------------------------------

#define TABLE_CRC //enable tabular CRC calculation

//----------------------------------------------------------------------------
//----------------------------- TWake class: ---------------------------------
//----------------------------------------------------------------------------

//----------------------------- Constructor: ---------------------------------

TWake::TWake(uint8_t frame)
{
  Frame = frame;
  RxData = new uint8_t[frame + PTR_DAT + 1]; //RX buffewr
  TxData = new uint8_t[frame + PTR_DAT + 1]; //TX buffer
  Addr = 0;
  TxState = WST_DONE;
  RxState = WST_IDLE;
}

//--------------------------- CRC calculation: -------------------------------

void TWake::Do_Crc8(uint8_t b, uint8_t *crc)
{
#ifdef TABLE_CRC
  static const uint8_t CrcTable[256] =
  {
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
    0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
    0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E,
    0x5F, 0x01, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC,
    0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0,
    0xE1, 0xBF, 0x5D, 0x03, 0x80, 0xDE, 0x3C, 0x62,
    0xBE, 0xE0, 0x02, 0x5C, 0xDF, 0x81, 0x63, 0x3D,
    0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF,
    0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5,
    0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x07,
    0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x06, 0x58,
    0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A,
    0x65, 0x3B, 0xD9, 0x87, 0x04, 0x5A, 0xB8, 0xE6,
    0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24,
    0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B,
    0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x05, 0xE7, 0xB9,
    0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0x0F,
    0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD,
    0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92,
    0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0x0E, 0x50,
    0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C,
    0x6D, 0x33, 0xD1, 0x8F, 0x0C, 0x52, 0xB0, 0xEE,
    0x32, 0x6C, 0x8E, 0xD0, 0x53, 0x0D, 0xEF, 0xB1,
    0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73,
    0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49,
    0x08, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B,
    0x57, 0x09, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4,
    0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16,
    0xE9, 0xB7, 0x55, 0x0B, 0x88, 0xD6, 0x34, 0x6A,
    0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8,
    0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7,
    0xB6, 0xE8, 0x0A, 0x54, 0xD7, 0x89, 0x6B, 0x35
  };
  *crc = CrcTable[*crc ^ b]; //table calculation
#else
  for(uint8_t i = 0; i < 8; b = b >> 1, i++) //loop calculation
    if((b ^ *crc) & 1) *crc = ((*crc ^ 0x18) >> 1) | 0x80;
     else *crc = (*crc >> 1) & ~0x80;
#endif
}

//----------------------------------------------------------------------------
//---------------------------- Packet receive: -------------------------------
//----------------------------------------------------------------------------

//---------------------------- Receive byte: ---------------------------------

void TWake::Rx(uint8_t data)
{
  if(RxState != WST_DONE)            //if receive is enabled
  {
    if(data == FEND)                 //FEND found (from any state)
    {
      RxState = WST_ADD;             //start to receive address
      RxPtr = RxData;                //pointer to the beginning of the buffer
      RxStuff = 0; return;           //no destaffing
    }
    if(data == FESC)                 //FESC received,
    { RxStuff = 1; return; }         //start of destaffing
    if(RxStuff)                      //if there is destafing,
    {
      if(data == TFESC)              //if TFESC received,
        data = FESC;                 //replacing it with FESC
      else if(data == TFEND)         //if TFEND received,
        data = FEND;                 //replacing it with FEND
        else { RxState = WST_IDLE; return; } //otherwise stuffing error
      RxStuff = 0;                   //destaffing completed
    }
    switch(RxState)
    {
    case WST_ADD:                    //receiving address
        RxState = WST_CMD;           //next - receiving command
        if(data & 0x80)              //if an address is received,
        {
          data &= ~0x80;             //restoring the address value
          if(data != Addr)           //address did not match,
          { RxState = WST_IDLE; return; } //start searching for FEND
          break;                     //address storing
        }
        else *RxPtr++ = 0;           //storing zero address
    case WST_CMD:                    //receive command code
        RxState = WST_LNG;           //next - receiving packet length
        break;                       //command storing
    case WST_LNG:                    //packet length is being received
        RxState = WST_DATA;          //next - receiving data
        if(data > Frame) data = Frame;   //packet length limit
        RxEnd = RxData + PTR_DAT + data; //pointer to end of data
        break;
    case WST_DATA:                   //data is being received
        if(RxPtr == RxEnd)           //if all data and CRC are received,
          RxState = WST_DONE;        //receiving is over
        break;
    default: return;
    }
    *RxPtr++ = data;                 //storing data in a buffer
  }
}

//------------------- Returns the current command code: ----------------------

uint8_t TWake::GetCmd(void)
{
  uint8_t cmd = CMD_NOP;
  if(RxState == WST_DONE)            //if the packet is received
  {
    RxCount = RxEnd - RxData - PTR_DAT; //number of data bytes received
    uint8_t crc = CRC_FEND;          //CRC init
    RxPtr = RxData;                  //pointer to the beginning of the buffer
    if(!*RxPtr) RxPtr++;             //if the address is null, skip it
    while(RxPtr <= RxEnd)            //for the entire buffer
      Do_Crc8(*RxPtr++, &crc);       //calculate CRC
    RxPtr = RxData + PTR_CMD;        //command code pointer
    if(!crc) cmd = *RxPtr;           //if the CRC matches, the command code
      else  cmd = CMD_ERR;           //otherwise error code
    TxCount = 0;                     //zeroing the number of bytes to send
    RxPtr = RxData + PTR_DAT;        //data receive pointer
    TxPtr = TxData + PTR_DAT;        //data transmit pointer
  }
  return(cmd);
}

//------------------ Returns the number of bytes received: -------------------

uint8_t TWake::GetRxCount(void)
{
  return(RxCount);
}

//------------------ Sets a pointer to the receive buffer: -------------------

void TWake::SetRxPtr(uint8_t p)
{
  if(p < Frame)
    RxPtr = RxData + PTR_DAT + p;
}

//-------------------- Reads the receive buffer pointer: ---------------------

uint8_t TWake::GetRxPtr(void)
{
  return(RxPtr - RxData - PTR_DAT);
}

//------------------ Reads a byte from the receive buffer: -------------------

uint8_t TWake::GetByte(void)
{
  return(*RxPtr++);
}

//------------------ Reads a word from the receive buffer: -------------------

int16_t TWake::GetWord(void)
{
  uint8_t l = *RxPtr++;
  uint8_t h = *RxPtr++;
  return(WORD(h, l));
}

//--------------- Reads a double word from the receive buffer: ---------------

int32_t TWake::GetDWord(void)
{
  uint8_t b1 = *RxPtr++;
  uint8_t b2 = *RxPtr++;
  uint8_t b3 = *RxPtr++;
  uint8_t b4 = *RxPtr++;
  return(DWORD(b4, b3, b2, b1));
}

//------------------- Reads data from the receive buffer: --------------------

void TWake::GetData(uint8_t *d, uint8_t count)
{
  for(uint8_t i = 0; i < count; i++)
    *d++ = *RxPtr++;
}

//----------------------------------------------------------------------------
//-------------------------- Packet transmit: --------------------------------
//----------------------------------------------------------------------------

//----------------- Sets a pointer to the transmit buffer: -------------------

void TWake::SetTxPtr(uint8_t p)
{
  if(p < Frame)
    TxPtr = TxData + PTR_DAT + p;
}

//------------------- Reads the transmit buffer pointer: ---------------------

uint8_t TWake::GetTxPtr(void)
{
  return(TxPtr - TxData - PTR_DAT);
}

//------------------- Puts a byte in the transmit buffer: --------------------

void TWake::AddByte(uint8_t b)
{
  if(TxPtr < TxData + PTR_DAT + Frame)
    *TxPtr++ = b;
}

//------------------ Puts a word in the transmit buffer: ---------------------

void TWake::AddWord(int16_t w)
{
  if(TxPtr < TxData + PTR_DAT + Frame - 1)
  {
    *TxPtr++ = LO(w);
    *TxPtr++ = HI(w);
  }
}

//--------------- Puts a double word in the transmit buffer: -----------------

void TWake::AddDWord(int32_t dw)
{
  if(TxPtr < TxData + PTR_DAT + Frame - 3)
  {
    *TxPtr++ = BYTE1(dw);
    *TxPtr++ = BYTE2(dw);
    *TxPtr++ = BYTE3(dw);
    *TxPtr++ = BYTE4(dw);
  }
}

//-------------------- Puts data in the transmit buffer: ---------------------

void TWake::AddData(uint8_t *d, uint8_t count)
{
  if(TxPtr <= (TxData + PTR_DAT + Frame) - count)
    for(uint8_t i = 0; i < count; i++)
      *TxPtr++ = *d++;
}

//----------------------- Start of packet transmission: ----------------------

void TWake::TxStart(uint8_t cmd, uint8_t &data)
{
  TxEnd = TxPtr;                     //end-of-packet pointer
  TxCount = TxPtr - TxData - PTR_DAT; //number of bytes to transmit
  TxPtr = TxData;                    //pointer to the beginning of the buffer
  *TxPtr++ = Addr | 0x80;            //adding address to buffer
  *TxPtr++ = cmd;                    //adding command code to buffer
  *TxPtr = TxCount;                  //adding packet size to buffer
  uint8_t crc = CRC_FEND;            //CRC init
  TxPtr = TxData;                    //pointer to the beginning of the buffer
  if(!Addr) TxPtr++;                 //skip null address
  while(TxPtr < TxEnd)
    Do_Crc8(*TxPtr++, &crc);         //CRC calculation for the entire buffer
  *TxPtr = crc;                      //adding CRC to buffer
  TxPtr = TxData;                    //pointer to the beginning of the buffer
  if(!Addr) TxPtr++;                 //skip null address
  TxStuff = 0;                       //no staffing
  RxState = WST_IDLE;                //packet receive enabling
  TxState = WST_DATA;                //data transfer state
  data = FEND;
}

//---------------------------- Byte transmit: --------------------------------

bool TWake::Tx(uint8_t &data)
{
  if(TxState == WST_DATA)            //if data is being transmitted,
  {
    data = *TxPtr++;                 //then reading a byte from the buffer
    if(data == FEND || data == FESC) //attempt to send FEND or FESC,
      if(!TxStuff)                   //need staffing
      {
        data = FESC;                 //transmit FESC
        TxStuff = 1;                 //start of staffing
        TxPtr--;                     //return to the same byte
      }
      else
      {
        if(data == FEND) data = TFEND; //transmit TFEND
          else data = TFESC;         //or TFESC
        TxStuff = 0;                 //end of staffing
      }
    if(TxPtr > TxEnd)                //if the end of the buffer is reached,
      TxState = WST_CRC;             //transmit CRC
    return(1);
  }
  else                               //if the transmit is completed,
  {
    TxState = WST_DONE;              //packet transmit completed
    return(0);
  }
}

//------------- Determining the end of a packet transmission: ----------------

bool TWake::AskTxEnd(void)
{
  return(TxState == WST_DONE);
}

//----------------------------------------------------------------------------
