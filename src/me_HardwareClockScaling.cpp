// Convert frequency to hardware duration. Implementation

/*
  Author: Martin Eden
  Last mod.: 2025-12-26
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
  Calculate hardware duration from from frequency, prescale and
  result size limit.
*/
TBool me_HardwareClockScaling::CalculateHardwareDuration(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz,
  TUint_1 Prescale_PowOfTwo,
  TUint_1 ScaleSize_NumBits
)
{
  if (!Freetown::CheckSpec(Prescale_PowOfTwo, ScaleSize_NumBits))
    return false;

  return
    Freetown::CalculateHardwareDuration(
      HwDur, Freq_Hz, Prescale_PowOfTwo, ScaleSize_NumBits
    );
}

/*
  Calculate scaling from frequency and list of scale limits
*/
TBool me_HardwareClockScaling::CalculateHardwareDuration_Specs(
  THardwareDuration * Scale,
  TUint_4 Freq_Hz,
  THardwareDurationSpecs Specs
)
{
  if (!Freetown::CheckSpecs(Specs))
    return false;

  return Freetown::CalculateHardwareDuration_Specs(Scale, Freq_Hz, Specs);
}

/*
  Calculate frequency from scaling
*/
TBool me_HardwareClockScaling::CalculateFrequency(
  TUint_4 * Freq_Hz,
  THardwareDuration Scale
)
{
  if (!Freetown::CheckScaling(Scale))
    return false;

  return Freetown::CalculateFrequency(Freq_Hz, Scale);
}

/*
  Find suitable clock scale for desired tick range (in micros)
*/
TBool me_HardwareClockScaling::GetPrescaleForTickDuration_Specs(
  TUint_1 * Prescale_PowOfTwo,
  TUint_2 TickDuration_Us,
  THardwareDurationSpecs Specs
)
{
  if (!Freetown::CheckSpecs(Specs))
    return false;

  *Prescale_PowOfTwo =
    Freetown::GetPrescaleForTickDuration_Specs(TickDuration_Us, Specs);

  return true;
}

const TUint_1 TicksPerMicroS = F_CPU / 1000000;

/*
  [Import] Convert microseconds to hardware duration
*/
TBool me_HardwareClockScaling::HwDurationFromMicros(
  THardwareDuration * HwDur,
  TUint_4 NumMicros,
  TUint_1 Prescale_PowOfTwo
)
{
  TUint_4 NumTicks;

  HwDur->Prescale_PowOfTwo = Prescale_PowOfTwo;

  NumTicks = (NumMicros * TicksPerMicroS) >> Prescale_PowOfTwo;

  if (NumTicks == 0)
  {
    HwDur->Scale_BaseOne = 0;

    return false;
  }

  if (NumTicks > TUint_2_Max)
  {
    HwDur->Scale_BaseOne = TUint_2_Max;

    return false;
  }

  HwDur->Scale_BaseOne = NumTicks - 1;

  return true;
}

/*
  [Export] Convert hardware duration to number of microseconds
*/
TUint_4 me_HardwareClockScaling::MicrosFromHwDuration(
  THardwareDuration HwDur
)
{
  TUint_4 NumTicks;
  TUint_4 NumMicros;

  NumTicks = (TUint_4(HwDur.Scale_BaseOne) + 1) << HwDur.Prescale_PowOfTwo;
  NumMicros = NumTicks / TicksPerMicroS;

  return NumMicros;
}

/*
  2025-10-15
  2025-10-16
  2025-10-19
  2025-10-21
  2025-11-28
  2025-11-29 Calculation for tick, restyling, stand-alone Freetown
  2025-11-30
  2025-12-14 Review and styling
*/
