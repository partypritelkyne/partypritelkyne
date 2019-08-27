#include "Arduino.h"
#include "../Animation.h"

#define SETTINGS_PIXEL_PRIMARY_COLOR 16

class MenuPrimaryColor : public Animation
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
        }
        setInsidePixelsColorToSecondary();
        
        if(secondaryColor[0]==255&&secondaryColor[1]==255&&secondaryColor[2]==255){
            pixels.setPixelColor(SETTINGS_PIXEL_PRIMARY_COLOR, 0, 0, 255);
        }else{
            pixels.setPixelColor(SETTINGS_PIXEL_PRIMARY_COLOR, 255, 255, 255);
        }
        pixels.show();

        return on;
    }

  private:
    byte on;
};
