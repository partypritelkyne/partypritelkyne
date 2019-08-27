#include "Arduino.h"
#include "../Animation.h"



class AnimationSnake : public Animation
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

    void setPixels(byte correction){
        char pix = currentPixel - correction;
        char prev1 = pix - 1;
        char prev2 = pix - 2;
        char prev3 = pix - 3;
        if (prev1 < 0) prev1 += 16;
        if (prev2 < 0) prev2 += 16;
        if (prev3 < 0) prev3 += 16;
        if (pix < 0) pix += 16;
        setPixelColorToPrimary(pix);
        setPixelColorToPrimary(prev1, 2);
        setPixelColorToPrimary(prev2, 4);
        setPixelColorToPrimary(prev3, 16);
    }

    bool draw()
    {
        pixels.clear();
        setPixels(0);
        setPixels(8);

        if ((currentPixel == 6 || currentPixel == 8) && !alt)
        {
            setInsidePixelsColorToSecondary();
        }

        pixels.show();

        currentPixel++;
        return (currentPixel < 16);
    }

  private:
    byte currentPixel;
};
