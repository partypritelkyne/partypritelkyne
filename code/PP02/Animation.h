#ifndef Animation_h
#define Animation_h

#define PIXEL_COUNT 20

#include "Arduino.h"

class Animation
{
  public:
    virtual void setup();
    virtual int getInterval();
    virtual bool draw();
    bool alt = false;

    void setOutsidePixelsColor(byte r, byte g, byte b)
    {
        for (byte i = 0; i < 16; i++)
        {
            pixels.setPixelColor(i, r, g, b);
        }
    }
    void setInsidePixelsColor(byte r, byte g, byte b)
    {
        for (byte i = 16; i < 20; i++)
        {
            pixels.setPixelColor(i, r, g, b);
        }
    }
    void setAllPixelsColor(byte r, byte g, byte b)
    {
        for (byte i = 0; i < 20; i++)
        {
            pixels.setPixelColor(i, r, g, b);
        }
    }

    void setPixelColorToPrimary(byte pixel)
    {
        pixels.setPixelColor(pixel, primaryColor[0], primaryColor[1], primaryColor[2]);
    }

    void setPixelColorToPrimary(byte pixel, byte divider)
    {
        pixels.setPixelColor(pixel, primaryColor[0] / divider, primaryColor[1] / divider, primaryColor[2] / divider);
    }

    void setPixelColorToSecondary(byte pixel)
    {
        pixels.setPixelColor(pixel, secondaryColor[0], secondaryColor[1], secondaryColor[2]);
    }

    void setPixelColorToSecondary(byte pixel, byte divider)
    {
        pixels.setPixelColor(pixel, secondaryColor[0] / divider, secondaryColor[1] / divider, secondaryColor[2] / divider);
    }

    void setOutsidePixelsColorToPrimary()
    {
        setOutsidePixelsColor(primaryColor[0], primaryColor[1], primaryColor[2]);
    }
    void setInsidePixelsColorToPrimary()
    {
        setInsidePixelsColor(primaryColor[0], primaryColor[1], primaryColor[2]);
    }
    void setOutsidePixelsColorToSecondary()
    {
        setOutsidePixelsColor(secondaryColor[0], secondaryColor[1], secondaryColor[2]);
    }
    void setInsidePixelsColorToSecondary()
    {
        setInsidePixelsColor(secondaryColor[0], secondaryColor[1], secondaryColor[2]);
    }

    void setOutsidePixelsColorToPrimary(byte divider)
    {
        setOutsidePixelsColor(primaryColor[0] / divider, primaryColor[1] / divider, primaryColor[2] / divider);
    }
    void setInsidePixelsColorToPrimary(byte divider)
    {
        setInsidePixelsColor(primaryColor[0] / divider, primaryColor[1] / divider, primaryColor[2] / divider);
    }
    void setOutsidePixelsColorToSecondary(byte divider)
    {
        setOutsidePixelsColor(secondaryColor[0] / divider, secondaryColor[1] / divider, secondaryColor[2] / divider);
    }
    void setInsidePixelsColorToSecondary(byte divider)
    {
        setInsidePixelsColor(secondaryColor[0] / divider, secondaryColor[1] / divider, secondaryColor[2] / divider);
    }
};

#endif
