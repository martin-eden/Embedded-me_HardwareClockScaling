// Convert frequency to hardware duration. Implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-19
*/

/*
  This implementation is doing closest match calculations

  Imagine we can do only 10 s delays and asked for 27 s delay.

  We can't do it exactly, so our options are:

    * Fail because we can't do it exactly
    * Do 20 s delay, we never overshoot
    * Do 30 s delay because it's closest match
*/

#include <me_HardwareClockScaling.h>

#include <me_BaseTypes.h>

using namespace me_HardwareClockScaling;

const TUint_4 BaseFreq = F_CPU;

// [Internal] Check hardware spec
static TBool CheckSpec(
  TClockScalingOptions * ScaleOpts
)
{
  TUint_1 Index;

  if (ScaleOpts->NumPrescalerValues == 0)
    return false;

  for (Index = 0; Index < ScaleOpts->NumPrescalerValues; ++Index)
  {
    if (ScaleOpts->CounterNumBits > 16)
      return false;

    if (ScaleOpts->Prescales_PowOfTwo[Index] > 16)
      return false;

    if (Index > 0)
      if (ScaleOpts->Prescales_PowOfTwo[Index] <= ScaleOpts->Prescales_PowOfTwo[Index - 1])
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
TBool me_HardwareClockScaling::CalculateClockScale(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScalingOptions ScaleOpts
)
{
  TUint_4 CounterMaxValue;
  TUint_1 Index;
  TUint_4 ScaledFreq;
  TUint_4 CounterLimit;

  if (!CheckSpec(&ScaleOpts))
    return false;

  CounterMaxValue = (1L << ScaleOpts.CounterNumBits);

  for (Index = 0; Index < ScaleOpts.NumPrescalerValues; ++Index)
  {
    ScaledFreq = BaseFreq >> ScaleOpts.Prescales_PowOfTwo[Index];

    if (!GetNumUnitsForLength(&CounterLimit, ScaledFreq, Freq_Hz))
      continue;

    if (CounterLimit == 0)
      continue;

    if (CounterLimit > CounterMaxValue)
      continue;

    Scale->Prescale_PowOfTwo = ScaleOpts.Prescales_PowOfTwo[Index];
    Scale->CounterLimit = CounterLimit - 1;

    return true;
  }

  return false;
}

// [Internal] Check scale
static TBool CheckScale(
  TClockScale Scale
)
{
  if (Scale.Prescale_PowOfTwo > 16)
    return false;

  return true;
}

/*
  Represent hardware duration as frequency
*/
TBool me_HardwareClockScaling::CalculateFrequency(
  TUint_4 * Freq_Hz,
  TClockScale Scale
)
{
  TUint_4 ScaledFreq;
  TUint_4 CounterLimit;

  if (!CheckScale(Scale))
    return false;

  ScaledFreq = BaseFreq >> Scale.Prescale_PowOfTwo;
  CounterLimit = (TUint_4) Scale.CounterLimit + 1;

  if (!GetNumUnitsForLength(Freq_Hz, ScaledFreq, CounterLimit))
    return false;

  return true;
}

/*
  2025-10-15
  2025-10-16
  2025-10-19
*/
