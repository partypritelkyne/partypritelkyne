#include "Arduino.h"
#include "../Animation.h"

/*
   * with 100K and 30K voltage divider and 1.1V ref
   * 4.2V --- 0.969V --- approx 902/1024
   * 2.75V --- 0.635V --- approx 591/1024
   */
#define BATTERY_FULL 902
#define BATTERY_EMPTY 591
#define SETTINGS_PIXEL_VOLTAGE 19

class MenuVoltage : public Animation
{

  public:
    void setup()
    {

        currentPixel = 1;

        // maps battery level to range 0-15
        batteryVal = constrain(map(analogRead(1), BATTERY_EMPTY, BATTERY_FULL, 0, 15), 0, 15);

        // color percentages
        redPerc = 0;
        greenPerc = 255;
        if (batteryVal < 3)
        {
            redPerc = 255;
            greenPerc = 0;
        }
        else if (batteryVal < 5)
        {
            redPerc = 178;
            greenPerc = 77;
        }
        else if (batteryVal < 7)
        {
            redPerc = 153;
            greenPerc = 102;
        }
        else if (batteryVal < 9)
        {
            redPerc = 51;
            greenPerc = 204;
        }
    }

    int getInterval()
    {
        return 65;
    }

    bool draw()
    {

        pixels.clear();
        for (byte i = 0; i < currentPixel; i++)
        {
            if (2 * i <= batteryVal)
            {
                pixels.setPixelColor(i + 8, redPerc, greenPerc, 0);
                pixels.setPixelColor(7 - i, redPerc, greenPerc, 0);
            }
            else if (2 * i - 1 <= batteryVal)
            {
                pixels.setPixelColor(i + 8, redPerc / 3, greenPerc / 3, 0);
                pixels.setPixelColor(7 - i, redPerc / 3, greenPerc / 3, 0);
            }
        }

        // battery voltage indicate
        if(!alt){
            pixels.setPixelColor(SETTINGS_PIXEL_VOLTAGE, 255, 255, 255);
        }

        pixels.show();
        // current pixel animation
        if (currentPixel < 8)
            currentPixel++;

        return true;

        
    }

  private:
    byte currentPixel;
    byte batteryVal;

    byte redPerc;
    byte greenPerc;
};
