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

        _queue = mbed::mbed_event_queue();
        _queue->call_in(100ms, mbed::callback(this, &GigaDisplayScreensaver::tick));

        _points = new GDTpoint_t[5];
    }

private:
    void tick()
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

        _queue->call_in(100ms, mbed::callback(this, &GigaDisplayScreensaver::tick));
    }

    GigaDisplayBacklight _backlight;
    Arduino_GigaDisplayTouch* _touchDetector;

    uint64_t _duration;

    events::EventQueue* _queue;

    GDTpoint_t* _points;

    uint64_t _lastTouchMillis = 0;
    bool _displayOn = true;
};
