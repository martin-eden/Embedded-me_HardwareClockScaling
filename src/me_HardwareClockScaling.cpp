// Convert frequency to hardware duration. Implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-19
*/

/*
  Imagine we can do only 10 ms delays and asked for 27 ms delay.

  We can't do it exactly, so our options are:

    * Fail because we can't do it exactly
    * Do 20 ms delay, we never overshoot
    * Do 30 ms delay because it's closes match

  This implementation is doing closest match calculations.
*/

#include <me_HardwareClockScaling.h>

#include <me_BaseTypes.h>

using namespace me_HardwareClockScaling;

const TUint_4 BaseFreq = F_CPU;

// [Internal] Check hardware spec
static TBool CheckSpec(
  THardwareDurationOptions * Spec
)
{
  TUint_1 Index;

  if (Spec->NumPrescalerValues == 0)
    return false;

  if (Spec->CounterNumBits > 16)
    return false;

  for (Index = 0; Index < Spec->NumPrescalerValues; ++Index)
  {
    if (Spec->PrescalerPowsOfTwo[Index] > 16)
      return false;

    if (Index > 0)
      if (Spec->PrescalerPowsOfTwo[Index] <= Spec->PrescalerPowsOfTwo[Index - 1])
        return false;
  }

  return true;
}

/*
  [Internal] Calculate how many units can be fit in given length

  Rounds result to nearest integer value.
*/
static TBool GetNumUnitsForLength(
  TUint_4 * NumUnits,
  TUint_4 Length,
  TUint_4 UnitSize
)
{
  if (Length == 0)
    return false;

  if (UnitSize == 0)
    return false;

  *NumUnits = ((2 * Length / UnitSize) + 1) / 2;

  return true;
}

/*
  Represent frequency as hardware duration
*/
TBool me_HardwareClockScaling::CalculateHardwareDuration(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz,
  THardwareDurationOptions HwSpec
)
{
  /*
    If clock ticked 1000 times per second, for delay of 10 milli-seconds
    we need count to 10.

    But clock ticks faster and maximum counter value is limited.
    So this code.

    Also clock speed (BaseFreq) is another parameter.
    But for practical reasons it's used as constant.
    Or else we should mention it in hardware duration record.
  */

  TUint_4 CounterMaxValue;
  TUint_1 Index;
  TUint_4 ScaledFreq;
  TUint_4 CounterLimit;

  if (!CheckSpec(&HwSpec))
    return false;

  CounterMaxValue = (1L << HwSpec.CounterNumBits);

  for (Index = 0; Index < HwSpec.NumPrescalerValues; ++Index)
  {
    ScaledFreq = BaseFreq >> HwSpec.PrescalerPowsOfTwo[Index];

    if (!GetNumUnitsForLength(&CounterLimit, ScaledFreq, Freq_Hz))
      continue;

    if (CounterLimit == 0)
      continue;

    if (CounterLimit > CounterMaxValue)
      continue;

    HwDur->PrescalerPowOfTwo = HwSpec.PrescalerPowsOfTwo[Index];
    HwDur->CounterLimit = CounterLimit - 1;

    return true;
  }

  return false;
}

/*
  Represent hardware duration as frequency
*/
TBool me_HardwareClockScaling::CalculateFrequency(
  TUint_4 * Freq_Hz,
  THardwareDuration HwDur
)
{
  /*
    It's easier. Imagine we want to get our 16 MHz back.
    It means no prescaler and no scaling. Then add adjustments.
  */

  TUint_4 ScaledFreq;
  TUint_4 CounterLimit;

  ScaledFreq = BaseFreq >> HwDur.PrescalerPowOfTwo;
  CounterLimit = (TUint_4) HwDur.CounterLimit + 1;

  /*
    Freq_Hz = ScaledFreq / CounterLimit

      27 / 10 = 2
      20 / 7 = 2

      We don't like this undershooting. We'll use rounding again.
  */
  if (!GetNumUnitsForLength(Freq_Hz, ScaledFreq, CounterLimit))
    return false;

  return true;
}

/*
  2025-10-15
  2025-10-16
*/
