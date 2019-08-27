#include "Arduino.h"
#include "../Animation.h"

class AnimationSlit : public Animation
{
  public:
    void setup()
    {
        currentPixel = 0;
    }
    int getInterval()
    {
        return animationBaseInterval / 3;
    }

    bool draw()
    {

        pixels.clear();

        if(!alt){
            if (currentPixel == 2)
            {
                setOutsidePixelsColorToPrimary();
            }
            if (currentPixel == 1 || currentPixel == 3)
            {
                setOutsidePixelsColorToPrimary(8);
            }
        }else{
            setOutsidePixelsColorToPrimary();
        }

        if (currentPixel < 4)
        {
            setPixelColorToSecondary(currentPixel + 16);
            if(currentPixel > 1){
                setPixelColorToSecondary(currentPixel + 15, 8);
            }
            if(currentPixel < 3){
                setPixelColorToSecondary(currentPixel + 17, 8);
            }
        }else{
            setPixelColorToSecondary(22 - currentPixel);
            setPixelColorToSecondary(21 - currentPixel, 8);
            setPixelColorToSecondary(23 - currentPixel, 8);
        }
        
        pixels.show();

        currentPixel++;
        return (currentPixel < 6);
    }

  private:
    byte currentPixel;
};
