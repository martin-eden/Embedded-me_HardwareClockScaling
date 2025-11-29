// Convert frequency to hardware duration description and back

/*
  Author: Martin Eden
  Last mod.: 2025-11-29
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_HardwareClockScaling
{
  /*
    Clock slowdown: slowdown factor and counter value

    For number of cycles: 0 means 1, 1 means 2 etc.
  */
  struct TClockScale
  {
    TUint_1 Prescale_PowOfTwo;
    TUint_2 CounterLimit;
  };

  /*
    Clock slowdown limits: slowdown factor and maximum counter value
  */
  struct TClockScaleSetting
  {
    TUint_1 Prescale_PowOfTwo;
    TUint_1 CounterNumBits;
  };

  const TUint_1 MaxPrescalerValues = 7;

  /*
    List of clock slowdown limits: slowdown factors and maximum counter
    value

    Slowdown factors should come in ascending order.
  */
  struct TClockScalingOptions
  {
    TUint_1 NumPrescalerValues;
    TUint_1 Prescales_PowOfTwo[MaxPrescalerValues];
    TUint_1 CounterNumBits;
  };

  // ( Interface functions

  // Calculate scaling from frequency and scale limits
  TBool CalculateClockScale_Spec(
    TClockScale * ClockScale,
    TUint_4 Freq_Hz,
    TClockScaleSetting ScaleSetting
  );

  // Calculate scaling from frequency and list of scale limits
  TBool CalculateClockScale_Specs(
    TClockScale * ClockScale,
    TUint_4 Freq_Hz,
    TClockScalingOptions ScalingOpts
  );

  // Calculate frequency from scaling
  TBool CalculateFrequency(
    TUint_4 * Freq_Hz,
    TClockScale ClockScale
  );

  // Find suitable clock scale for desired tick duration (in micros)
  TBool PrescaleFromTickDuration_Specs(
    TUint_1 *,
    TUint_2,
    TClockScalingOptions
  );

  // )

  // Real implementation and input checks
  namespace Freetown
  {
    TBool CheckScaling(TClockScale);
    TBool CheckSpec(TClockScaleSetting);
    TBool CheckSpecs(TClockScalingOptions);

    TBool CalculateClockScale_Spec(TClockScale *, TUint_4, TClockScaleSetting);
    TBool CalculateClockScale_Specs(TClockScale *, TUint_4, TClockScalingOptions);
    TBool CalculateFrequency(TUint_4 *, TClockScale);
    TUint_1 GetPrescaleFromTickDuration_Specs(TUint_2, TClockScalingOptions);
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
*/
