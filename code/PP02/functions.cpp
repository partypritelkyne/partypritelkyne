#include "Arduino.h"
#include "functions.h"


void createNiceRandomColor(byte *color){
    byte primaryColor = random(0,3);
    byte secondaryColor = primaryColor + random(1,3);
    if(secondaryColor > 2) secondaryColor -= 3; 
    
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    color[primaryColor] = random(128, 255);
    color[secondaryColor] = random(0, 128);

}

