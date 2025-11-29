// Convert frequency to hardware duration. Implementation

/*
  Author: Martin Eden
  Last mod.: 2025-11-29
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
  TClockScaleSetting Setting
)
{
  if (!Freetown::CheckSpec(Setting))
    return false;

  return Freetown::CalculateClockScale_Spec(Scale, Freq_Hz, Setting);
}

/*
  Calculate scaling from frequency and list of scale limits
*/
TBool me_HardwareClockScaling::CalculateClockScale_Specs(
  TClockScale * Scale,
  TUint_4 Freq_Hz,
  TClockScalingOptions ScaleOpts
)
{
  if (!Freetown::CheckSpecs(ScaleOpts))
    return false;

  return Freetown::CalculateClockScale_Specs(Scale, Freq_Hz, ScaleOpts);
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
  2025-10-15
  2025-10-16
  2025-10-19
  2025-10-21
  2025-11-29 Calculation for tick, restyling, stand-alone Freetown
*/
