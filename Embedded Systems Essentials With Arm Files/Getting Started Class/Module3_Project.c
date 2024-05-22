#include "mbed.h"

// Define the Input pins
DigitalIn  START(p5);   // closure = logic 1;
DigitalIn  STOP(p6);    // closure = logic 1;
DigitalIn  GAURD(p7);   // closed condition = logic 1;
DigitalIn  TEMP(p8);    // excess temp = logic 1;

//Define the Output pins
DigitalOut READY_LED(p9);           // LED1 = Motor is ready. Will "flash" when not ready
DigitalOut RUNNING_LED(p10);        // LED2 = Motor is running
DigitalOut GAURD_OPEN_LED(p11);     // LED3 = Fault condition, gaurd opened while motor was running. Will light for 0.5 sec 
DigitalOut EXCESS_TEMP_LED(p12);    // LED4 = Fault condition, excess temperature while motor was running. Will light for 0.5 sec

int main()
{
    while(1) {

        while((GAURD == 0)||(TEMP == 1)){                       // Conditions for not ready
            RUNNING_LED = GAURD_OPEN_LED = EXCESS_TEMP_LED = 0; // reset LEDs
            READY_LED = !READY_LED;                             // Flashing ready
            wait(0.25);                                         // buffer time
        }
        
        while(START==0){                                        // Condition for ready state
            READY_LED = 1;                                      // LED1 On to indicate Ready condition. Waiting for Start to be Pressed
            wait(0.1);                                          // buffer for input start button
        }

        while((GAURD == 1)&&(TEMP == 0)&&(STOP == 0)){          // Condition to keep motor on
            READY_LED = 0;                                      // LED1 turned off since Motor is running
            RUNNING_LED = 1;                                    // LED2 turned on since Motor is running
            wait(0.25);                                         // buffer to let computer see if any of the 3 exit conditions where pressed
        }

        if (GAURD == 0){                                        // defines activity for open gaurd fault condition
            RUNNING_LED=0;
            GAURD_OPEN_LED = 1;
            wait(0.5);
            GAURD_OPEN_LED = 0;
        }
                
        if(TEMP == 1){                                          // defines activity for excess temperature fault condition
            RUNNING_LED = 0;
            EXCESS_TEMP_LED = 1;
            wait(0.5);
            EXCESS_TEMP_LED = 0;
        }
        
        if(STOP == 1){                                          // defines activity for stop botton pressed exit condition
        RUNNING_LED = 0;
        }
    }
}
