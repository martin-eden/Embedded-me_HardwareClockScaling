// Convert frequency to hardware duration description and back

/*
  Author: Martin Eden
  Last mod.: 2025-12-27
*/

/*
  Scope

  Frequency (Hz) <--> Hardware Duration

  Hardware Duration record used for interface to lower-level modules
  like UART and Counters. It stores number of system clock cycles.
  It is stored like number of 2^Prescale granules.

  We have generic time record Duration. Which is based on one second.
  It is out of focus of this module but we provide I/O interface for
  it when required.
*/

/*
  This module is used differently

  [me_Uart] and [me_RunTime] use it to get hardware duration.
  [me_DigitalSignalRecorder] use it to get software duration.
*/

/*
  Design and composition

  Focus is on CalculateHardwareDuration() function. It's simple
  formula but it accepts hardware-friendly arguments. That's her
  main point.

  Hardware facilities like counters, UART and watchdogs use counters
  that can be described as Hardware Duration. They often provide choice
  from several prescalers and differ in size of number that stores
  granules. We provide specific datatype for that options.
  Their options are described in [ScalingSpecs].

  And for that options we have feeder CalculateHardwareDuration_Specs().

  So caller may not know exact options for counter 3 on this
  microcontroller but still able to provide them from
  AtMega328::GetSpecs_Counter3().
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_HardwareClockScaling
{
  /*
    System clock slowdown: prescale and scale

    You can think of this as floating-point number:

      NumTicks = 2^Prescale * Scale

    <Scale> is 1-based number stored from zero (0 means 1).
  */
  struct THardwareDuration
  {
    TUint_1 Prescale_PowOfTwo;
    TUint_2 Scale_BaseOne;
  };

  /*
    List of clock slowdown limits: slowdown factors and counter size

    Slowdown factors should be in ascending order.
  */
  const TUint_1 MaxPrescalerValues = 7;
  struct THardwareDurationSpecs
  {
    TUint_1 NumPrescalerValues;
    TUint_1 Prescales_PowOfTwo[MaxPrescalerValues];
    TUint_1 ScaleSize_NumBits;
  };

  // Calculate hardware duration from frequency and scale limit
  TBool CalculateHardwareDuration(
    THardwareDuration * HwDur,
    TUint_4 Freq_Hz,
    TUint_1 Prescale_PowOfTwo,
    TUint_1 ScaleSize_NumBits
  );

  // Calculate hardware duration from frequency and scale limits
  TBool CalculateHardwareDuration_Specs(
    THardwareDuration * HwDur,
    TUint_4 Freq_Hz,
    THardwareDurationSpecs Specs
  );

  // Calculate frequency from hardware duration
  TBool CalculateFrequency(
    TUint_4 * Freq_Hz,
    THardwareDuration HwDur
  );

  // Find suitable prescale for desired tick duration (in micros)
  TBool GetPrescaleForTickDuration_Specs(
    TUint_1 * Prescale_PowOfTwo,
    TUint_2 TickDuration_Us,
    THardwareDurationSpecs Specs
  );

  // [Import] Microseconds to hardware duration
  TBool HwDurationFromMicros(
    THardwareDuration * HwDur,
    TUint_4,
    TUint_1 Prescale_PowOfTwo
  );

  // [Export] Hardware duration to microseconds
  TUint_4 MicrosFromHwDuration(
    THardwareDuration
  );

  // Real implementation and input checks
  namespace Freetown
  {
    TBool CheckScaling(THardwareDuration);
    TBool CheckSpec(TUint_1 Prescale_PowOfTwo, TUint_1 ScaleSize_NumBits);
    TBool CheckSpecs(THardwareDurationSpecs);

    TBool CalculateHardwareDuration(
      THardwareDuration * HwDur,
      TUint_4 Freq_Hz,
      TUint_1 Prescale_PowOfTwo,
      TUint_1 ScaleSize_NumBits
    );
    TBool CalculateHardwareDuration_Specs(
      THardwareDuration *,
      TUint_4 Freq_Hz,
      THardwareDurationSpecs Specs
    );
    TBool CalculateFrequency(TUint_4 *, THardwareDuration);
    TUint_1 GetPrescaleForTickDuration_Specs(TUint_2, THardwareDurationSpecs);
    TBool SetMaxCounterValue(
      THardwareDuration * HwDur,
      TUint_1 Prescale_PowOfTwo,
      TUint_1 ScaleSize_NumBits
    );
  }

  namespace AtMega328
  {
    THardwareDurationSpecs GetSpecs_Counter1();
    THardwareDurationSpecs GetSpecs_Counter2();
    THardwareDurationSpecs GetSpecs_Counter3();
    THardwareDurationSpecs GetSpecs_Uart();
  }
}

/*
  2025-10-15
  2025-10-16
  2025-10-18
  2025-10-21
  2025-11-28
  2025-11-30
  2025-12-08
  2025-12-14
  2025-12-27
*/
