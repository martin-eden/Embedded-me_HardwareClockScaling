// Convert frequency to hardware duration. Implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-16
*/

#include <me_FrequencyCalculator.h>

#include <me_BaseTypes.h>

using namespace me_FrequencyCalculator;

// [Internal] Check hardware spec
TBool CheckSpec(
  THardwareSpec * Spec
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
  Represent frequency as hardware duration
*/
TBool me_FrequencyCalculator::CalculateHardwareDuration(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz,
  THardwareSpec HwSpec
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

  const TUint_4 BaseFreq = F_CPU;

  TUint_4 CounterMaxValue;
  TUint_1 Index;
  TUint_4 ScaledFreq;

  if (!CheckSpec(&HwSpec))
    return false;

  CounterMaxValue = (1L << HwSpec.CounterNumBits);

  for (Index = 0; Index < HwSpec.NumPrescalerValues; ++Index)
  {
    ScaledFreq = BaseFreq >> HwSpec.PrescalerPowsOfTwo[Index];

    if (
      (ScaledFreq > Freq_Hz) &&
      (ScaledFreq / Freq_Hz < CounterMaxValue)
    )
    {
      HwDur->PrescalerPowOfTwo = HwSpec.PrescalerPowsOfTwo[Index];
      HwDur->CounterLimit = ScaledFreq / Freq_Hz - 1;

      return true;
    }
  }

  return false;
}

THardwareSpec me_FrequencyCalculator::GetSpec_Counter1()
{
  THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 5;
  HwSpec.PrescalerPowsOfTwo[0] = 0;
  HwSpec.PrescalerPowsOfTwo[1] = 3;
  HwSpec.PrescalerPowsOfTwo[2] = 6;
  HwSpec.PrescalerPowsOfTwo[3] = 8;
  HwSpec.PrescalerPowsOfTwo[4] = 10;

  HwSpec.CounterNumBits = 8;

  return HwSpec;
}

THardwareSpec me_FrequencyCalculator::GetSpec_Counter2()
{
  THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 5;
  HwSpec.PrescalerPowsOfTwo[0] = 0;
  HwSpec.PrescalerPowsOfTwo[1] = 3;
  HwSpec.PrescalerPowsOfTwo[2] = 6;
  HwSpec.PrescalerPowsOfTwo[3] = 8;
  HwSpec.PrescalerPowsOfTwo[4] = 10;

  HwSpec.CounterNumBits = 16;

  return HwSpec;
}

THardwareSpec me_FrequencyCalculator::GetSpec_Counter3()
{
  THardwareSpec HwSpec;

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

THardwareSpec me_FrequencyCalculator::GetSpec_Uart()
{
  THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 2;
  HwSpec.PrescalerPowsOfTwo[0] = 3;
  HwSpec.PrescalerPowsOfTwo[1] = 4;

  HwSpec.CounterNumBits = 12;

  return HwSpec;
}

/*
  2025-10-15
  2025-10-16
*/
