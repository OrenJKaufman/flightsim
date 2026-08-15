Uses my OKEncoder library to store knob state and updates it as pin1 and pin2 change. Probably no reason to use this instead of [Ben Buxton's code](https://github.com/buxtronix/arduino/blob/master/libraries/Rotary/README.md).

Does not use interrupts.

No special debouncing outside of some natural debouncing offered by OKEncoder. This fails as knobs get noisier.