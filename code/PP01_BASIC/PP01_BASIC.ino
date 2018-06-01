/*
    == PP01_BASIC ==
    
    Simple PP01 example code, which serves as
    a template for creating your own projects.
 
    
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
 
#define LEDS_A          2
#define LEDS_B          1
#define LEDS_C          0

#define BUTTON_MODE     4
#define BUTTON_BPM      3

#define BUTTON_PRESSED  LOW
#define ON              HIGH
#define OFF             LOW


/*
 * Initial function
 */
void setup() {  
    pinMode(LEDS_A, OUTPUT);
    pinMode(LEDS_B, OUTPUT);
    pinMode(LEDS_C, OUTPUT);

    pinMode(BUTTON_MODE, INPUT_PULLUP);
    pinMode(BUTTON_BPM, INPUT_PULLUP);
}


/*
 * Function, that repeats itself
 */
void loop() {

    // when BPM button pressed, LEDS A are on 
    if(digitalRead(BUTTON_BPM) == BUTTON_PRESSED)
    {      
        digitalWrite(LEDS_A, ON);
    }
    else
    {
        digitalWrite(LEDS_A, OFF);
    }

    // when MODE button pressed, LEDS B and C flash
    if(digitalRead(BUTTON_MODE) == BUTTON_PRESSED)
    {   
        digitalWrite(LEDS_B, ON);
        delay(300);
        digitalWrite(LEDS_B, OFF);
        delay(300);        
        digitalWrite(LEDS_C, ON);
        delay(300);
        digitalWrite(LEDS_C, OFF);
        delay(300);
    }
    

}
