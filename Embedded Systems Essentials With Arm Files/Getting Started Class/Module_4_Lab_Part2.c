#include "mbed.h"

#define BUTTON_1 p5
#define BUTTON_2 p6 
#define BUTTON_3 p7 
#define BUTTON_4 p8 

#define RED_LED p9
#define YELLOW_LED p10
#define BLUE_LED p11

//Define outputs
DigitalOut led1(RED_LED);
DigitalOut led2(YELLOW_LED);
DigitalOut led3(BLUE_LED);

//Define interrupt inputs
InterrupIn button_1(BUTTON_1);
InterrupIn button_2(BUTTON_2);
InterrupIn button_3(BUTTON_3);
InterrupIn button_4(BUTTON_4);

//Define counters
volatile unsigned int count1;
volatile unsigned int count2;
volatile unsigned int count3;
volatile unsigned int countMax;

void updateLEDs(){

    led1 = 0;
    led2 = 0;
    led3 = 0;

    if(count1 == countMax)led1=1;
    if(count2 == countMax)led2=1;
    if(count3 == countMax)led3=1;
}


//Define ISRs for the interrupts
void button_1_handler(){
	
	count1++;
    if(count1>countMax) countMax = count1;
    updateLEDs();
	
}

void button_2_handler(){
	
	count2++;
    if(count2>countMax) countMax = count2;
    updateLEDs();
	
}

void button_3_handler(){
	
	count3++;
    if(count3>countMax) countMax = count3;
    updateLEDs();
	
}

void button_4_handler(){
	
	led1=0;
    led2=0;
    led3=0;
    led4-0;

    count1=0;
    count2=0;
    count3=0;
    countMax=0;
	 
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
		
	//Initially turn off all LEDs and set all the counters to 0
	
	button_4_handler();

	//Interrupt handlers
	
	button_1.rise(&button_1_handler);
    button_2.rise(&button_2_handler);
    button_3.rise(&button_3_handler);
    button_4.rise(&button_4_handler);

	//wait 100 ms
	while(1)
		wait_ms(100);       // simulates cpu sleeping
}
