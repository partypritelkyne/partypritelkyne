
#include <Adafruit_NeoPixel.h>


#include "functions.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define NEOPIXEL_PIN 0
#define NEOPIXEL_COUNT 20

#define BUTTON_MODE 4
#define BUTTON_BPM 3
#define BUTTON_PRESSED LOW

bool buttonPressedMode = false;
bool buttonPressedBpm = false;
unsigned long lastTimeButtonMode = 0; // last time - used for turning off
unsigned long lastTimeButtonBpm = 0;  // last time - for bpm analysis
bool modeChangeDisabled = false;      // if changing pwm or turning on, mode change disabled when button released

unsigned long lastTimeMenuClick = 0;  // last time - for exiting menu check

/*
 * "BPM" analysis
 */
#define WAIT_BETWEEN 3000 // maximum interval
unsigned int intervalSamples = 0;
byte intervalCount = 0;

/*
 * Intervals
 */
#define TURN_OFF_INTERVAL 1000
#define MENU_INTERVAL 1200
#define MENU_TIMEOUT_INTERVAL 10000

#define BRIGHTNESS_MODES 8
byte brightnessSetting = 0;
byte brightnessModes[BRIGHTNESS_MODES] = {22, 40, 64, 128, 255, 8, 12, 16};
byte brightness = brightnessModes[0];

// COLORS
#define COLORS 10
byte primaryColor[3] = {0, 0, 0};
byte secondaryColor[3] = {0, 0, 0};

//byte primaryColorIndex = 6;   // starting at pink
//byte secondaryColorIndex = 3; // starting at yellow

byte primaryColorIndex = COLORS - 1;   // starting at random
byte secondaryColorIndex = COLORS - 1; // starting at random

byte colors[COLORS][3] = {
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
    {255, 255, 0},
    {0, 255, 128},
    {70, 255, 0},
    {204, 0, 70},
    {120, 60, 0},
    {255, 255, 255},
    {0, 0, 0} // last one is hard/coded random
};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int mode = 0;

bool animationEnabled = true;
bool forceRedraw = true;
bool animationEnded = true;
int animationBaseInterval = 250;
int animationWait = 1000;
unsigned long lastTime = 0; // last time in milliseconds the frame changed

#define LED_ENABLE_PIN 1

#define MODES 18

#include "Animation.h"
#include "animations/AnimationBasicBlink.cpp"
#include "animations/AnimationInverseBlink.cpp"
#include "animations/AnimationFlashlight.cpp"
#include "animations/AnimationSlit.cpp"
#include "animations/AnimationSnake.cpp"
#include "animations/AnimationW.cpp"
#include "animations/AnimationFade.cpp"
#include "animations/AnimationKnife.cpp"
#include "animations/AnimationOnly.cpp"
#include "animations/AnimationInverseFade.cpp"
#include "animations/AnimationCount.cpp"
#include "animations/AnimationRain.cpp"
#include "animations/AnimationLgbt.cpp"
#include "menus/MenuVoltage.cpp"
#include "menus/MenuBrightness.cpp"
#include "menus/MenuPrimaryColor.cpp"
#include "menus/MenuSecondaryColor.cpp"

Animation *animation;

#define MENU_MODES 4
bool menuEnabled = false;
byte currentMenuIndex = 0;


void setup()
{

    analogReference(INTERNAL);

    pinMode(BUTTON_MODE, INPUT_PULLUP);
    pinMode(BUTTON_BPM, INPUT_PULLUP);
    pinMode(LED_ENABLE_PIN, OUTPUT);

    //digitalWrite(LED_ENABLE_PIN, LOW);

    delay(100); // Delay to discharge button capacitors

    digitalWrite(LED_ENABLE_PIN, HIGH);
    pixels.begin(); // This initializes the NeoPixel library.
    pixels.setBrightness(brightness);

    //updatePrimaryColor();
    //updateSecondaryColor();

    reloadMode();
}

/* 
 * The loop function runs over and over again forever
 */
void loop()
{
    checkButtons();
    draw();
    if (menuEnabled){
        checkMenuTimeout();
    }
}

void reloadMode()
{

    delete animation; // clears memory from previous animation

    if (!menuEnabled)
    {

        switch (mode)
        {
        case 0:
            animation = new AnimationFlashlight();
            break;
        case 1:
            animation = new AnimationBasicBlink();
            break;
        case 2:
            animation = new AnimationInverseBlink();
            break;
        case 3:
            animation = new AnimationOnly();
            break;
        case 4:
            animation = new AnimationOnly();
            animation->alt = true;
            break;
        case 5:
            animation = new AnimationFade();
            break;
        case 6:
            animation = new AnimationInverseFade();
            break;
        case 7:
            animation = new AnimationSnake();
            break;
        case 8:
            animation = new AnimationSnake();
            animation->alt = true;
            break;
        case 9:
            animation = new AnimationKnife();
            break;
        case 10:
            animation = new AnimationKnife();
            animation->alt = true;
            break;
        case 11:
            animation = new AnimationCount();
            pixels.clear();
            break;
        case 12:
            animation = new AnimationSlit();
            break;
        case 13:
            animation = new AnimationSlit();
            animation->alt = true;
            break;
        case 14:
            animation = new AnimationW();
            break;
        case 15:
            animation = new AnimationRain();
            break;
        case 16:
            animation = new AnimationLgbt();
            break;
        case 17:
            animation = new AnimationLgbt();
            animation->alt = true;
            break;
        }
    }
    else
    {
        switch (currentMenuIndex)
        {
        case 0:
            animation = new MenuPrimaryColor();
            break;
        case 1:
            animation = new MenuSecondaryColor();
            break;
        case 2:
            animation = new MenuBrightness();
            break;
        case 3:
            animation = new MenuVoltage();
            break;
        }
    }


}

void currentModeSetup()
{

    updatePrimaryColor();
    updateSecondaryColor();

    animationEnded = false;
    animation->setup();
    animationWait = animation->getInterval();
    pixels.setBrightness(brightness);
}

void draw()
{
    if (!animationEnabled)
        return;

    if (forceRedraw || animationEnded)
    {
        currentModeSetup();
    }

    if (!forceRedraw && millis() - lastTime < animationWait)
    { // check whether is the time to do it
        return;
    }

    forceRedraw = false;
    lastTime = millis();
    if (!animation->draw())
    {
        animationEnded = true;
    }
}

void nextMode()
{
    forceRedraw = true;
    mode++;
    if (mode >= MODES){
        mode = 0;
    }

    reloadMode();
}

void nextBrightness()
{
    brightnessSetting++;
    if (brightnessSetting >= BRIGHTNESS_MODES){
        brightnessSetting = 0;
    }
    brightness = brightnessModes[brightnessSetting];
    pixels.setBrightness(brightness);
    forceRedraw = true;
}

void nextMenuItem()
{
    currentMenuIndex++;
    if (currentMenuIndex >= MENU_MODES)
    {
        currentMenuIndex = 0;
    }
    lastTimeMenuClick = millis();
    forceRedraw = true;
    reloadMode();
}

void nextPrimaryColor()
{
    forceRedraw = true;
    primaryColorIndex++;
    if (primaryColorIndex >= COLORS)
    {
        primaryColorIndex = 0;
    }
    updatePrimaryColor();
}

void nextSecondaryColor()
{
    forceRedraw = true;
    secondaryColorIndex++;
    if (secondaryColorIndex >= COLORS)
    {
        secondaryColorIndex = 0;
    }
    updateSecondaryColor();
}

void updatePrimaryColor()
{
    if (primaryColorIndex == COLORS - 1)
    {
        createNiceRandomColor(primaryColor);
        return;
    }
    primaryColor[0] = colors[primaryColorIndex][0];
    primaryColor[1] = colors[primaryColorIndex][1];
    primaryColor[2] = colors[primaryColorIndex][2];
}

void updateSecondaryColor()
{
    if (secondaryColorIndex == COLORS - 1)
    {
        createNiceRandomColor(secondaryColor);
        return;
    }
    secondaryColor[0] = colors[secondaryColorIndex][0];
    secondaryColor[1] = colors[secondaryColorIndex][1];
    secondaryColor[2] = colors[secondaryColorIndex][2];
}

void checkButtons()
{

    /*
     * BUTTON MODE switches animations, but after is released 
     */
    if (digitalRead(BUTTON_MODE) == BUTTON_PRESSED)
    {
        if (!buttonPressedMode)
        {
            buttonPressedMode = true;
            lastTimeButtonMode = millis();
            lastTimeButtonBpm = 0;
            animationEnabled = true;
        }
        checkLongPress(); // checking whether to sleep
    }
    else
    {

        // if mode released, change mode
        if (buttonPressedMode)
        {
            if (!modeChangeDisabled && animationEnabled) // changing modes is disabled after longpress or when animation disabled
            {
                if (!menuEnabled)
                {
                    nextMode();
                }
                else
                {
                    nextMenuItem();
                }
            }
            modeChangeDisabled = false;
        }

        buttonPressedMode = false;
        lastTimeButtonMode = 0;
    }

   
    if (digitalRead(BUTTON_BPM) == BUTTON_PRESSED)
    {
        if (!buttonPressedBpm)
        {
            buttonPressedBpm = true;
            if (!buttonPressedMode)
            {
                if (!menuEnabled)
                {
                    readBpm();
                }else{
                    setPressedInMenu();
                    lastTimeButtonBpm = millis();
                }
            }else{
                lastTimeButtonBpm = millis();
            }
            
        }
    }
    else
    {
        buttonPressedBpm = false;
        /* 
            * If too long turned off, enable animations again without setting a new interval
            */
        if (millis() - lastTimeButtonBpm > WAIT_BETWEEN && animationEnabled == false)
        {
            animationEnabled = true;
        }
    }
    
}

void setPressedInMenu()
{
    switch (currentMenuIndex)
    {
    case 0:
        nextPrimaryColor();
        break;
    case 1:
        nextSecondaryColor();
        break;
    case 2:
        nextBrightness();
        break;
    case 3:
        exitMenu();
        break;
    }

    lastTimeMenuClick = millis();
    forceRedraw = true;
}

void exitMenu(){
    forceRedraw = true;
    menuEnabled = false;
    reloadMode();
}

/*
 * Reads "BPM" from the button pressings - it's not real "BPM" metering, just an interval metering
 */
void readBpm()
{
    long interval = millis() - lastTimeButtonBpm;
    if (interval > 20) // debouncing
    {
        if (interval < WAIT_BETWEEN && lastTimeButtonBpm > 0)
        { // if not first press
            animationEnabled = true;
            intervalCount++;
            forceRedraw = true;
            intervalSamples += interval;
            animationBaseInterval = intervalSamples / intervalCount / 2; // dividing interval by 2 - works for most animations the best
        }
        else
        { // first press
            intervalSamples = 0;
            intervalCount = 0;
            animationEnabled = false;
            pixels.clear();
            pixels.show();
        }
        lastTimeButtonBpm = millis();
    }
}

/*
 * Checks whether buttons are pressed for too long
 */
void checkLongPress()
{
    /* Opening/closing menu long press */
    if (millis() - lastTimeButtonMode > MENU_INTERVAL)
    {
        if (!modeChangeDisabled)
        {
            lastTimeMenuClick = millis();
            currentMenuIndex = 0;
            modeChangeDisabled = true;
            forceRedraw = true;
            animationEnabled = true;
            menuEnabled = !menuEnabled;
            reloadMode();
        }
    }

    /* Shutting down long press */
    if (!modeChangeDisabled && buttonPressedBpm && (millis() - lastTimeButtonMode > TURN_OFF_INTERVAL) && (millis() - lastTimeButtonBpm > TURN_OFF_INTERVAL))
    {
        menuEnabled = false;
        forceRedraw = true;
        currentMenuIndex = 0;
        lastTimeButtonMode = 0;
        modeChangeDisabled = true;
        prepareAndSleep();
    }
}

/*
 * Checks whether to exit menu
 */
void checkMenuTimeout()
{
    if (millis() - lastTimeMenuClick > MENU_TIMEOUT_INTERVAL){
        exitMenu();
    }
}

void setPixelColorToWhite(byte pixel){
    pixels.setPixelColor(pixel,255,255,255);
}

void prepareAndSleep()
{
    // TURN OFF ANIMATION
    pixels.clear();
    setPixelColorToWhite(16);
    setPixelColorToWhite(17);
    setPixelColorToWhite(18);
    setPixelColorToWhite(19);
    pixels.show();
    delay(400);

    pixels.clear();
    setPixelColorToWhite(17);
    setPixelColorToWhite(18);
    pixels.show();
    delay(400);

    // NOW GOING TO SLEEP
    sleep();
    // NOW WAKING UP

    // SHOW BATTERY INFO WHEN WAKE UP
    delete animation;
    animation = new MenuVoltage();
    animation->setup();
    animation->alt=true;
    animationWait = animation->getInterval();
    for(byte i=0; i<14; i++){
        animation->draw();
        delay(animationWait);
    }

    // BACK TO NORMAL
    reloadMode();
}

/*
 * This is called when the interrupt occurs - button pressed, but we don't need to do anything in it
 */

ISR(PCINT0_vect)
{
}

/* 
 *  Doing sleep
 *  
 *  The sleep is interrupted by even shortest pressing MODE,
 *  but then there is a check, wheter the button is pressed for longer time to prevent unwanted waking-ups
 *  
 */
void sleep()
{
    // Turn off leds
    digitalWrite(LED_ENABLE_PIN, LOW);
    digitalWrite(NEOPIXEL_PIN, LOW);

    GIMSK |= _BV(PCIE);                  // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT4);                // Use PB4 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // replaces above statement
    sleep_enable();                      // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                               // Enable interrupts
    sleep_cpu();                         // sleep

    // NOW WAKING UP
    cli();                 // Disable interrupts
    PCMSK &= ~_BV(PCINT4); // Turn off PB4 as interrupt pin
    sleep_disable();       // Clear SE bit
    ADCSRA |= _BV(ADEN);   // ADC on
    sei();                 // Enable interrupts

    // CHECK WHETHER THE BUTTONS ARE PRESSED FOR 2s, if not, sleep again
    bool slp = false;
    for (byte i = 0; i < 4; i++)
    {
        delay(500);
        if ((digitalRead(BUTTON_MODE) != BUTTON_PRESSED) || (digitalRead(BUTTON_BPM) != BUTTON_PRESSED))
        {
            slp = true;
            break;
        }
    }
    if (slp)
    {
        sleep();
    }

    // TURN ON LEDS
    digitalWrite(LED_ENABLE_PIN, HIGH);
}
