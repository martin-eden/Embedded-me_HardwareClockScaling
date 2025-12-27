// Test from decommissioned [me_TimerTools] module

/*
  Author: Martin Eden
  Last mod.: 2025-12-27
*/

/*
  Note to myself - merge/rework. Also print prescales.
*/

#include <me_HardwareClockScaling.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>

void Test_CounterToDuration(
  TUint_2 Counter,
  TUint_1 Prescale_PowOfTwo
)
{
  me_HardwareClockScaling::THardwareDuration HwDur;
  TUint_4 Duration_Us;

  me_DebugPrints::Print("Counter", Counter);

  HwDur.Prescale_PowOfTwo = Prescale_PowOfTwo;
  HwDur.Scale_BaseOne = Counter;

  Duration_Us = me_HardwareClockScaling::MicrosFromHwDuration(HwDur);

  me_DebugPrints::Print("Duration (us)", Duration_Us);
  Console.EndLine();
}

void Test_DurationToCounter(
  TUint_4 Duration_Us,
  TUint_1 Prescale_PowOfTwo
)
{
  me_HardwareClockScaling::THardwareDuration HwDur;

  me_DebugPrints::Print("Duration (us)", Duration_Us);

  if (!me_HardwareClockScaling::HwDurationFromMicros(&HwDur, Duration_Us, Prescale_PowOfTwo))
    Console.Write("(Conversion to counter is capped)");

  me_DebugPrints::Print("Counter", HwDur.Scale_BaseOne);

  Console.EndLine();
}

void Test_CounterToDurations()
{
  TUint_1 Prescale_PowOfTwo;

  Prescale_PowOfTwo = 6;
  Test_CounterToDuration(0, Prescale_PowOfTwo);
  Test_CounterToDuration(1, Prescale_PowOfTwo);
  Test_CounterToDuration(249, Prescale_PowOfTwo);
  Test_CounterToDuration(32767, Prescale_PowOfTwo);
  Test_CounterToDuration(32768, Prescale_PowOfTwo);
  Test_CounterToDuration(TUint_2_Max, Prescale_PowOfTwo);
}

void Test_DurationsToCounter()
{
  TUint_1 Prescale_PowOfTwo;

  Prescale_PowOfTwo = 6;
  Test_DurationToCounter(0, Prescale_PowOfTwo);
  Test_DurationToCounter(1, Prescale_PowOfTwo);
  Test_DurationToCounter(1000, Prescale_PowOfTwo);
  Test_DurationToCounter(1000000, Prescale_PowOfTwo);
  Test_DurationToCounter(1000000000, Prescale_PowOfTwo);
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
  2025-12-27
*/
