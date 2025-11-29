// Core scaling functions and checks

/*
  Author: Martin Eden
  Last mod.: 2025-11-29
*/

#include <me_HardwareClockScaling.h>

using namespace me_HardwareClockScaling;

const TUint_4 ClocksPerSecond = F_CPU;

/*
  Check scaling
*/
TBool Freetown::CheckScaling(
  TClockScale Scaling
)
{
  return (Scaling.Prescale_PowOfTwo <= 16);
}

/*
  Check hardware setting
*/
TBool Freetown::CheckSpec(
  TClockScaleSetting Spec
)
{
  return
    (Spec.CounterNumBits <= 16) &&
    (Spec.Prescale_PowOfTwo <= 16);
}

/*
  Check hardware spec
*/
TBool Freetown::CheckSpecs(
  TClockScalingOptions Specs
)
{
  TClockScaleSetting Spec;
  TUint_1 Index;

  if (Specs.NumPrescalerValues == 0)
    return false;

  Spec.CounterNumBits = Specs.CounterNumBits;

  for (Index = 0; Index < Specs.NumPrescalerValues; ++Index)
  {
    Spec.Prescale_PowOfTwo = Specs.Prescales_PowOfTwo[Index];

    if (!CheckSpec(Spec))
      return false;

    if (Index > 0)
      if (Specs.Prescales_PowOfTwo[Index] <= Specs.Prescales_PowOfTwo[Index - 1])
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
  Calculate scaling from frequency and scale limits
*/
TBool Freetown::CalculateClockScale_Spec(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScaleSetting Setting
)
{
  TUint_4 CounterMaxValue;
  TUint_4 ClockSlowdown;
  TUint_4 ScaledFreq;
  TUint_4 CounterLimit;

  CounterMaxValue = (1L << Setting.CounterNumBits);

  ClockSlowdown = (1L << Setting.Prescale_PowOfTwo);

  if (!GetNumUnitsForLength(&ScaledFreq, ClocksPerSecond, ClockSlowdown))
    return false;

  if (!GetNumUnitsForLength(&CounterLimit, ScaledFreq, Freq_Hz))
    return false;

  if (CounterLimit == 0)
    return false;

  if (CounterLimit > CounterMaxValue)
    return false;

  Scale->Prescale_PowOfTwo = Setting.Prescale_PowOfTwo;
  Scale->CounterLimit = CounterLimit - 1;

  return true;
}

/*
  Calculate scaling from frequency and list of scale limits
*/
TBool Freetown::CalculateClockScale_Specs(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScalingOptions Specs
)
{
  TClockScaleSetting Setting;
  TUint_1 Index;

  Setting.CounterNumBits = Specs.CounterNumBits;

  for (Index = 0; Index < Specs.NumPrescalerValues; ++Index)
  {
    Setting.Prescale_PowOfTwo = Specs.Prescales_PowOfTwo[Index];

    if (Freetown::CalculateClockScale_Spec(Scale, Freq_Hz, Setting))
      return true;
  }

  return false;
}

/*
  Calculate frequency from scaling
*/
TBool Freetown::CalculateFrequency(
  TUint_4 * Freq_Hz,
  TClockScale Scale
)
{
  TUint_4 ScaledFreq;
  TUint_4 ClockSlowdown;
  TUint_4 CounterLimit;

  ClockSlowdown = (1L << Scale.Prescale_PowOfTwo);

  if (!GetNumUnitsForLength(&ScaledFreq, ClocksPerSecond, ClockSlowdown))
    return false;

  CounterLimit = (TUint_4) Scale.CounterLimit + 1;

  if (!GetNumUnitsForLength(Freq_Hz, ScaledFreq, CounterLimit))
    return false;

  return true;
}

// [Internal] Calculate tick duration (in micros) for given clock prescaling
static TUint_2 CalcTickDuration_us(
  TUint_1 Prescale_PowOfTwo
)
{
  return 1000000 / (ClocksPerSecond >> Prescale_PowOfTwo);
}

// [Internal] Return distance between two unsigned integers
static TUint_2 CalcDelta(
  TUint_2 Current,
  TUint_2 Target
)
{
  if (Current < Target)
    return Target - Current;

  return Current - Target;
}

/*
  Find suitable clock scale for desired tick duration (in micros)
*/
void Freetown::PrescaleFromTickDuration_Specs(
  TUint_1 * ResultPrescale_Pow2,
  TUint_2 TargetTickDuration_us,
  TClockScalingOptions Specs
)
{
  /*
    Okay you're building your own timer. In this current case you
    want to get maximum period (to handle overflows rarely) while
    keeping reasonable precision.

    You provides us precision and counter specification:

      (3 (8 (0 3 5 6 7 8 10)))

    We're converting counter's prescales to tick duration and
    looking for best match for duration you asked:

      0 0 2 4 8 16 64 (us for 16 MHz)
          ^ ^
           Best matches for 3 us durations.

    We're taking last one of "best" matches. That's 4 in this case.

    We returning (6) - set prescale to 1/64 (2^6 = 64).
  */

  TUint_1 Index;
  TUint_1 Prescale_PowOfTwo;
  TUint_2 TickDuration_us;
  TUint_2 MinDelta;
  TUint_2 CurDelta;
  TUint_2 PrevDelta;
  TUint_1 BestIndex;

  if (Specs.NumPrescalerValues == 0)
    return;

  MinDelta = TUint_2_Max;
  PrevDelta = TUint_2_Max;
  BestIndex = 0;

  for (Index = 0; Index < Specs.NumPrescalerValues; ++Index)
  {
    Prescale_PowOfTwo = Specs.Prescales_PowOfTwo[Index];

    TickDuration_us = CalcTickDuration_us(Prescale_PowOfTwo);

    CurDelta = CalcDelta(TickDuration_us, TargetTickDuration_us);

    if (CurDelta > PrevDelta)
      break;

    if (CurDelta <= MinDelta)
    {
      BestIndex = Index;

      MinDelta = CurDelta;
    }

    PrevDelta = CurDelta;
  }

  *ResultPrescale_Pow2 = Specs.Prescales_PowOfTwo[BestIndex];
}

/*
  2025-11-29
*/
