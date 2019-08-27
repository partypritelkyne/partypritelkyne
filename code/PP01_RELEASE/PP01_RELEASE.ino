/*
    == PP01_RELEASE ==
    
    Final code used in Party pritelkyne.
    If you have any tips or improvements, don't hesitate to write us!
 
    
    == LED MAPPING ==
     
           B C
          B   C
         B  A  C
        B   A   C
        C   A   B
         C  A  B
          C   B
           C B

    
    == SEE MORE AT ==
    
    http://party-pritelkyne.cz
    https://github.com/partypritelkyne/pp01

    
    == CONTACT THE AUTHOR ==
    
    kontakt@party-pritelkyne.cz
    partypritelkyne@gmail.com

    
    == LICENSE ==

    MIT
    (see the repository root for more info)

 */

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "RunningMedian.h"


#define LEDS_A          2
#define LEDS_B          1
#define LEDS_C          0
#define ON              HIGH
#define OFF             LOW

#define BUTTON_MODE     4
#define BUTTON_BPM      3

#define BUTTON_PRESSED  LOW


/*
 * Animations
 */
byte animationType = 0;           // current animation
int animationInterval = 250;      // interval between animation frames
byte animationFrame = 0;          // current animation frame
unsigned long lastTime = 0;       // last time in milliseconds the frame changed
byte animationEnabled = true;     // whether animations enabled


/*
 * Buttons
 */
bool buttonPressedMode = false;   // whether mode button pressed last time
bool buttonPressedBpm = false;    // whether bpm button pressed last time
unsigned long lastTimeButtonMode = 0; // last time - used for turning off
unsigned long lastTimeButtonBpm = 0; // last time - for bpm analysis
bool modeChangeDisabled = false;    // if changing pwm or turning on, mode change disabled when button released


/*
 * "BPM" analysis
 */
#define WAIT_BETWEEN            3000  // maximum interval
#define INTERVAL_SAMPLES        9     // number of last samples used in average/median
RunningMedian intervalSamples = RunningMedian(INTERVAL_SAMPLES);     // external library
bool bpmAnalysisEnabled = true;

/*
 * PWM - brightness settings
 */
#define PWM_MODES 6
byte currentPwm = 0;
byte pwmSetting = 0;
byte pwmModes[PWM_MODES] = {0, 1, 6, 20, 100, 250};


/*
 * Turn off interval
 */
#define TURN_OFF_INTERVAL 3000


/*
 * Leds state (A, B, C)
 */
bool leds[3] = {0,0,0};




/*
 * ANIMATION DEFINITIONS
 * 
 * If adding new, don't forget to change the count
 * 
 * The LEDs are not controlled directly with digitalWrite, but using "setLeds" function,
 * beacuse the PWM method takes values from the "led" array and switches them rapidly to simulate brightness
 * 
 */
#define ANIMATIONS    8  // animations count
 
void animate(){
    
    switch(animationType){
        case 0:
            bpmAnalysisEnabled = false;
            animationStayOn();
            break;
        case 1:
            bpmAnalysisEnabled = true;
            animationBlinkBasic();
            break;     
        case 2:
            bpmAnalysisEnabled = true;
            animationBlinkInverse();
            break;  
        case 3:
            bpmAnalysisEnabled = true;
            animationBlinkInside();
            break;   
        case 4:
            bpmAnalysisEnabled = false;
            animationOutsideOn();
            break;    
        case 5:
            bpmAnalysisEnabled = false;
            animationInsideOn();
            break;                          
        case 6:
            bpmAnalysisEnabled = true;
            animationCircle();
            break;      
        case 7:
            bpmAnalysisEnabled = true;
            animationRandom();
            break;               
    }
}

// OFF
void animationOff(){
    setLeds(0,0,0);
}

// STAY ON
void animationStayOn(){
    setLeds(1,1,1);
}

// OUTSIDE ON
void animationOutsideOn(){
    setLeds(0,1,1);
}

// INSIDE ON
void animationInsideOn(){
    setLeds(1,0,0);
}

// BLINK BASIC
void animationBlinkBasic(){

    switch(animationFrame){
        case 0:
            setLeds(1,1,1);
            animationFrame++;
            break;
        case 1:
            setLeds(0,0,0);
            animationFrame = 0;
            break;            
    }
}

// BLINK INVERSE
void animationBlinkInverse(){

    switch(animationFrame){
        case 0:
            setLeds(0,1,1);
            animationFrame++;
            break;
       case 1:
            setLeds(1,0,0);
            animationFrame = 0;
            break;            
    }
}

// BLINK INSIDE
void animationBlinkInside(){

    switch(animationFrame){
        case 0:
           setLeds(0,1,1);
            animationFrame++;
            break;
       case 1:
            setLeds(1,1,1);
            animationFrame = 0;
            break;            
    }
}


// CIRCLE
void animationCircle(){

    switch(animationFrame){
        case 0:
        case 2:
        case 4:
            setLeds(0,1,0);
            animationFrame++;
            break;
        case 1:
        case 3:
        case 5:
            setLeds(0,0,1);
            animationFrame++;
            break;
       case 6:
            setLeds(1,0,0);
            animationFrame++;
            break;
       default:
            setLeds(1,1,1);
            animationFrame = 0;
            break;            
    }
}


// RANDOM MODE
void animationRandom(){
    setLeds((random(10) > 8),(random(10) > 7),(random(10) > 7));
}





/* 
 *  The setup function runs once when the battery is inserted (not when "turned off" via button)
 */
void setup() { 
    pinMode(LEDS_A, OUTPUT);
    pinMode(LEDS_B, OUTPUT);
    pinMode(LEDS_C, OUTPUT);

    pinMode(BUTTON_MODE, INPUT_PULLUP);
    pinMode(BUTTON_BPM, INPUT_PULLUP);   
}


/* 
 * The loop function runs over and over again forever
 */
void loop() {

    /*
     * BUTTON MODE switches animations, but after is released (because of PWM and turning off)
     */
    if(digitalRead(BUTTON_MODE) == BUTTON_PRESSED){
        if(!buttonPressedMode){
            buttonPressedMode = true;    
            lastTimeButtonMode = millis();
            delay(1); // little bit of debouncing here and there
        }
        checkWhetherToSleep(); // checking whether to sleep
    }else{
       
        // if mode released, change mode
        if(buttonPressedMode){
            if(!modeChangeDisabled){ // changing modes is disabled after PWM setting and waking up
                lastTime = 0;
                animationType++;
                if(animationType >= ANIMATIONS) animationType = 0;
                animationFrame = 0;
            }
            modeChangeDisabled = false;
        }
        
        buttonPressedMode = false;
        lastTimeButtonMode = 0;
    }

    /*
     * BPM button has different behaviour, whether MODE is pressed or not
     */
    if(!buttonPressedMode){

        /* 
         * Default BPM usage 
         */
        if(bpmAnalysisEnabled && digitalRead(BUTTON_BPM) == BUTTON_PRESSED){
            if(!buttonPressedBpm){
                buttonPressedBpm = true;    
                readBpm();
            }  
        }else{       
            buttonPressedBpm = false;
            /* 
             * If too long turned off, enable animations again without setting a new interval
             */
            if(millis() - lastTimeButtonBpm > WAIT_BETWEEN && animationEnabled == false){
                animationEnabled = true;
            }
        }

    }else{

        /* 
         * If MODE pressed "BPM" does "PWM"
         */
        if(digitalRead(BUTTON_BPM) == BUTTON_PRESSED){
            if(!buttonPressedBpm){
                buttonPressedBpm = true;    
                pwmSetting++;
                if(pwmSetting >= PWM_MODES) pwmSetting = 0;
                modeChangeDisabled = true;
                lastTimeButtonMode = millis(); // to disable sleep for holding MODE too long
            }
    
        }else{       
            buttonPressedBpm = false;
        }     
    }



    /*
     * Doing animation and PWM!
     */
    if(animationEnabled)
    {
        doAnimation();
        crudePwm();
    } 

  
}

/*
 * Whether is time to do animation
 */
void doAnimation(){
    if(millis() - lastTime < animationInterval){  // check whether is the time to do it
        return;
    }

    lastTime = millis();
    animate();
}

/*
 * Reads "BPM" from the button pressings - it's not real "BPM" metering, just an interval metering
 */
void readBpm(){
    long interval = millis() - lastTimeButtonBpm;
    if(interval > 50) // debouncing
    {
        if(interval < WAIT_BETWEEN && lastTimeButtonBpm > 0){ // if not first press
                           
            animationEnabled = true;
            if(intervalSamples.getSize() == 0){ // If no previous meterings (second real press) reset time
                lastTime = 0; // reset last time
                animationFrame = 0; // reset animation frame
            }
          
            intervalSamples.add(interval);
            animationInterval = intervalSamples.getAverage() / 2; // dividing interval by 2 - works for most animations the best
       
        }else{ // first press         
            intervalSamples.clear();
            animationEnabled = false;
            animationOff();
            updateLedsWithoutPwm();
        }
        lastTimeButtonBpm = millis();    
    }
}

/*
 * Checks whether MODE has been pressed for too long, if so, go to sleep - then wake up
 */
void checkWhetherToSleep(){
    if(lastTimeButtonMode == 0) return;
    
    if(millis() - lastTimeButtonMode > TURN_OFF_INTERVAL){     
        animationOff();
        animationEnabled = false;
        updateLedsWithoutPwm();
        delay(200); // for person to have a little time to put their finger off

        // NOW GOING TO SLEEP
        sleep();
        // NOW WAKING UP
        
        lastTimeButtonMode = 0;
        animationEnabled = true;
        modeChangeDisabled = true;
    }
}


/*
 * This is called when the interrupt occurs - button pressed, but we don't need to do anything in it
 */

ISR(PCINT0_vect) {
}


/* 
 *  Doing sleep
 *  
 *  The sleep is interrupted by even shortest pressing MODE,
 *  but then there is a check, wheter the button is pressed for longer time to prevent unwanted waking-ups
 *  
 */
void sleep() {
    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT4);                   // Use PB4 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement
    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    // NOW WAKING UP
    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT4);                  // Turn off PB4 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on
    sei();                                  // Enable interrupts

    // CHECK WHETHER THE BUTTON IS PRESSED FOR 2s, if not, sleep again
    bool slp = false;
    for(byte i = 0; i < 4; i++){
      delay(500);
      if(digitalRead(BUTTON_MODE) != BUTTON_PRESSED){
          slp = true;
          break;
      }
    }
    if(slp) {
      sleep();
    }
}

/*
 * Doing crude software PWM - ATtiny85 has 2 hardware PWM's, but we need 3...
 */
void crudePwm(){

    if(currentPwm > pwmModes[pwmSetting]){
        currentPwm = 0;
    }else{
        currentPwm++;
    }
    
    if(currentPwm == 0){
        digitalWrite(LEDS_A, leds[0]);
        digitalWrite(LEDS_B, leds[1]);
        digitalWrite(LEDS_C, leds[2]);
    }else{
        digitalWrite(LEDS_A, LOW);
        digitalWrite(LEDS_B, LOW);
        digitalWrite(LEDS_C, LOW);
    }
}




/*
 * When LEDS need to be updated directly
 */
void updateLedsWithoutPwm(){

  digitalWrite(LEDS_A, leds[0]);
  digitalWrite(LEDS_B, leds[1]);
  digitalWrite(LEDS_C, leds[2]);
}

/*
 * Shorthand for setting the LEDs
 */
void setLeds(bool a, bool b, bool c){
    leds[0] = a;
    leds[1] = b;
    leds[2] = c;
}

