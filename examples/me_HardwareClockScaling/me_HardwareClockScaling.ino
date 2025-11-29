// [me_HardwareClockScaling] test

/*
  Author: Martin Eden
  Last mod.: 2025-11-29
*/

#include <me_HardwareClockScaling.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>

void PrintScaling(
  me_HardwareClockScaling::TClockScale Scale
)
{
  Console.Write("Clock scale:");
  Console.Write("(");
  Console.Print(Scale.Prescale_PowOfTwo);
  Console.Print(Scale.CounterLimit);
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

// ( Testing frequencies calculation

void TestFreq(
  TUint_4 Freq_Hz,
  me_HardwareClockScaling::TClockScalingOptions Specs
)
{
  me_HardwareClockScaling::TClockScale HwDur;
  TUint_4 RealFreq_Hz;

  PrintFrequency("Wished frequency (Hz):", Freq_Hz);

  if (!me_HardwareClockScaling::CalculateClockScale_Specs(&HwDur, Freq_Hz, Specs))
  {
    Console.Print("Duration calculation failed");

    return;
  }
  PrintScaling(HwDur);

  if (!me_HardwareClockScaling::CalculateFrequency(&RealFreq_Hz, HwDur))
  {
    Console.Print("Frequency calculation failed");

    return;
  }
  PrintFrequency("Real frequency (Hz):", RealFreq_Hz);
}

//*
// Spectrum base
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
//*/

/*
// Demo of non-exact frequencies
const TUint_1 NumTestFreqs = 4;
TUint_4 FreqsTestSet[NumTestFreqs] =
  {
    9600,
    31337,
    57600,
    115200,
  };
//*/

void TestFreqCalculator(
  TAsciiz CalcName,
  me_HardwareClockScaling::TClockScalingOptions Specs
)
{
  Console.Write("(");
  Console.Write(CalcName);
  Console.Write("frequency calculation test");
  Console.EndLine();

  Console.Indent();

  Console.Print("");

  for (TUint_1 Index = 0; Index < NumTestFreqs; ++Index)
  {
    TestFreq(FreqsTestSet[Index], Specs);
    Console.Print("");
  }

  Console.Unindent();
  Console.Print(")");
}

void RunFreqTest()
{
  TestFreqCalculator("Counter 1", me_HardwareClockScaling::AtMega328::GetSpecs_Counter1());
  TestFreqCalculator("Counter 2", me_HardwareClockScaling::AtMega328::GetSpecs_Counter2());
  TestFreqCalculator("Counter 3", me_HardwareClockScaling::AtMega328::GetSpecs_Counter3());
  TestFreqCalculator("UART", me_HardwareClockScaling::AtMega328::GetSpecs_Uart());
}

// )

// ( Testing ticks durations

void TestTick(
  TUint_2 TickDuration_Us,
  me_HardwareClockScaling::TClockScalingOptions Specs
)
{
  TUint_1 Prescale_PowOfTwo;

  me_DebugPrints::Print("Target tick duration (us)", TickDuration_Us);
  Console.EndLine();

  if (!me_HardwareClockScaling::PrescaleFromTickDuration_Specs(&Prescale_PowOfTwo, TickDuration_Us, Specs))
  {
    Console.Print("Calculating scale for tick duration failed");

    return;
  }

  me_DebugPrints::Print("Prescale (power of 2):", Prescale_PowOfTwo);
}

const TUint_1 NumTestTickDurations = 7;
TUint_2 TickDurationsTestSet[NumTestTickDurations] =
  {
    0,
    1,
    3,
    8,
    21,
    55,
    89
  };

void TestTickCalculator(
  TAsciiz CalcName,
  me_HardwareClockScaling::TClockScalingOptions Specs
)
{
  Console.Write("(");
  Console.Write(CalcName);
  Console.Write("tick calculation test");
  Console.EndLine();

  Console.Indent();

  Console.Print("");

  for (TUint_1 Index = 0; Index < NumTestTickDurations; ++Index)
  {
    TestTick(TickDurationsTestSet[Index], Specs);
    Console.Print("");
  }

  Console.Unindent();
  Console.Print(")");
}

void RunTickTest()
{
  // TestTickCalculator("Counter 1", me_HardwareClockScaling::AtMega328::GetSpecs_Counter1());
  TestTickCalculator("Counter 2", me_HardwareClockScaling::AtMega328::GetSpecs_Counter2());
  TestTickCalculator("Counter 3", me_HardwareClockScaling::AtMega328::GetSpecs_Counter3());
  // TestTickCalculator("UART", me_HardwareClockScaling::AtMega328::GetSpecs_Uart());
}

// )

void setup()
{
  Console.Init();

  Console.Print("[me_HardwareClockScaling] test");
  Console.Indent();
  RunFreqTest();
  RunTickTest();
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
  2025-11-29
*/
