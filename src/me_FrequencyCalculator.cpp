// Convert frequency to hardware duration. Implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-15
*/

#include <me_FrequencyCalculator.h>

#include <me_BaseTypes.h>

#include <me_Console.h>

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

TBool me_FrequencyCalculator::CalculateHardwareDuration(
  THardwareDuration * HwDur [[gnu::unused]],
  TUint_4 Freq_Hz [[gnu::unused]],
  THardwareSpec HwSpec
)
{
  const TUint_4 BaseFreq = 20000000L; //F_CPU;
  TUint_4 ScaledFreq;

  if (!CheckSpec(&HwSpec))
    return false;

  Console.Write("BaseFreq");
  Console.Print(BaseFreq);
  Console.EndLine();

  TUint_4 CounterMaxValue;
  TUint_1 Index;

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

      Console.Write("ScaledFreq");
      Console.Print(ScaledFreq);
      Console.EndLine();

      Console.Write("CountTo");
      Console.Print(HwDur->CounterLimit + 1);
      Console.EndLine();

      return true;
    }
  }

  return false;
}

TBool me_FrequencyCalculator::CalculateHardwareDuration_Counter1(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz
)
{
  THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 5;
  HwSpec.PrescalerPowsOfTwo[0] = 0;
  HwSpec.PrescalerPowsOfTwo[1] = 3;
  HwSpec.PrescalerPowsOfTwo[2] = 6;
  HwSpec.PrescalerPowsOfTwo[3] = 8;
  HwSpec.PrescalerPowsOfTwo[4] = 10;

  HwSpec.CounterNumBits = 8;

  return CalculateHardwareDuration(HwDur, Freq_Hz, HwSpec);
}

TBool me_FrequencyCalculator::CalculateHardwareDuration_Counter2(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz
)
{
  THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 5;
  HwSpec.PrescalerPowsOfTwo[0] = 0;
  HwSpec.PrescalerPowsOfTwo[1] = 3;
  HwSpec.PrescalerPowsOfTwo[2] = 6;
  HwSpec.PrescalerPowsOfTwo[3] = 8;
  HwSpec.PrescalerPowsOfTwo[4] = 10;

  HwSpec.CounterNumBits = 16;

  return CalculateHardwareDuration(HwDur, Freq_Hz, HwSpec);
}

TBool me_FrequencyCalculator::CalculateHardwareDuration_Counter3(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz
)
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

  return CalculateHardwareDuration(HwDur, Freq_Hz, HwSpec);
}

TBool me_FrequencyCalculator::CalculateHardwareDuration_Uart(
  THardwareDuration * HwDur,
  TUint_4 Freq_Hz
)
{
  THardwareSpec HwSpec;

  HwSpec.NumPrescalerValues = 2;
  HwSpec.PrescalerPowsOfTwo[0] = 3;
  HwSpec.PrescalerPowsOfTwo[1] = 4;

  HwSpec.CounterNumBits = 12;

  return CalculateHardwareDuration(HwDur, Freq_Hz, HwSpec);
}

/*
  2025-10-15
*/
