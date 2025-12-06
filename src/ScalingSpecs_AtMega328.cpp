// Time scaling specifications for ATmega329 counters and USART

/*
  Author: Martin Eden
  Last mod.: 2025-12-06
*/

#include <me_HardwareClockScaling.h>

using namespace me_HardwareClockScaling;

/*
  Return counter 1 specs
*/
TClockScalingOptions AtMega328::GetSpecs_Counter1()
{
  return
    {
      .NumPrescalerValues = 5,
      .Prescales_PowOfTwo = { 0, 3, 6, 8, 10 },
      .CounterNumBits = 8,
    };
}

/*
  Return counter 2 specs
*/
TClockScalingOptions AtMega328::GetSpecs_Counter2()
{
  return
    {
      .NumPrescalerValues = 5,
      .Prescales_PowOfTwo = { 0, 3, 6, 8, 10 },
      .CounterNumBits = 16,
    };
}

/*
  Return counter 3 specs
*/
TClockScalingOptions AtMega328::GetSpecs_Counter3()
{
  return
    {
      .NumPrescalerValues = 7,
      .Prescales_PowOfTwo = { 0, 3, 5, 6, 7, 8, 10 },
      .CounterNumBits = 8,
    };
}

/*
  Return USART module specs
*/
TClockScalingOptions AtMega328::GetSpecs_Uart()
{
  return
    {
      .NumPrescalerValues = 2,
      .Prescales_PowOfTwo = { 3, 4 },
      .CounterNumBits = 12,
    };
}

/*
  2025-10-18
  2025-12-06
*/
