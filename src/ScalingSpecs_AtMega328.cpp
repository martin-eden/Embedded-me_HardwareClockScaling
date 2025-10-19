// Time scaling specifications for ATmega329 counters and USART

/*
  Author: Martin Eden
  Last mod.: 2025-10-19
*/

#include <me_HardwareClockScaling.h>

using namespace me_HardwareClockScaling;

/*
  Return counter 1 specs
*/
TClockScalingOptions AtMega328::GetSpec_Counter1()
{
  TClockScalingOptions HwSpec;

  HwSpec.NumPrescalerValues = 5;
  HwSpec.Prescales_PowOfTwo[0] = 0;
  HwSpec.Prescales_PowOfTwo[1] = 3;
  HwSpec.Prescales_PowOfTwo[2] = 6;
  HwSpec.Prescales_PowOfTwo[3] = 8;
  HwSpec.Prescales_PowOfTwo[4] = 10;

  HwSpec.CounterNumBits = 8;

  return HwSpec;
}

/*
  Return counter 2 specs
*/
TClockScalingOptions AtMega328::GetSpec_Counter2()
{
  TClockScalingOptions HwSpec;

  HwSpec.NumPrescalerValues = 5;
  HwSpec.Prescales_PowOfTwo[0] = 0;
  HwSpec.Prescales_PowOfTwo[1] = 3;
  HwSpec.Prescales_PowOfTwo[2] = 6;
  HwSpec.Prescales_PowOfTwo[3] = 8;
  HwSpec.Prescales_PowOfTwo[4] = 10;

  HwSpec.CounterNumBits = 16;

  return HwSpec;
}

/*
  Return counter 3 specs
*/
TClockScalingOptions AtMega328::GetSpec_Counter3()
{
  TClockScalingOptions HwSpec;

  HwSpec.NumPrescalerValues = 7;
  HwSpec.Prescales_PowOfTwo[0] = 0;
  HwSpec.Prescales_PowOfTwo[1] = 3;
  HwSpec.Prescales_PowOfTwo[2] = 5;
  HwSpec.Prescales_PowOfTwo[3] = 6;
  HwSpec.Prescales_PowOfTwo[4] = 7;
  HwSpec.Prescales_PowOfTwo[5] = 8;
  HwSpec.Prescales_PowOfTwo[6] = 10;

  HwSpec.CounterNumBits = 8;

  return HwSpec;
}

/*
  Return USART module specs
*/
TClockScalingOptions AtMega328::GetSpec_Uart()
{
  TClockScalingOptions HwSpec;

  HwSpec.NumPrescalerValues = 2;
  HwSpec.Prescales_PowOfTwo[0] = 3;
  HwSpec.Prescales_PowOfTwo[1] = 4;

  HwSpec.CounterNumBits = 12;

  return HwSpec;
}

/*
  2025-10-18
*/
