#include "Arduino.h"
#include "../Animation.h"

class AnimationInverseBlink : public Animation
{

  public:
    void setup()
    {
        on = false;
    }
    int getInterval()
    {
        return animationBaseInterval;
    }

    bool draw()
    {
        on = !on;

        pixels.clear();
        if (on)
        {
            setOutsidePixelsColorToPrimary();
        }
        else
        {
            setInsidePixelsColorToSecondary();
        }
        pixels.show();
        return on;
    }

  private:
    byte on;
};
