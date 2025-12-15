// Test from decommissioned [me_TimerTools] module

/*
  Author: Martin Eden
  Last mod.: 2025-12-15
*/

/*
  Note to myself - merge/rework. Also print prescales.
*/

#include <me_HardwareClockScaling.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>
#include <me_Duration.h>

void Test_CounterToDuration(
  TUint_2 Counter,
  TUint_1 Prescale_PowOfTwo
)
{
  me_HardwareClockScaling::THardwareDuration HwDur;
  me_Duration::TDuration Duration;

  me_DebugPrints::Print("Counter", Counter);

  HwDur.Prescale_PowOfTwo = Prescale_PowOfTwo;
  HwDur.Scale_BaseOne = Counter;

  Duration = me_HardwareClockScaling::HwToSwDuration(HwDur);

  Console.Write("Duration");
  me_DebugPrints::PrintDuration(Duration);
  Console.EndLine();

  Console.Print("");
}

void Test_DurationToCounter(
  me_Duration::TDuration Duration,
  TUint_1 Prescale_PowOfTwo
)
{
  me_HardwareClockScaling::THardwareDuration HwDur;

  Console.Write("Duration");
  me_DebugPrints::PrintDuration(Duration);
  Console.EndLine();

  if (!me_HardwareClockScaling::SwToHwDuration(&HwDur, Duration, Prescale_PowOfTwo))
    Console.Print("Conversion to counter is capped");

  me_DebugPrints::Print("Counter", HwDur.Scale_BaseOne);

  Console.Print("");
}

void Test_CounterToDurations()
{
  TUint_1 Prescale_PowOfTwo;

  Prescale_PowOfTwo = 6;
  Test_CounterToDuration(0, Prescale_PowOfTwo);
  Test_CounterToDuration(1, Prescale_PowOfTwo);
  Test_CounterToDuration(32767, Prescale_PowOfTwo);
  Test_CounterToDuration(32768, Prescale_PowOfTwo);
  Test_CounterToDuration(TUint_2_Max, Prescale_PowOfTwo);
}

void Test_DurationsToCounter()
{
  TUint_1 Prescale_PowOfTwo;

  Prescale_PowOfTwo = 6;
  Test_DurationToCounter({0, 0, 0, 0}, Prescale_PowOfTwo);
  Test_DurationToCounter({0, 0, 0, 1}, Prescale_PowOfTwo);
  Test_DurationToCounter({0, 0, 1, 0}, Prescale_PowOfTwo);
  Test_DurationToCounter({0, 1, 0, 0}, Prescale_PowOfTwo);
  Test_DurationToCounter({1, 0, 0, 0}, Prescale_PowOfTwo);
}

void setup()
{
  Console.Init();

  Console.Print("[me_HardwareClockScaling] test");
  Test_CounterToDurations();
  Test_DurationsToCounter();
  Console.Print("Done");
}

void loop()
{
}

/*
  2025-11-28
  2025-11-30
*/
