#include "Arduino.h"
#include "Encoder.h"

Encoder::Encoder(int pin1, int pin2, void (*rotationCallback)(bool clockwise))
{
    _pin1 = pin1;
    _pin2 = pin2;
    _rotationCallback = rotationCallback;

    _movingClockwise = false;
    _movingCounterClockwise = false;
}

void Encoder::begin()
{
    pinMode(_pin1, INPUT_PULLUP);
    pinMode(_pin2, INPUT_PULLUP);

    if (digitalRead(_pin1) == LOW && digitalRead(_pin2) == LOW)
    {
        _truthes = true;
    }
    else
    {
        _truthes = false;
    }
}

void Encoder::process()
{
    _pin1Val = digitalRead(_pin1) == LOW;
    _pin2Val = digitalRead(_pin2) == LOW;

    if (_movingClockwise || _movingCounterClockwise) {
        _processMoving();
    } else {
        _processNotMoving();
    }
}

bool Encoder::_bothFalse() {
    return !(_pin1Val || _pin2Val);
}

bool Encoder::_bothTrue() {
    return (_pin1Val && _pin2Val);
}

void Encoder::_processNotMoving() {
    if (_truthes && _bothFalse()) {
        _truthes = false;
        return;
    }
    
    if (!_truthes && _bothTrue()) {
        _truthes = true;
        return;
    }

    if (_truthes ^ _pin1Val) {
        _movingClockwise = true;
    } else if (_truthes ^ _pin2Val) {
        _movingCounterClockwise = true;
    }
}

void Encoder::_processMoving() {
    if ((_truthes && _bothFalse()) || (!_truthes && _bothTrue())) {
        if (_movingClockwise) {
            _movingClockwise = false;
            _rotationCallback(true);
        } else {
            _movingCounterClockwise = false;
            _rotationCallback(false);
        }
        _truthes = _pin1Val;
    }
    else if ((_truthes && _bothTrue()) || (!_truthes && _bothFalse())) {
        _movingClockwise = _movingCounterClockwise = false;
    }
}
