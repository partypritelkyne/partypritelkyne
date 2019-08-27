#include "Arduino.h"
#include "../Animation.h"



class AnimationInverseFade : public Animation
{

  public:
    void setup()
    {
        currentBrightness = 0;
        brightnessDirection = 1;
        out = true;
    }

    int getInterval()
    {
        return animationBaseInterval / brightness;
    }

    bool draw()
    {
        if(currentBrightness + brightnessDirection > brightness){
            brightnessDirection = -brightnessDirection;
        }
        currentBrightness += brightnessDirection;

        pixels.clear();        
        pixels.setBrightness(currentBrightness);

        if(out){
            setOutsidePixelsColorToPrimary();
        }
        else{
            setInsidePixelsColorToSecondary();
        }
        
        pixels.show();


        if(currentBrightness == 0 && brightnessDirection < 0 && out){
            out = false;
            brightnessDirection = 1;
        }

      
        return (!(currentBrightness == 0 && brightnessDirection < 0 && !out));
    }

  private:
    byte currentBrightness;
    short brightnessDirection;
    bool out;
};
