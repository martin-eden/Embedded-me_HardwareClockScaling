// Time scaling specifications for ATmega329 counters and USART

/*
  Author: Martin Eden
  Last mod.: 2025-10-18
*/

/*
  [me_FrequencyCalculator] provides data structure and methods
  for hardware time scaling. Not data.

  Each module that uses that calculator should know it's specs.
  But for test purposes, we'll provide it here.
*/

#pragma once

#include <me_FrequencyCalculator.h>

namespace me_FrequencySpecs
{
  me_FrequencyCalculator::THardwareSpec GetSpec_Counter1();
  me_FrequencyCalculator::THardwareSpec GetSpec_Counter2();
  me_FrequencyCalculator::THardwareSpec GetSpec_Counter3();
  me_FrequencyCalculator::THardwareSpec GetSpec_Uart();
}

/*
  Return counter 1 specs
*/
me_FrequencyCalculator::THardwareSpec me_FrequencySpecs::GetSpec_Counter1()
{
  me_FrequencyCalculator::THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 5;
  HwSpec.PrescalerPowsOfTwo[0] = 0;
  HwSpec.PrescalerPowsOfTwo[1] = 3;
  HwSpec.PrescalerPowsOfTwo[2] = 6;
  HwSpec.PrescalerPowsOfTwo[3] = 8;
  HwSpec.PrescalerPowsOfTwo[4] = 10;

  HwSpec.CounterNumBits = 8;

  return HwSpec;
}

/*
  Return counter 2 specs
*/
me_FrequencyCalculator::THardwareSpec me_FrequencySpecs::GetSpec_Counter2()
{
  me_FrequencyCalculator::THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 5;
  HwSpec.PrescalerPowsOfTwo[0] = 0;
  HwSpec.PrescalerPowsOfTwo[1] = 3;
  HwSpec.PrescalerPowsOfTwo[2] = 6;
  HwSpec.PrescalerPowsOfTwo[3] = 8;
  HwSpec.PrescalerPowsOfTwo[4] = 10;

  HwSpec.CounterNumBits = 16;

  return HwSpec;
}

/*
  Return counter 3 specs
*/
me_FrequencyCalculator::THardwareSpec me_FrequencySpecs::GetSpec_Counter3()
{
  me_FrequencyCalculator::THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 7;
  HwSpec.PrescalerPowsOfTwo[0] = 0;
  HwSpec.PrescalerPowsOfTwo[1] = 3;
  HwSpec.PrescalerPowsOfTwo[2] = 5;
  HwSpec.PrescalerPowsOfTwo[3] = 6;
  HwSpec.PrescalerPowsOfTwo[4] = 7;
  HwSpec.PrescalerPowsOfTwo[5] = 8;
  HwSpec.PrescalerPowsOfTwo[6] = 10;

  HwSpec.CounterNumBits = 8;

  return HwSpec;
}

/*
  Return USART module specs
*/
me_FrequencyCalculator::THardwareSpec me_FrequencySpecs::GetSpec_Uart()
{
  me_FrequencyCalculator::THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 2;
  HwSpec.PrescalerPowsOfTwo[0] = 3;
  HwSpec.PrescalerPowsOfTwo[1] = 4;

  HwSpec.CounterNumBits = 12;

  return HwSpec;
}

/*
  2025-10-18
*/
