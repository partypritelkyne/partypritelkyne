#include "Arduino.h"
#include "../Animation.h"

#define SETTINGS_PIXEL_BRIGHTNESS 18

class MenuBrightness : public Animation
{

  public:
    void setup()
    {
        on = false;
    }

    int getInterval()
    {
        return 150;
    }

    bool draw()
    {

        pixels.clear();
        on = !on;
        if (on)
        {
            setOutsidePixelsColorToPrimary();
            setInsidePixelsColorToSecondary();
        }

        pixels.setPixelColor(SETTINGS_PIXEL_BRIGHTNESS, 255, 255, 255);
        pixels.show();

        return on;
    }

  private:
    byte on;
};
