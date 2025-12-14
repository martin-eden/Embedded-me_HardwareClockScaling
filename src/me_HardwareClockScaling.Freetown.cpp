// Core scaling functions and checks

/*
  Author: Martin Eden
  Last mod.: 2025-12-14
*/

#include <me_HardwareClockScaling.h>

using namespace me_HardwareClockScaling;

const TUint_4 BaseFreq_Hz = F_CPU;

/*
  Constant 16

  16 is maximum number of bits in counter value for ATmega328.
  We can make it named const here but some other functions
  return TUint_2, implying maximum size of 16 bits.

  So we're not naming it. It does not improve readability and
  changing it will not change much really.
*/

/*
  Check scaling
*/
TBool Freetown::CheckScaling(
  THardwareDuration Scaling
)
{
  return (Scaling.Prescale_PowOfTwo <= 16);
}

/*
  Check hardware duration limits
*/
TBool Freetown::CheckSpec(
  TUint_1 Prescale_PowOfTwo,
  TUint_1 ScaleSize_NumBits
)
{
  return
    (Prescale_PowOfTwo <= 16) &&
    ((ScaleSize_NumBits > 0) && (ScaleSize_NumBits <= 16));
}

/*
  Check hardware spec
*/
TBool Freetown::CheckSpecs(
  TClockScalingOptions Specs
)
{
  TUint_1 Prescale_PowOfTwo;
  TUint_1 ScaleSize_NumBits;
  TUint_1 Index;

  if (Specs.NumPrescalerValues == 0)
    return false;

  ScaleSize_NumBits = Specs.ScaleSize_NumBits;

  for (Index = 0; Index < Specs.NumPrescalerValues; ++Index)
  {
    Prescale_PowOfTwo = Specs.Prescales_PowOfTwo[Index];

    if (!CheckSpec(Prescale_PowOfTwo, ScaleSize_NumBits))
      return false;

    // Check for ascending order of prescales
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
  Calculate hardware duration from frequency and prescale

  Also we need to know result variable size to be sure
  that our result will satisfy your requirements.
*/
TBool Freetown::CalculateHardwareDuration(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz,
  TUint_1 Prescale_PowOfTwo,
  TUint_1 ScaleSize_NumBits
)
{
  TUint_4 Prescale;
  TUint_4 ScaledFreq_Hz;
  TUint_4 Scale;
  TUint_4 MaxScale;

  Prescale = (1L << Prescale_PowOfTwo);

  if (!GetNumUnitsForLength(&ScaledFreq_Hz, BaseFreq_Hz, Prescale))
    return false;

  if (!GetNumUnitsForLength(&Scale, ScaledFreq_Hz, Freq_Hz))
    return false;

  if (Scale == 0)
    return false;

  MaxScale = (1L << ScaleSize_NumBits);

  if (Scale > MaxScale)
    return false;

  HwDur->Prescale_PowOfTwo = Prescale_PowOfTwo;
  HwDur->Scale_BaseOne = Scale - 1;

  return true;
}

/*
  Calculate scaling from frequency and list of scale limits
*/
TBool Freetown::CalculateClockScale_Specs(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz,
  TClockScalingOptions Specs
)
{
  TUint_1 Prescale_PowOfTwo;
  TUint_1 ScaleSize_NumBits;
  TUint_1 Index;

  ScaleSize_NumBits = Specs.ScaleSize_NumBits;

  for (Index = 0; Index < Specs.NumPrescalerValues; ++Index)
  {
    Prescale_PowOfTwo = Specs.Prescales_PowOfTwo[Index];

    if (
      Freetown::CalculateHardwareDuration(
        HwDur, Freq_Hz, Prescale_PowOfTwo, ScaleSize_NumBits
      )
    )
      return true;
  }

  return false;
}

/*
  Calculate frequency from hardware duration
*/
TBool Freetown::CalculateFrequency(
  TUint_4 * Freq_Hz,
  THardwareDuration HwDur
)
{
  TUint_4 Prescale;
  TUint_4 ScaledFreq_Hz;
  TUint_4 Scale;

  Prescale = (1L << HwDur.Prescale_PowOfTwo);

  if (!GetNumUnitsForLength(&ScaledFreq_Hz, BaseFreq_Hz, Prescale))
    return false;

  Scale = (TUint_4) HwDur.Scale_BaseOne + 1;

  if (!GetNumUnitsForLength(Freq_Hz, ScaledFreq_Hz, Scale))
    return false;

  return true;
}

// [Internal] Calculate tick duration (in micros) for given clock prescaling
static TUint_2 CalcTickDuration_us(
  TUint_1 Prescale_PowOfTwo
)
{
  return 1000000 / (BaseFreq_Hz >> Prescale_PowOfTwo);
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
TUint_1 Freetown::GetPrescaleFromTickDuration_Specs(
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
    return 0;

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

  return Specs.Prescales_PowOfTwo[BestIndex];
}

/*
  Return maximum value counter can hold by spec
*/
TUint_2 Freetown::GetMaxCounterValue(
  TUint_1 CounterWidthInBits
)
{
  return TUint_2_Max >> (16 - CounterWidthInBits);
}

/*
  2025-11-29
  2025-11-30
  2025-12-14
*/
