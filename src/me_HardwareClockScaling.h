// Convert frequency to hardware duration description and back

/*
  Author: Martin Eden
  Last mod.: 2025-10-19
*/

/*
  In ATmega328 hardware counters and UART (and maybe other modules)
  need to set speed slower than base clock speed.

  They are using "prescaler". Which is just integer power of two.
  For prescaler 1024 hardware module will run at each 1024th tick.

  (Well, that's speculation and abstraction for explanation.)

  Prescaler defines temporal granularity.

  And to store number of time granules another integer is used.
  For UART it's 10-bit int, for counters it's 8- or 16-bit.
*/

/*
  We do not like duplicated logic and we want back conversion:
  from duration to frequency. So we wrote this module.
*/

/*
  C language is wrong tool for this but we have no options now.
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_HardwareClockScaling
{
  /*
    Clock slowdown and number of clock cycles

    For number of cycles: 0 means 1, 1 means 2 etc.
  */
  struct TClockScale
  {
    TUint_1 Prescale_PowOfTwo;
    TUint_2 CounterLimit;
  };

  const TUint_1 MaxPrescalerValues = 7;

  /*
    Available prescalers and counter size

    Prescalers array should store ascending values.
  */
  struct TClockScalingOptions
  {
    TUint_1 NumPrescalerValues;
    TUint_1 Prescales_PowOfTwo[MaxPrescalerValues];
    TUint_1 CounterNumBits;
  };

  // Convert frequency to hardware duration
  TBool CalculateClockScale(
    TClockScale * ClockScale,
    TUint_4 Freq_Hz,
    TClockScalingOptions HwOpts
  );

  // Convert hardware duration to frequency
  TBool CalculateFrequency(
    TUint_4 * Freq_Hz,
    TClockScale ClockScale
  );
}

/*
  2025-10-15
  2025-10-16
  2025-10-18
*/
