#include "Arduino.h"
#include "../Animation.h"



class AnimationLgbt : public Animation
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
      
        pixels.setPixelColor((currentPixel) % 16, 228, 3, 3);
        pixels.setPixelColor((currentPixel + 1) % 16, 230, 87, 2);
        pixels.setPixelColor((currentPixel + 2) % 16, 255, 140, 0);
        pixels.setPixelColor((currentPixel + 3) % 16, 255, 201, 38);
        pixels.setPixelColor((currentPixel + 4) % 16, 255, 237, 0);
        pixels.setPixelColor((currentPixel + 5) % 16, 169, 216, 7);
        pixels.setPixelColor((currentPixel + 6) % 16, 40, 255, 3);
        pixels.setPixelColor((currentPixel + 7) % 16, 0, 128, 38);
        pixels.setPixelColor((currentPixel + 8) % 16, 1, 163, 126);
        pixels.setPixelColor((currentPixel + 9) % 16, 0, 147, 234);
        pixels.setPixelColor((currentPixel + 10) % 16, 0, 120, 245);
        pixels.setPixelColor((currentPixel + 11) % 16, 0, 77, 255);
        pixels.setPixelColor((currentPixel + 12) % 16, 115, 0, 140);
        pixels.setPixelColor((currentPixel + 13) % 16, 120, 7, 120);
        pixels.setPixelColor((currentPixel + 14) % 16, 180, 11, 103);
        pixels.setPixelColor((currentPixel + 15) % 16, 255, 0, 0);


        if(alt){
            if(currentPixel == 0){
                setInsidePixelsColor(1, 220, 50);
            }
            if(currentPixel == 4){
                setInsidePixelsColor(90, 2, 255);
            }

            if(currentPixel == 8){
                setInsidePixelsColor(220, 100, 0);
            }

            if(currentPixel == 12){
                setInsidePixelsColor(240, 0, 0);
            }
        }
      
        pixels.show();

        currentPixel++;
        return (currentPixel < 16);
    }

  private:
    byte currentPixel;
};
