#include "mbed.h"

#define SPEAKER p21
#define LED p12
#define AIN1 p15
#define AIN2 p16

// Define LED Output
DigitalOur led(LED);    // used for volume under 75% use case in tank

// Define the PWM speaker output
PwmOut speaker(SPEAKER);

// Define analog (Potentiometer) inputs
AnalogIn pot1(AIN1);    // acts as the sensor for the volume of the tank
AnalogIn pot2(AIN2);    // acts as the sensor for the temp of the tank

//Define variables for pots and for loop counter and LED flash
float val1;
float val2;
float i;
bool flashingLed;
float lowFreq;
float highFreq;
/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int main(){
    while(1){
        speaker = 7 // speaker consistent volume 
        
        // reading the value of the potentiometers
        val1 = pot1.read(); // reading the volume of liquid in the tank. Considered low if under 75% full
        val2 = pot2.read(); // reading the temp of the liquid in the tank

        // looking for different ouput cases to set the sound wave appropriately

        if (val1 < 0.75){    // low tank volume - flashing led and frequency modulated warble sound
            flashingLed = true;

        }
        else{               // when tank is considered full - need to consider temperature cases
            if (val2 > 0.66){       // tank temp is high - two - tone
                lowFreq = 20;
                highFreq = 19357;

            }
            else if (val2 < 0.33){  // tank temp is low - single beeping tone
                lowFreq = 20
                highFreq = 0;
            }
            else {                  // tank temp is good - continous tone
                lowFreq = 1000;
                highFreq = 1000;
            }
        }

        // Creating sound waves based on potentiometer readings

        // low frequency wave
        for (i=0; i<1; i+=0.5){
            speaker.period(1/lowFreq);  // controls the pitch (freq)
        }

        // high frequency wave
        for (i=0; i<1; i+=0.5){
            speaker.period(1/highFreq);  // controls the pitch (freq)
        }

        wait_ms(2000);  // 2 second delay
        led = 0;
    }
}
