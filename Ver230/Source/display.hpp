//----------------------------------------------------------------------------

//LED display support class, header file

//----------------------------------------------------------------------------

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

//----------------------------------------------------------------------------

#include "systimer.hpp"
#include "sreg.hpp"

//----------------------------- Constants: -----------------------------------

//channels, channels number:
enum { LEFT, RIGHT, CHANS };

//registers outputs:
enum Outs_t
{ Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7 };

//LED masks:
enum
{
  TRNON = 0x0000,
  TR0   = 0x0001,
  TR1   = 0x0002,
  TR2   = 0x0004,
  TR3   = 0x0008,
  TR4   = 0x0010,
  TR5   = 0x0020,
  TR6   = 0x0040,
  TR7   = 0x0080,
  TR8   = 0x0100,
  TR9   = 0x0200,
  SCALE = 0x0400,
  TRALL = 0x03FF,
  TSALL = 0x07FF
};

//----------------------------------------------------------------------------
//------------------------------ METER50: ------------------------------------
//----------------------------------------------------------------------------

#ifdef METER50
//bar segments count:
static const uint8_t SEGS = 50;

//LEDs, LEDs count:
enum { T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, SCA, TRS };

//registers:
enum Regs_t
{ U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, REGS };

//register-output structure:
struct Seg_t {
  Regs_t r;
  Outs_t q;
};

//assignment of bar segments (left):
static const Seg_t SegsL[SEGS] =
{
  {U1,  Q2}, /*L01*/ {U1,  Q1}, /*L02*/ {U2,  Q0}, /*L03*/ {U2,  Q3}, /*L04*/
  {U2,  Q2}, /*L05*/ {U2,  Q1}, /*L06*/ {U3,  Q0}, /*L07*/ {U3,  Q3}, /*L08*/
  {U3,  Q2}, /*L09*/ {U3,  Q1}, /*L10*/ {U4,  Q0}, /*L11*/ {U4,  Q3}, /*L12*/
  {U4,  Q2}, /*L13*/ {U4,  Q1}, /*L14*/ {U5,  Q0}, /*L15*/ {U5,  Q3}, /*L16*/
  {U5,  Q2}, /*L17*/ {U5,  Q1}, /*L18*/ {U6,  Q0}, /*L19*/ {U6,  Q3}, /*L20*/
  {U6,  Q2}, /*L21*/ {U6,  Q1}, /*L22*/ {U7,  Q0}, /*L23*/ {U7,  Q3}, /*L24*/
  {U7,  Q2}, /*L25*/ {U7,  Q1}, /*L26*/ {U8,  Q0}, /*L27*/ {U8,  Q3}, /*L28*/
  {U8,  Q2}, /*L29*/ {U8,  Q1}, /*L30*/ {U9,  Q0}, /*L31*/ {U9,  Q3}, /*L32*/
  {U9,  Q2}, /*L33*/ {U9,  Q1}, /*L34*/ {U10, Q0}, /*L35*/ {U10, Q3}, /*L36*/
  {U10, Q2}, /*L37*/ {U10, Q1}, /*L38*/ {U11, Q0}, /*L39*/ {U11, Q3}, /*L40*/
  {U11, Q2}, /*L41*/ {U11, Q1}, /*L42*/ {U12, Q0}, /*L43*/ {U12, Q3}, /*L44*/
  {U12, Q2}, /*L45*/ {U12, Q1}, /*L46*/ {U13, Q0}, /*L47*/ {U13, Q3}, /*L48*/
  {U13, Q2}, /*L49*/ {U13, Q1}, /*L50*/
};

//assignment of bar segments (right):
static const Seg_t SegsR[SEGS] =
{
  {U1,  Q7}, /*R01*/ {U1,  Q6}, /*R02*/ {U2,  Q4}, /*R03*/ {U2,  Q5}, /*R04*/
  {U2,  Q7}, /*R05*/ {U2,  Q6}, /*R06*/ {U3,  Q4}, /*R07*/ {U3,  Q5}, /*R08*/
  {U3,  Q7}, /*R09*/ {U3,  Q6}, /*R10*/ {U4,  Q4}, /*R11*/ {U4,  Q5}, /*R12*/
  {U4,  Q7}, /*R13*/ {U4,  Q6}, /*R14*/ {U5,  Q4}, /*R15*/ {U5,  Q5}, /*R16*/
  {U5,  Q7}, /*R17*/ {U5,  Q6}, /*R18*/ {U6,  Q4}, /*R19*/ {U6,  Q5}, /*R20*/
  {U6,  Q7}, /*R21*/ {U6,  Q6}, /*R22*/ {U7,  Q4}, /*R23*/ {U7,  Q5}, /*R24*/
  {U7,  Q7}, /*R25*/ {U7,  Q6}, /*R26*/ {U8,  Q4}, /*R27*/ {U8,  Q5}, /*R28*/
  {U8,  Q7}, /*R29*/ {U8,  Q6}, /*R30*/ {U9,  Q4}, /*R31*/ {U9,  Q5}, /*R32*/
  {U9,  Q7}, /*R33*/ {U9,  Q6}, /*R34*/ {U10, Q4}, /*R35*/ {U10, Q5}, /*R36*/
  {U10, Q7}, /*R37*/ {U10, Q6}, /*R37*/ {U11, Q4}, /*R39*/ {U11, Q5}, /*R40*/
  {U11, Q7}, /*R41*/ {U11, Q6}, /*R42*/ {U12, Q4}, /*R43*/ {U12, Q5}, /*R44*/
  {U12, Q7}, /*R45*/ {U12, Q6}, /*R46*/ {U13, Q4}, /*R47*/ {U13, Q5}, /*R48*/
  {U13, Q7}, /*R49*/ {U13, Q6}, /*R50*/
};

//assignment of LEDs:
static const Seg_t Transp[TRS] =
{
  {U1,  Q5}, /*T0*/ {U1,  Q4}, /*T1*/ {U1,  Q3}, /*T2*/ {U14, Q3}, /*T3*/
  {U14, Q4}, /*T4*/ {U14, Q5}, /*T5*/ {U14, Q6}, /*T6*/ {U14, Q7}, /*T7*/
  {U14, Q2}, /*T8*/ {U14, Q1}, /*T9*/ {U14, Q0} /*SCALE*/
};
#endif

//----------------------------------------------------------------------------
//------------------------------ METER40: ------------------------------------
//----------------------------------------------------------------------------

#ifdef METER40
//bar segments count:
static const uint8_t SEGS = 40;

//LEDs, LEDs count:
enum { TRS };

//registers:
enum Regs_t
{ U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, REGS };

//register-output structure:
struct Seg_t {
  Regs_t r;
  Outs_t q;
};

//assignment of bar segments (left):
static const Seg_t SegsL[SEGS] =
{
  {U1,  Q0}, /*L01*/ {U1,  Q3}, /*L02*/ {U1,  Q2}, /*L03*/ {U1,  Q1}, /*L04*/
  {U2,  Q0}, /*L05*/ {U2,  Q3}, /*L06*/ {U2,  Q2}, /*L07*/ {U2,  Q1}, /*L08*/
  {U3,  Q0}, /*L09*/ {U3,  Q3}, /*L10*/ {U3,  Q2}, /*L11*/ {U3,  Q1}, /*L12*/
  {U4,  Q0}, /*L13*/ {U4,  Q3}, /*L14*/ {U4,  Q2}, /*L15*/ {U4,  Q1}, /*L16*/
  {U5,  Q0}, /*L17*/ {U5,  Q3}, /*L18*/ {U5,  Q2}, /*L19*/ {U5,  Q1}, /*L20*/
  {U6,  Q0}, /*L21*/ {U6,  Q3}, /*L22*/ {U6,  Q2}, /*L23*/ {U6,  Q1}, /*L24*/
  {U7,  Q0}, /*L25*/ {U7,  Q3}, /*L26*/ {U7,  Q2}, /*L27*/ {U7,  Q1}, /*L28*/
  {U8,  Q0}, /*L29*/ {U8,  Q3}, /*L30*/ {U8,  Q2}, /*L31*/ {U8,  Q1}, /*L32*/
  {U9,  Q0}, /*L33*/ {U9,  Q3}, /*L34*/ {U9,  Q2}, /*L35*/ {U9,  Q1}, /*L36*/
  {U10, Q0}, /*L37*/ {U10, Q3}, /*L38*/ {U10, Q2}, /*L39*/ {U10, Q1}, /*L40*/
};

//assignment of bar segments (right):
static const Seg_t SegsR[SEGS] =
{
  {U1,  Q4}, /*R01*/ {U1,  Q5}, /*R02*/ {U1,  Q7}, /*R03*/ {U1,  Q6}, /*R04*/
  {U2,  Q4}, /*R05*/ {U2,  Q5}, /*R06*/ {U2,  Q7}, /*R07*/ {U2,  Q6}, /*R08*/
  {U3,  Q4}, /*R09*/ {U3,  Q5}, /*R10*/ {U3,  Q7}, /*R11*/ {U3,  Q6}, /*R12*/
  {U4,  Q4}, /*R13*/ {U4,  Q5}, /*R14*/ {U4,  Q7}, /*R15*/ {U4,  Q6}, /*R16*/
  {U5,  Q4}, /*R17*/ {U5,  Q5}, /*R18*/ {U5,  Q7}, /*R19*/ {U5,  Q6}, /*R20*/
  {U6,  Q4}, /*R21*/ {U6,  Q5}, /*R22*/ {U6,  Q7}, /*R23*/ {U6,  Q6}, /*R24*/
  {U7,  Q4}, /*R25*/ {U7,  Q5}, /*R26*/ {U7,  Q7}, /*R27*/ {U7,  Q6}, /*R28*/
  {U8,  Q4}, /*R29*/ {U8,  Q5}, /*R30*/ {U8,  Q7}, /*R31*/ {U8,  Q6}, /*R32*/
  {U9,  Q4}, /*R33*/ {U9,  Q5}, /*R34*/ {U9,  Q7}, /*R35*/ {U9,  Q6}, /*R36*/
  {U10, Q4}, /*R37*/ {U10, Q5}, /*R37*/ {U10, Q7}, /*R39*/ {U10, Q6}, /*R40*/
};

#endif

//----------------------------------------------------------------------------
//------------------------------ METER35: ------------------------------------
//----------------------------------------------------------------------------

#ifdef METER35
//bar segments count:
static const uint8_t SEGS = 35;

//LEDs, LEDs count:
enum { SCA, TRS };

//registers:
enum Regs_t
{ U1, U2, U3, U4, U5, U6, U7, U8, U9, REGS };

//register-output structure:
struct Seg_t {
  Regs_t r;
  Outs_t q;
};

//assignment of bar segments (left):
static const Seg_t SegsL[SEGS] =
{
  {U1,  Q0}, /*L01*/ {U1,  Q3}, /*L02*/ {U1,  Q2}, /*L03*/ {U1,  Q1}, /*L04*/
  {U2,  Q0}, /*L05*/ {U2,  Q3}, /*L06*/ {U2,  Q2}, /*L07*/ {U2,  Q1}, /*L08*/
  {U3,  Q0}, /*L09*/ {U3,  Q3}, /*L10*/ {U3,  Q2}, /*L11*/ {U3,  Q1}, /*L12*/
  {U4,  Q0}, /*L13*/ {U4,  Q3}, /*L14*/ {U4,  Q2}, /*L15*/ {U4,  Q1}, /*L16*/
  {U5,  Q0}, /*L17*/ {U5,  Q3}, /*L18*/ {U5,  Q2}, /*L19*/ {U5,  Q1}, /*L20*/
  {U6,  Q0}, /*L21*/ {U6,  Q3}, /*L22*/ {U6,  Q2}, /*L23*/ {U6,  Q1}, /*L24*/
  {U7,  Q0}, /*L25*/ {U7,  Q3}, /*L26*/ {U7,  Q2}, /*L27*/ {U7,  Q1}, /*L28*/
  {U8,  Q0}, /*L29*/ {U8,  Q3}, /*L30*/ {U8,  Q2}, /*L31*/ {U8,  Q1}, /*L32*/
  {U9,  Q0}, /*L33*/ {U9,  Q3}, /*L34*/ {U9,  Q2}, /*L35*/
};

//assignment of bar segments (right):
static const Seg_t SegsR[SEGS] =
{
  {U1,  Q4}, /*R01*/ {U1,  Q5}, /*R02*/ {U1,  Q7}, /*R03*/ {U1,  Q6}, /*R04*/
  {U2,  Q4}, /*R05*/ {U2,  Q5}, /*R06*/ {U2,  Q7}, /*R07*/ {U2,  Q6}, /*R08*/
  {U3,  Q4}, /*R09*/ {U3,  Q5}, /*R10*/ {U3,  Q7}, /*R11*/ {U3,  Q6}, /*R12*/
  {U4,  Q4}, /*R13*/ {U4,  Q5}, /*R14*/ {U4,  Q7}, /*R15*/ {U4,  Q6}, /*R16*/
  {U5,  Q4}, /*R17*/ {U5,  Q5}, /*R18*/ {U5,  Q7}, /*R19*/ {U5,  Q6}, /*R20*/
  {U6,  Q4}, /*R21*/ {U6,  Q5}, /*R22*/ {U6,  Q7}, /*R23*/ {U6,  Q6}, /*R24*/
  {U7,  Q4}, /*R25*/ {U7,  Q5}, /*R26*/ {U7,  Q7}, /*R27*/ {U7,  Q6}, /*R28*/
  {U8,  Q4}, /*R29*/ {U8,  Q5}, /*R30*/ {U8,  Q7}, /*R31*/ {U8,  Q6}, /*R32*/
  {U9,  Q4}, /*R33*/ {U9,  Q5}, /*R34*/ {U9,  Q7}, /*R35*/
};

//assignment of LEDs:
static const Seg_t Transp[TRS] =
{
  {U9, Q1} /*SCALE*/
};
#endif

//----------------------------------------------------------------------------
//---------------------------- TDisplay class: -------------------------------
//----------------------------------------------------------------------------

class TDisplay
{
private:
  TSreg Sreg;
#ifdef METER40
  Pin_BKL_t Pin_BKL;
#endif
  uint8_t RegData[REGS]; //registers copy
public:
  TDisplay(void);
  void Clear(void);      //display clear
  void Update(void);     //display update
  void SegL(uint8_t s, bool on); //segment switch on for L channel
  void BarL(uint8_t n);  //bar switch on for L channel
  void SegR(uint8_t s, bool on); //segment switch on  R channel
  void BarR(uint8_t n);  //bar switch on for R channel
  void Leds(uint16_t s); //LEDs switch on
};

//----------------------------------------------------------------------------

#endif
