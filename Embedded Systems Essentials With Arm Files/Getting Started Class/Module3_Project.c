#include "mbed.h"

#define START p5
#define STOP p6
#define GAURD P7
#define TEMP p8

#define READY p9
#define RUNNING p10
#define GAURD_OPEN p11
#define EXCESS_TEMP P12


// Define the Input pins
DigitalIn  b1(START);
DigitalIn  b2(STOP);
DigitalIn  b3(GAURD);
DigitalIn  b4(TEMP);

//Define the Output pins
DigitalOut gLed(READY);
DigitalOut bLed(RUNNING);
DigitalOut rLed(GAURD_OPEN);
DigitalOut yLed(EXCESS_TEMP);


//Define Input/Output buses
//BusIn buttonsbus (BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4);
//BusOut ledsbus(BLUE_LED,YELLOW_LED,RED_LED);


void ControlLED_DigitalIO ();
void ControlLED_BusIO ();


int main()
{
    while(1) {
        ControlLED_DigitalIO ();
        // ControlLED_BusIO ();
        wait(0.25);
    }
}

void ControlLED_DigitalIO (){
    rled = (!b4&&(!b3||b2)) || (b4&&b3&&b2&&b1);
    yled= (b4^b3) || (b4&&b3&&b2&&b1);
    bled = b4;
}

//void ControlLED_BusIO (){
    switch (buttonsbus){
        case 0 ... 3:
            ledsbus=0b0100;
            break;
        case 4 ... 5:
            ledsbus=0b0010;
            break;
        case 6 ... 7:
            ledsbus=0b0110;
            break;
        case 8 ... 11:
            ledsbus=0b0011;
            break;
        case 12 ... 14:
            ledsbus=0b0001;
            break;
        case 15:
            ledsbus=0b0111;
            break;
        default: ;
    }
}