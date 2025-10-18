// Time scaling specifications for ATmega329 counters and USART

/*
  Author: Martin Eden
  Last mod.: 2025-10-18
*/

/*
  [me_FrequencyCalculator] provides data structure and methods
  for hardware time scaling. Not data.

  Each module that uses this calculator should know it's specs.
*/

  /*
    Well, that won't work

    Counters module don't want this yet. It does not deal with
    frequencies, it provides interface to hardware counters.
    Which is wider scope but less abstractions.

    UART module don't need this yet. It uses frequency for speed
    setting. But speed setting is one of several things it does.
    It calculates frequency with similar formulas but not exposing
    abstraction for them.

    Frequency generator deals with frequencies. But it's
    implementation uses just one counter of three available.
    And it can't switch to another externally specified
    counter (yet?).

    So looks like it's gonna become another code heap which
    noone wants to host and maintain but everyone wants to use.
    Like [me_StreamsCollection].
  */

#pragma once

#include <me_FrequencyCalculator.h>

namespace me_FrequencySpecs
{
  me_FrequencyCalculator::THardwareDurationOptions GetSpec_Counter1();
  me_FrequencyCalculator::THardwareDurationOptions GetSpec_Counter2();
  me_FrequencyCalculator::THardwareDurationOptions GetSpec_Counter3();
  me_FrequencyCalculator::THardwareDurationOptions GetSpec_Uart();
}

/*
  Return counter 1 specs
*/
me_FrequencyCalculator::THardwareDurationOptions me_FrequencySpecs::GetSpec_Counter1()
{
  me_FrequencyCalculator::THardwareDurationOptions HwSpec;

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
me_FrequencyCalculator::THardwareDurationOptions me_FrequencySpecs::GetSpec_Counter2()
{
  me_FrequencyCalculator::THardwareDurationOptions HwSpec;

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
me_FrequencyCalculator::THardwareDurationOptions me_FrequencySpecs::GetSpec_Counter3()
{
  me_FrequencyCalculator::THardwareDurationOptions HwSpec;

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
me_FrequencyCalculator::THardwareDurationOptions me_FrequencySpecs::GetSpec_Uart()
{
  me_FrequencyCalculator::THardwareDurationOptions HwSpec;

  HwSpec.NumPrescalerValues = 2;
  HwSpec.PrescalerPowsOfTwo[0] = 3;
  HwSpec.PrescalerPowsOfTwo[1] = 4;

  HwSpec.CounterNumBits = 12;

  return HwSpec;
}

/*
  2025-10-18
*/
