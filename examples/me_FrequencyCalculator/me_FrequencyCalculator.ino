// [me_FrequencyCalculator] test

/*
  Author: Martin Eden
  Last mod.: 2025-10-15
*/

#include <me_FrequencyCalculator.h>

#include <me_BaseTypes.h>
#include <me_Console.h>

#include <me_Uart.h>

typedef TBool (*TCalculator)(
  me_FrequencyCalculator::THardwareDuration *,
  TUint_4
);

void PrintHardwareDuration(
  me_FrequencyCalculator::THardwareDuration HwDur
)
{
  Console.Write("Hardware duration:");
  Console.Write("(");
  Console.Print(HwDur.PrescalerPowOfTwo);
  Console.Print(HwDur.CounterLimit);
  Console.Write(")");
  Console.EndLine();
}

void PrintFrequency(
  TUint_4 Freq_Hz
)
{
  Console.Write("Frequency (Hz):");
  Console.Print(Freq_Hz);
  Console.EndLine();
}

void TestFreq(
  TUint_4 Freq_Hz,
  TCalculator Calc
)
{
  me_FrequencyCalculator::THardwareDuration HwDur;

  Console.Print("(");
  Console.Indent();

  PrintFrequency(Freq_Hz);

  if (!Calc(&HwDur, Freq_Hz))
    Console.Print("Duration calculation failed");
  else
    PrintHardwareDuration(HwDur);

  Console.Unindent();
  Console.Print(")");
}

const TUint_1 NumTestFreqs = 9;
TUint_4 FreqsTestSet[NumTestFreqs] =
  {
    50,
    100,
    500,
    1000,
    5000,
    10000,
    50000,
    100000,
    500000
  };

void TestCalculator(
  TAsciiz CalcName,
  TCalculator Calc
)
{
  Console.Write("(");
  Console.Write(CalcName);
  Console.Write("test");
  Console.EndLine();

  Console.Indent();

  for (TUint_1 Index = 0; Index < NumTestFreqs; ++Index)
    TestFreq(FreqsTestSet[Index], Calc);

  Console.Unindent();
  Console.Print(")");
}

void TestCounter1()
{
  TCalculator Calc = me_FrequencyCalculator::CalculateHardwareDuration_Counter1;

  TestCalculator("Counter 1", Calc);
}

void TestCounter2()
{
  TCalculator Calc = me_FrequencyCalculator::CalculateHardwareDuration_Counter2;

  TestCalculator("Counter 2", Calc);
}

void TestCounter3()
{
  TCalculator Calc = me_FrequencyCalculator::CalculateHardwareDuration_Counter3;

  TestCalculator("Counter 3", Calc);
}

void TestUart()
{
  TCalculator Calc = me_FrequencyCalculator::CalculateHardwareDuration_Uart;

  TestCalculator("UART", Calc);
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

  me_Uart::Init(9600);

  Console.Print("[me_FrequencyCalculator] test");
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
*/
