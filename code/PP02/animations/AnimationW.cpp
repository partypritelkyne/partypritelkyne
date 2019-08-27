#include "Arduino.h"
#include "../Animation.h"

class AnimationW : public Animation
{
  public:
    void setup()
    {
        currentPixel = 0;
    }

    int getInterval()
    {
        return animationBaseInterval / 7 * 2;
    }

    bool draw()
    {
        pixels.clear();
        if (currentPixel < 8)
        {
            setPixelColorToPrimary(currentPixel);
            setPixelColorToPrimary(15 - currentPixel);

            if(currentPixel > 1){
                setPixelColorToPrimary(currentPixel - 1, 4);
                setPixelColorToPrimary(15 - currentPixel - 1, 4);
            }
            
            if(currentPixel < 7){
                setPixelColorToPrimary(currentPixel + 1, 4);
                setPixelColorToPrimary(15 - currentPixel + 1, 4);
            }

            if (currentPixel % 2 == 0)
            {
                setInsidePixelsColorToSecondary();
            }
        }
        else
        {
            setPixelColorToPrimary(14 - currentPixel);
            setPixelColorToPrimary(currentPixel + 1);

            setPixelColorToPrimary(13 - currentPixel, 4);
            setPixelColorToPrimary(currentPixel + 2, 4);
            
            if(currentPixel > 8){
                setPixelColorToPrimary(15 - currentPixel, 4);
                setPixelColorToPrimary(currentPixel, 4);
            }
        }
        pixels.show();
        currentPixel++;
        return (currentPixel < 14);
    }

  private:
    byte currentPixel;
};
