#pragma once

#include <Arduino_GigaDisplay.h>
#include <Arduino_GigaDisplayTouch.h>
#include <rtos.h>

class GigaDisplayScreensaver
{
public:
    GigaDisplayScreensaver(Arduino_GigaDisplayTouch* touchDetector) : _backlight{ GigaDisplayBacklight() }, _touchDetector{ touchDetector } {}
    void begin(uint64_t duration)
    {
        _backlight.begin();

        _duration = duration;

        _thread.start(mbed::callback(this, &GigaDisplayScreensaver::loop));

        _points = new GDTpoint_t[5];
    }

private:
    void loop()
    {
        while (true)
        {
            uint8_t contacts = _touchDetector->getTouchPoints(_points);

            uint64_t millisNow = millis();

            if (contacts > 0)
            {
                _lastTouchMillis = millisNow;
            }

            if (millisNow - _lastTouchMillis > _duration)
            {
                if (_displayOn)
                {
                    _backlight.set(0);

                    _displayOn = false;
                }
            }
            else if (!_displayOn)
            {
                _backlight.set(100);

                _displayOn = true;
            }

            rtos::ThisThread::sleep_for(100ms);
        }
    }

    GigaDisplayBacklight _backlight;
    Arduino_GigaDisplayTouch* _touchDetector;

    uint64_t _duration;

    rtos::Thread _thread;

    GDTpoint_t* _points;

    uint64_t _lastTouchMillis = 0;
    bool _displayOn = true;
};
