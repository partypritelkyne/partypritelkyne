#include "Arduino.h"
#include "../Animation.h"

class AnimationCount : public Animation
{

  public:
    void setup()
    {
        currentPixel = 0;
    }

    int getInterval()
    {
        return animationBaseInterval / 8;
    }

    bool draw()
    {

        //pixels.clear();

        
        for (byte i = 0; i <= currentPixel; i++)
        {
            setPixelColorToPrimary(i);
        }
        
        setInsidePixelsColorToSecondary();

        pixels.show();
        currentPixel++;

        return (currentPixel < 16);
    }

  private:
    byte currentPixel;
};
