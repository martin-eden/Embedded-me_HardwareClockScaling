// Convert frequency to hardware duration. Implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-21
*/

/*
  This implementation is doing closest match calculations

  Imagine we can do only 10 s delays and asked for 27 s delay.
  We'll do 30 s delay because it's closest match.
*/

#include <me_HardwareClockScaling.h>

#include <me_BaseTypes.h>

using namespace me_HardwareClockScaling;

const TUint_4 BaseFreq = F_CPU;

// [Internal] Check scale
static TBool CheckScale(
  TClockScale Scale
)
{
  return (Scale.Prescale_PowOfTwo <= 16);
}

// [Internal] Check hardware setting
static TBool CheckSetting(
  TClockScaleSetting Setting
)
{
  return
    (Setting.CounterNumBits <= 16) &&
    (Setting.Prescale_PowOfTwo <= 16);
}

// [Internal] Check hardware spec
static TBool CheckSpec(
  TClockScalingOptions ScaleOpts
)
{
  TClockScaleSetting Setting;
  TUint_1 Index;

  if (ScaleOpts.NumPrescalerValues == 0)
    return false;

  Setting.CounterNumBits = ScaleOpts.CounterNumBits;

  for (Index = 0; Index < ScaleOpts.NumPrescalerValues; ++Index)
  {
    Setting.Prescale_PowOfTwo = ScaleOpts.Prescales_PowOfTwo[Index];

    if (!CheckSetting(Setting))
      return false;

    if (Index > 0)
      if (ScaleOpts.Prescales_PowOfTwo[Index] <= ScaleOpts.Prescales_PowOfTwo[Index - 1])
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
  [Interface] Represent frequency as hardware duration
*/
TBool me_HardwareClockScaling::CalculateClockScale_Spec(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScaleSetting Setting
)
{
  if (!CheckSetting(Setting))
    return false;

  return Freetown::CalculateClockScale_Spec(Scale, Freq_Hz, Setting);
}

/*
  [Interface] Represent frequency as hardware duration using list of options
*/
TBool me_HardwareClockScaling::CalculateClockScale_Specs(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScalingOptions ScaleOpts
)
{
  if (!CheckSpec(ScaleOpts))
    return false;

  return Freetown::CalculateClockScale_Specs(Scale, Freq_Hz, ScaleOpts);
}

/*
  [Interface] Represent hardware duration as frequency
*/
TBool me_HardwareClockScaling::CalculateFrequency(
  TUint_4 * Freq_Hz,
  TClockScale Scale
)
{
  if (!CheckScale(Scale))
    return false;

  return Freetown::CalculateFrequency(Freq_Hz, Scale);
}

/*
  Represent frequency as hardware duration
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

  if (!GetNumUnitsForLength(&ScaledFreq, BaseFreq, ClockSlowdown))
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
  Represent frequency as hardware duration using list of options
*/
TBool Freetown::CalculateClockScale_Specs(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScalingOptions ScaleOpts
)
{
  TClockScaleSetting Setting;
  TUint_1 Index;

  Setting.CounterNumBits = ScaleOpts.CounterNumBits;

  for (Index = 0; Index < ScaleOpts.NumPrescalerValues; ++Index)
  {
    Setting.Prescale_PowOfTwo = ScaleOpts.Prescales_PowOfTwo[Index];

    if (Freetown::CalculateClockScale_Spec(Scale, Freq_Hz, Setting))
      return true;
  }

  return false;
}

/*
  Represent hardware duration as frequency
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

  if (!GetNumUnitsForLength(&ScaledFreq, BaseFreq, ClockSlowdown))
    return false;

  CounterLimit = (TUint_4) Scale.CounterLimit + 1;

  if (!GetNumUnitsForLength(Freq_Hz, ScaledFreq, CounterLimit))
    return false;

  return true;
}

/*
  2025-10-15
  2025-10-16
  2025-10-19
  2025-10-21
*/
