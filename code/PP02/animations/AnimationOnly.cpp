#include "Arduino.h"
#include "../Animation.h"

class AnimationOnly : public Animation
{

  public:
    void setup()
    {
      
    }
    int getInterval()
    {
        return animationBaseInterval;
    }

    bool draw()
    {
        pixels.clear();
        if(!alt){
            setOutsidePixelsColorToPrimary();
        }else{
            setInsidePixelsColorToSecondary();
        }
        pixels.show();
        return false;
    }

 
};
