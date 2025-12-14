// Convert frequency to hardware duration. Implementation

/*
  Author: Martin Eden
  Last mod.: 2025-11-30
*/

/*
  This implementation is doing closest match calculations

  Imagine we can do only 10 s delays and asked for 27 s delay.
  We'll do 30 s delay because it's closest match.
*/

#include <me_HardwareClockScaling.h>

#include <me_BaseTypes.h>

using namespace me_HardwareClockScaling;

/*
  Calculate scaling from frequency and scale limits
*/
TBool me_HardwareClockScaling::CalculateClockScale_Spec(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScaleSetting Spec
)
{
  if (!Freetown::CheckSpec(Spec))
    return false;

  return Freetown::CalculateClockScale_Spec(Scale, Freq_Hz, Spec);
}

/*
  Calculate scaling from frequency and list of scale limits
*/
TBool me_HardwareClockScaling::CalculateClockScale_Specs(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScalingOptions Specs
)
{
  if (!Freetown::CheckSpecs(Specs))
    return false;

  return Freetown::CalculateClockScale_Specs(Scale, Freq_Hz, Specs);
}

/*
  Calculate frequency from scaling
*/
TBool me_HardwareClockScaling::CalculateFrequency(
  TUint_4 * Freq_Hz,
  TClockScale Scale
)
{
  if (!Freetown::CheckScaling(Scale))
    return false;

  return Freetown::CalculateFrequency(Freq_Hz, Scale);
}

/*
  Find suitable clock scale for desired tick range (in micros)
*/
TBool me_HardwareClockScaling::PrescaleFromTickDuration_Specs(
  TUint_1 * Prescale_Pow2,
  TUint_2 TickDuration_Us,
  TClockScalingOptions Specs
)
{
  if (!Freetown::CheckSpecs(Specs))
    return false;

  *Prescale_Pow2 =
    Freetown::GetPrescaleFromTickDuration_Specs(TickDuration_Us, Specs);

  return true;
}

/*
  Set counter value to max allowed by spec
*/
TBool me_HardwareClockScaling::SetMaxCounterValue(
  TClockScale * Scale,
  TClockScaleSetting Spec
)
{
  if (!Freetown::CheckSpec(Spec))
    return false;

  Scale->CounterLimit = Freetown::GetMaxCounterValue(Spec.CounterNumBits);
  Scale->Prescale_PowOfTwo = Spec.Prescale_PowOfTwo;

  return true;
}

// ( Imported from [me_TimerTools]

const TUint_1 TicksPerMicroS = F_CPU / 1000000;

me_Duration::TDuration me_HardwareClockScaling::CounterToDuration(
  TUint_2 Counter,
  TUint_1 Prescale_PowOfTwo
)
{
  TUint_4 NumTicks;
  TUint_4 NumMicros;
  me_Duration::TDuration Result;

  NumTicks = (TUint_4(Counter) + 1) << Prescale_PowOfTwo;
  NumMicros = NumTicks / TicksPerMicroS;

  me_Duration::MicrosToDuration(&Result, NumMicros);

  return Result;
}

TBool me_HardwareClockScaling::DurationToCounter(
  TUint_2 * Counter,
  me_Duration::TDuration Duration,
  TUint_1 Prescale_PowOfTwo
)
{
  TUint_4 NumMicros;
  TUint_4 NumTicks;

  me_Duration::DurationToMicros(&NumMicros, Duration);
  NumTicks = (NumMicros * TicksPerMicroS) >> Prescale_PowOfTwo;

  if (NumTicks == 0)
  {
    *Counter = 0;

    return false;
  }

  if (NumTicks > TUint_2_Max)
  {
    *Counter = TUint_2_Max;

    return false;
  }

  *Counter = NumTicks - 1;

  return true;
}

/*
  Convert duration from hardware to software format

  Hardware duration format is slowdown and counter value.
  Software format is seconds record scaled by 10^3.
*/
me_Duration::TDuration me_HardwareClockScaling::HwToSwDuration(
  me_HardwareClockScaling::THardwareDuration HwDur
)
{
  return CounterToDuration(HwDur.CounterLimit, HwDur.Prescale_PowOfTwo);
}

// )

/*
  2025-10-15
  2025-10-16
  2025-10-19
  2025-10-21
  2025-11-28
  2025-11-29 Calculation for tick, restyling, stand-alone Freetown
  2025-11-30
*/
