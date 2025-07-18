#include "Arduino.h"
#ifndef Encoder_h
#define Encoder_h

class Encoder
{
public:
    Encoder(int pin1, int pin2, void (*rotationCallback)(bool clockwise));
    void begin();
    void process();

private:
    int _pin1;
    int _pin2;
    bool _truthes;
    bool _movingClockwise;
    bool _movingCounterClockwise;
    bool _pin1Val;
    bool _pin2Val;
    void (*_rotationCallback)(bool clockwise);
    bool _bothFalse();
    bool _bothTrue();
    void _processNotMoving();
    void _processMoving();
};

#endif
