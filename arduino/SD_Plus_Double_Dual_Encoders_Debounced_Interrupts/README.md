Uses [PaulStoffregen's code](https://www.pjrc.com/teensy/td_libs_Encoder.html) which will stores the accumulated position of the knob as it rotates. Note that with my encoders, this accumulates 2 position changes with each single rotation so we divide the stored position by 2.

Will use interrupts if it can.

The stored position is debounced which corrects light noise but fails on noisier encoders.