#include "mbed.h"

#define BUTTON_1 p5
#define BUTTON_2 p6
#define BUTTON_3 p7
#define BUTTON_4 p8

#define RED_LED p9
#define YELLOW_LED p10
#define BLUE_LED p11


// Define the Input pins
	//Write your code here
DigitalIn b1(BUTTON_1);
DigitalIn b2(BUTTON_2);
DigitalIn b3(BUTTON_3);
DigitalIn b4(BUTTON_4);

//Define the Output pins
	//Write your code here
DigitalOut rLed1(RED_LED);
DigitalOut yLed2(YELLOW_LED);
DigitalOut bLed3(BLUE_LED);

//Define Input/Output buses
	//Write your code here
BusIn buttonbus(BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4);
BusOut ledbus(BLUE_LED, YELLOW_LED, RED_LED);

void ControlLED_DigitalIO ();
void ControlLED_BusIO ();


int main()
{
    while(1) {
        // ControlLED_DigitalIO ();
        ControlLED_BusIO ();
        wait(0.25);
    }
}

void ControlLED_DigitalIO (){
          //Write your code here
    rLed1 = (!b4&&(!b3||b2)) || (b4&&b3&&b2&&b1);
    yLed2 = (b4^b3)||(b4&&b3&&b2&&b1);
    bLed3 = b4;
}

void ControlLED_BusIO (){
    //Write your code here
    switch(buttonbus){          //ButtonBus is converting the input positions of the switches into binary
        case 0 ... 3:           //to match the decimal case numbers (Button 1 is the first bit .. LSB)
            ledbus = 0b0100;    //LedBus is being set in binary notation. RED_LED is the first bit (LSB)
            break;              //notice BusOut LSB is the last arguement (in binary number position)
        case 4 ... 5:           //but BusIn, the LSB is the first arguement (inverse binary position)
            ledbus = 0b0010;
            break;
        case 6 ... 7:
            ledbus = 0b0110;
            break;
        case 8 ... 11:
            ledbus = 0b0011;
            break;
        case 12 ... 14:
            ledbus = 0b0001;
            break;
        case 15:
            ledbus = 0b0111;
            break;
        default: ;
    }
}
