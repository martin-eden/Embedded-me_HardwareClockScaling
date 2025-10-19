// [me_HardwareClockScaling] test

/*
  Author: Martin Eden
  Last mod.: 2025-10-19
*/

#include <me_HardwareClockScaling.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>
#include "me_ScalingSpecs.hpp" // test data

void PrintHardwareDuration(
  me_HardwareClockScaling::TClockScale HwDur
)
{
  Console.Write("Hardware duration:");
  Console.Write("(");
  Console.Print(HwDur.Prescale_PowOfTwo);
  Console.Print(HwDur.CounterLimit);
  Console.Write(")");
  Console.EndLine();
}

void PrintFrequency(
  TAsciiz Annotation,
  TUint_4 Freq_Hz
)
{
  me_DebugPrints::Print(Annotation, Freq_Hz);
  Console.EndLine();
}

void TestFreq(
  TUint_4 Freq_Hz,
  me_HardwareClockScaling::TClockScalingOptions HwSpec
)
{
  me_HardwareClockScaling::TClockScale HwDur;
  TUint_4 RealFreq_Hz;

  PrintFrequency("Wished frequency (Hz):", Freq_Hz);

  if (!me_HardwareClockScaling::CalculateHardwareDuration(&HwDur, Freq_Hz, HwSpec))
  {
    Console.Print("Duration calculation failed");

    return;
  }
  PrintHardwareDuration(HwDur);

  if (!me_HardwareClockScaling::CalculateFrequency(&RealFreq_Hz, HwDur))
  {
    Console.Print("Frequency calculation failed");

    return;
  }
  PrintFrequency("Real frequency (Hz):", RealFreq_Hz);
}

/*
// Testing spectrum base
const TUint_1 NumTestFreqs = 8;
TUint_4 FreqsTestSet[NumTestFreqs] =
  {
    100,
    500,
    1000,
    5000,
    10000,
    50000,
    100000,
    500000,
  };
*/

// Demo of non-exact frequencies
const TUint_1 NumTestFreqs = 4;
TUint_4 FreqsTestSet[NumTestFreqs] =
  {
    9600,
    31337,
    57600,
    115200,
  };

void TestCalculator(
  TAsciiz CalcName,
  me_HardwareClockScaling::TClockScalingOptions HwSpec
)
{
  Console.Write("(");
  Console.Write(CalcName);
  Console.Write("test");
  Console.EndLine();

  Console.Indent();

  Console.Print("");

  for (TUint_1 Index = 0; Index < NumTestFreqs; ++Index)
  {
    TestFreq(FreqsTestSet[Index], HwSpec);
    Console.Print("");
  }

  Console.Unindent();
  Console.Print(")");
}

void TestCounter1()
{
  TestCalculator("Counter 1", me_ScalingSpecs::GetSpec_Counter1());
}

void TestCounter2()
{
  TestCalculator("Counter 2", me_ScalingSpecs::GetSpec_Counter2());
}

void TestCounter3()
{
  TestCalculator("Counter 3", me_ScalingSpecs::GetSpec_Counter3());
}

void TestUart()
{
  TestCalculator("UART", me_ScalingSpecs::GetSpec_Uart());
}

void RunTest()
{
  TestCounter1();
  TestCounter2();
  TestCounter3();
  TestUart();
}

void setup()
{
  Console.Init();

  Console.Print("[me_HardwareClockScaling] test");
  Console.Indent();
  RunTest();
  Console.Unindent();
  Console.Print("Done");
}

void loop()
{
}

/*
  2025-10-15
  2025-10-16
  2025-10-18
*/
