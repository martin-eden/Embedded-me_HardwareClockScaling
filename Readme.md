## What

(2025-10)

Data structure for hardware duration with conversion methods.


## Details

Counters and USART at ATmega328 are independent hardware units running
from slowed down main clock. It's called "clock scaling".

Clock scaling is done in two stages.

First, we divide main clock by some power of two.
That's called "prescaling".

Second, we count cycles of prescaled clock and do hardware magic
every N'th cycle. That's called "scaling".

Humans like to work with seconds and hertz, not with number of ticks.

This module provides functions to convert hertz to ticks and back.


## Code

* [Interface][Interface]
* [Implementation][Implementation]
* [Example][Example]


## Requirements

  * arduino-cli
  * bash


## Install/remove

Easy way is to clone [GetLibs][GetLibs] repo and run it's code.


## See also

* [My other embedded C++ libraries][Embedded]
* [My other repositories][Repos]


[Interface]: src/me_FrequencyCalculator.h
[Implementation]: src/me_FrequencyCalculator.cpp
[Example]: examples/me_FrequencyCalculator/me_FrequencyCalculator.ino

[GetLibs]: https://github.com/martin-eden/Embedded-Framework-GetLibs

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents
