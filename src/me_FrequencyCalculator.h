// Convert frequency to hardware duration description and back

/*
  Author: Martin Eden
  Last mod.: 2025-10-18
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

namespace me_FrequencyCalculator
{
  /*
    Duration as clock slowdown and number of clock cycles

    For number of cycles: 0 means 1, 1 means 2 etc.
  */
  struct THardwareDuration
  {
    TUint_1 PrescalerPowOfTwo;
    TUint_2 CounterLimit;
  };

  const TUint_1 MaxPrescalerValues = 7;

  /*
    Available prescalers and counter size

    Prescalers array should store ascending values.
  */
  struct THardwareDurationOptions
  {
    TUint_1 NumPrescalerValues;
    TUint_1 PrescalerPowsOfTwo[MaxPrescalerValues];
    TUint_1 CounterNumBits;
  };

  // Convert frequency to hardware duration
  TBool CalculateHardwareDuration(
    THardwareDuration * HwDur,
    TUint_4 Freq_Hz,
    THardwareDurationOptions HwOpts
  );

  // Convert hardware duration to frequency
  TBool CalculateFrequency(
    TUint_4 * Freq_Hz,
    THardwareDuration HwDur
  );
}

/*
  2025-10-15
  2025-10-16
  2025-10-18
*/
