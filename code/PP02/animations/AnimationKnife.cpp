#include "Arduino.h"
#include "../Animation.h"

class AnimationKnife : public Animation
{

  public:
    void setup()
    {
        currentPixel = 0;
        secondStage = false;
    }

    int getInterval()
    {
        return animationBaseInterval / 4;
    }

    bool draw()
    {

        pixels.clear();

        if (currentPixel < 8)
        {
            for (byte i = 0; i <= currentPixel; i++)
            {
                if(!secondStage){
                    setPixelColorToPrimary(i);
                    setPixelColorToSecondary(i + 8);

                    if(alt){
                       setPixelColorToSecondary(i / 2 + 16);
                    }
                }else{
                    setPixelColorToSecondary(i);
                    setPixelColorToPrimary(i + 8);

                    if(alt){
                       setPixelColorToSecondary(20 - i / 2);
                    }
                }

               
            }
        }
        else
        {
            for (byte i = currentPixel - 8; i < 8; i++)
            {
                if(!secondStage){
                    setPixelColorToPrimary(i);
                    setPixelColorToSecondary(i + 8);

                    if(alt){
                        setPixelColorToPrimary(i / 2 + 16);
                    }
                }else{
                    setPixelColorToSecondary(i);
                    setPixelColorToPrimary(i + 8);

                    if(alt){
                        setPixelColorToPrimary(19 - i / 2);
                    }
                }

               
            }
        }

        pixels.show();
        currentPixel++;

        if(currentPixel == 16 && !secondStage){
            currentPixel = 0;
            secondStage = true;
        }

        return (currentPixel < 16);
    }

  private:
    byte currentPixel;
    byte secondStage;
};
