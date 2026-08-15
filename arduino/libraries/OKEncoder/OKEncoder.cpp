#include "Arduino.h"
#include "OKEncoder.h"

OKEncoder::OKEncoder(int pin1, int pin2, void (*rotationCallback)(bool clockwise))
{
    _pin1 = pin1;
    _pin2 = pin2;
    _rotationCallback = rotationCallback;

    _movingClockwise = false;
    _movingCounterClockwise = false;
}

void OKEncoder::begin()
{
    pinMode(_pin1, INPUT_PULLUP);
    pinMode(_pin2, INPUT_PULLUP);

    if (_isLow(_pin1) && _isLow(_pin2))
    {
        _truthes = true;
    }
    else
    {
        _truthes = false;
    }
}

void OKEncoder::process()
{
    _pin1Val = _isLow(_pin1);
    _pin2Val = _isLow(_pin2);

    if (_movingClockwise || _movingCounterClockwise) {
        _processMoving();
    } else {
        _processNotMoving();
    }
}

bool OKEncoder::_isLow(int pin) {
    return digitalRead(pin) == LOW;
}

bool OKEncoder::_bothFalse() {
    return !(_pin1Val || _pin2Val);
}

bool OKEncoder::_bothTrue() {
    return (_pin1Val && _pin2Val);
}

void OKEncoder::_processNotMoving() {
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

void OKEncoder::_processMoving() {
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
