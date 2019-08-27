#include "Arduino.h"
#include "../Animation.h"



class AnimationRain : public Animation
{

  public:
    void setup()
    {
        currentPixel = 0;
    }

    int getInterval()
    {
        return animationBaseInterval / 4;
    }

    bool draw()
    {
        pixels.clear();

        
        // A = first drop
        char p1 = currentPixel;
        char p2 = currentPixel - 1;
        char p3 = currentPixel - 2;

        if(p1 < 8) setPixelColorToPrimary(p1);
        if(p2 >= 0 && p2 < 8) setPixelColorToPrimary(p2, 2);
        if(p3 >= 0 && p3 < 8) setPixelColorToPrimary(p3, 4);

        p1 = 22 - currentPixel;
        p2 = 23 - currentPixel;
        p3 = 24 - currentPixel;
        if(p1 > 7 && p1 < 16) setPixelColorToPrimary(p1);
        if(p2 > 7 && p2 < 16) setPixelColorToPrimary(p2, 2);
        if(p3 > 7 && p3 < 16) setPixelColorToPrimary(p3, 4);

        p1 = currentPixel - 2;
        p2 = currentPixel - 3;
        p3 = currentPixel - 4;
        if(p1 > 15 && p1 < 20) setPixelColorToSecondary(p1);
        if(p2 > 15 && p2 < 20) setPixelColorToSecondary(p2, 2);
        if(p3 > 15 && p3 < 20) setPixelColorToSecondary(p3, 4);
  
        pixels.show();

        currentPixel++;
        return (currentPixel < 32);
    }

  private:
    byte currentPixel;
};
