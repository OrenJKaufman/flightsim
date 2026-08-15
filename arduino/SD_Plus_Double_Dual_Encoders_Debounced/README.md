This is currently uploaded to the double dual encoder arduino.

Uses [Ben Buxton's code](https://github.com/buxtronix/arduino/blob/master/libraries/Rotary/README.md) for storing the state of the knob.

Does not use interrupts.

digitalReads of pin1 and pin2 are debounced. The state table is supposed to handle debouncing but I was still getting intermittent incorrect rotations. Probably works better with full step encoders.