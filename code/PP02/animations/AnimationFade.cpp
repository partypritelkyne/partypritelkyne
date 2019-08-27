#include "Arduino.h"
#include "../Animation.h"



class AnimationFade : public Animation
{

  public:
    void setup()
    {
        currentBrightness = 0;
        brightnessDirection = 1;
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
        setOutsidePixelsColorToPrimary();
        setInsidePixelsColorToSecondary();
        
        pixels.show();

      
        return (!(currentBrightness == 0 && brightnessDirection < 0));
    }

  private:
    byte currentBrightness;
    short brightnessDirection;
};
