#pragma once

#include <Arduino_GigaDisplay.h>
#include <Arduino_GigaDisplayTouch.h>
#include <lvgl.h>
#include <mbed_shared_queues.h>

class GigaDisplayScreensaver
{
public:
    GigaDisplayScreensaver(Arduino_GigaDisplayTouch *touchDetector) : _backlight{GigaDisplayBacklight()}, _touchDetector{touchDetector} {}
    void begin(uint64_t duration)
    {
        _backlight.begin();

        _duration = duration;

        _queue = mbed::mbed_event_queue();
        _queue->call_every(100ms, mbed::callback(this, &GigaDisplayScreensaver::tick));

        _screensaverScreen = lv_obj_create(NULL);
        lv_obj_clear_flag(_screensaverScreen, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_color(_screensaverScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

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
                _prevScreen = lv_disp_get_scr_act(NULL);
                lv_disp_load_scr(_screensaverScreen);
                _backlight.set(0);

                _displayOn = false;
            }
        }
        else if (!_displayOn)
        {
            lv_disp_load_scr(_prevScreen);
            _backlight.set(100);
            _prevScreen = NULL;

            _displayOn = true;
        }
    }

    GigaDisplayBacklight _backlight;
    Arduino_GigaDisplayTouch *_touchDetector;

    uint64_t _duration;

    events::EventQueue *_queue;

    lv_obj_t *_screensaverScreen;
    lv_obj_t *_prevScreen;

    GDTpoint_t *_points;

    uint64_t _lastTouchMillis = 0;
    bool _displayOn = true;
};