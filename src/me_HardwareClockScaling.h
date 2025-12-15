// Convert frequency to hardware duration description and back

/*
  Author: Martin Eden
  Last mod.: 2025-12-14
*/

/*
  Scope

  Duration <--> Hardware Duration

  "Duration" is time record, based on one second and base 1000.
  "Hardware Duration" is.. well, hardware time record, based on
  number of clock cycles and scaling factor.

  Hardware Duration record used in interface of harder-level modules
  like UART and RunTime.
*/

/*
  This module is used differently

  [me_Uart] and [me_RunTime] use it to get hardware duration.
  [me_DigitalSignalRecorder] use it to get software duration.
*/

#pragma once

#include <me_BaseTypes.h>
#include <me_Duration.h>

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

  const TUint_1 MaxPrescalerValues = 7;

  /*
    List of clock slowdown limits: slowdown factors and counter size

    Slowdown factors should be in ascending order.
  */
  struct TClockScalingOptions
  {
    TUint_1 NumPrescalerValues;
    TUint_1 Prescales_PowOfTwo[MaxPrescalerValues];
    TUint_1 ScaleSize_NumBits;
  };

  // ( Interface functions

  // Calculate hardware duration from frequency and scale limits
  TBool CalculateHardwareDuration(
    THardwareDuration * ClockScale,
    TUint_4 Freq_Hz,
    TUint_1 Prescale_PowOfTwo,
    TUint_1 ScaleSize_NumBits
  );

  // Calculate scaling from frequency and list of scale limits
  TBool CalculateHardwareDuration_Specs(
    THardwareDuration * ClockScale,
    TUint_4 Freq_Hz,
    TClockScalingOptions ScalingOpts
  );

  // Calculate frequency from scaling
  TBool CalculateFrequency(
    TUint_4 * Freq_Hz,
    THardwareDuration ClockScale
  );

  // Find suitable clock scale for desired tick duration (in micros)
  TBool PrescaleFromTickDuration_Specs(
    TUint_1 * Prescale_Pow2,
    TUint_2 TickDuration_Us,
    TClockScalingOptions Specs
  );

  // Set counter value to max allowed by spec
  TBool SetMaxCounterValue(
    THardwareDuration * Scale,
    TUint_1 Prescale_PowOfTwo,
    TUint_1 ScaleSize_NumBits
  );

  // ( Imported from [me_TimerTools]
  me_Duration::TDuration CounterToDuration(
    TUint_2 Counter,
    TUint_1 Prescale_PowOfTwo
  );

  TBool DurationToCounter(
    TUint_2 * Counter,
    me_Duration::TDuration,
    TUint_1 Prescale_PowOfTwo
  );

  me_Duration::TDuration HwToSwDuration(
    me_HardwareClockScaling::THardwareDuration
  );
  // )

  // )

  // Real implementation and input checks
  namespace Freetown
  {
    TBool CheckScaling(THardwareDuration);
    TBool CheckSpec(TUint_1 Prescale_PowOfTwo, TUint_1 ScaleSize_NumBits);
    TBool CheckSpecs(TClockScalingOptions);

    TBool CalculateHardwareDuration(
      THardwareDuration * HwDur,
      TUint_4 Freq_Hz,
      TUint_1 Prescale_PowOfTwo,
      TUint_1 ScaleSize_NumBits
    );
    TBool CalculateHardwareDuration_Specs(
      THardwareDuration *,
      TUint_4 Freq_Hz,
      TClockScalingOptions Specs
    );
    TBool CalculateFrequency(TUint_4 *, THardwareDuration);
    TUint_1 GetPrescaleFromTickDuration_Specs(TUint_2, TClockScalingOptions);
    TUint_2 GetMaxCounterValue(TUint_1);
  }

  namespace AtMega328
  {
    TClockScalingOptions GetSpecs_Counter1();
    TClockScalingOptions GetSpecs_Counter2();
    TClockScalingOptions GetSpecs_Counter3();
    TClockScalingOptions GetSpecs_Uart();
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
*/
