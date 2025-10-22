// Convert frequency to hardware duration description and back

/*
  Author: Martin Eden
  Last mod.: 2025-10-22
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

  // Convert frequency to clock scale, given limit
  TBool CalculateClockScale_Spec(
    TClockScale * ClockScale,
    TUint_4 Freq_Hz,
    TClockScaleSetting ScaleSetting
  );

  // Convert frequency to clock scale, given limits
  TBool CalculateClockScale_Specs(
    TClockScale * ClockScale,
    TUint_4 Freq_Hz,
    TClockScalingOptions ScalingOpts
  );

  // Convert clock scale to frequency
  TBool CalculateFrequency(
    TUint_4 * Freq_Hz,
    TClockScale ClockScale
  );

  // )

  // Real implementation without input checks
  namespace Freetown
  {
    TBool CalculateClockScale_Spec(TClockScale *, TUint_4, TClockScaleSetting);
    TBool CalculateClockScale_Specs(TClockScale *, TUint_4, TClockScalingOptions);
    TBool CalculateFrequency(TUint_4 *, TClockScale);
  }

  namespace AtMega328
  {
    TClockScalingOptions GetSpec_Counter1();
    TClockScalingOptions GetSpec_Counter2();
    TClockScalingOptions GetSpec_Counter3();
    TClockScalingOptions GetSpec_Uart();
  }
}

/*
  2025-10-15
  2025-10-16
  2025-10-18
  2025-10-21
*/
