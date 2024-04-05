#include <Arduino_H7_Video.h>
#include <Arduino_GigaDisplayTouch.h>
#include <lvgl.h>
#include <GigaDisplayScreensaver.h>

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;
GigaDisplayScreensaver Screensaver(&TouchDetector);

void setup()
{
    Display.begin();
    TouchDetector.begin();
    Screensaver.begin(5 * 60 * 1000); // 5min

    // Your LVGL UI initialization code
}

void loop()
{
    lv_timer_handler();

    delay(10);
}
