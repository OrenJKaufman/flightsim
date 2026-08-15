#include "Arduino.h"
#ifndef OKEncoder_h
#define OKEncoder_h

class OKEncoder
{
public:
    OKEncoder(int pin1, int pin2, void (*rotationCallback)(bool clockwise));
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
    bool _isLow(int pin);
};

#endif
