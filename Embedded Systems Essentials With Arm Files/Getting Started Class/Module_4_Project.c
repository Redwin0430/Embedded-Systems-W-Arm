#include "mbed.h"

#define BUTTON_1 p5     // button for child request entry
#define BUTTON_2 p6     // button for adult request entry
#define BUTTON_3 p7     // switch to set Fire Alarm ON

#define RED_LED p8              // LED for "No Entry"
#define YELLOW_LED_BARB1 p10    // LED for Barber1 Seat
#define YELLOW_LED_BARB2 p11    // LED for Barber2 Seat
#define YELLOW_LED_BARB3 p12    // LED for Barber3 Seat
#define WHITE_LED p13           // Wall Clock Timer
#define BLUE_LED_S1 p14         // Waiting Seat 1
#define BLUE_LED_S2 p15         // Waiting Seat 2
#define BLUE_LED_S3 p16         // Waiting Seat 3
#define BLUE_LED_S4 p17         // Waiting Seat 4
#define BLUE_LED_S5 p18         // Waiting Seat 5
#define BLUE_LED_S6 p19         // Waiting Seat 6
#define BLUE_LED_S7 p20         // Waiting Seat 7
#define BLUE_LED_S8 p21         // Waiting Seat 8

//Haircut Timing for each chair
int time_Barb1, time_Barb2, time_Barb3;

int seat_Pat;       // seating pattern
int child_waiting;   // # of children and parents waiting (counted as pairs)
int adult_waiting;  // # of adults waiting

//Define outputs
DigitalOut noEntry(RED_LED);
DigitalOut barber1(YELLOW_LED_BARB1);
DigitalOut barber2(YELLOW_LED_BARB2);
DigitalOut barber3(YELLOW_LED_BARB3);
DigitalOut wallClock(WHITE_LED);
BusOut seating (BLUE_LED_S1,BLUE_LED_S2,BLUE_LED_S3,BLUE_LED_S4,BLUE_LED_S5,BLUE_LED_S6,BLUE_LED_S7,BLUE_LED_S8);

//Define interrupt inputs
InterruptIn childReqEntry(BUTTON_1);
InterruptIn AdultReqEntry(BUTTON_2);
InterruptIn fireAlarm(BUTTON_3);

// Resets all LEDd
void resetLEDs(){

    noEntry = 0;
    barber1 = 0;
    barber2 = 0;
    barber3 = 0;
    wallClock = 0;
    seating = 0b00000000;

}


//Define ISRs for the interrupts
void childReqEntry_handler(){
    if (seating <= 0b00111111)      // checks to see if there is enough seats for both a child and a parent 
    {
        child_waiting++;            // child and parent pair counted as 1
        seat_Pat = seat_Pat << 2;   // shifts seats aside by 2 to fit the child and parent pair
        seat_Pat = seat_Pat + 3;    // adding 3 bc 3 in binary is 0b00000011  to light up the appropriate LEDs
        seating = seat_Pat;         // updates the seating LED array with new seating pattern
    }
	else{
        noEntry = 1;                // denies entry with a small buffer
        wait_ms(300);
        noEntry = 0;
    }	
}

void adultReqEntry_handler(){
    if (seating != 0b11111111)      // checks to see if ther is enough seats for 1 adult seat
    {
        adult_waiting++;            // increased the adult wating by 1
        seat_Pat = seat_Pat << 1;   // shifts seating arrangement by 1 to account for the new adult
        seat_Pat = seat_Pat + 1;    // adds the missing 1 binary to account for new adult
        seating = seat_Pat;         // updates the seating LED array with new seating pattern
    }
	else{
        noEntry = 1;                // denies entry with a small buffer
        wait_ms(300);
        noEntry = 0;
    }
}

void fireAlarm_handler(){
    noEntry = 1;    // no entry LED turns on
    
    //  Clears all barber seats in an orderly fashion
    if (barber1){
        barber1 = 0;
        wait_ms(300);
    }
    if (barber2){
        barber2 = 0;
        wait_ms(300);
    }
    if (barber3){
        barber3 = 0;
        seat_Pat = seat_Pat>>1;     // clears the associated parent for the child at barber seat 3
        wait_ms(300);
    }


    while (child_waiting != 0){     // clears child and parents waiting 
        seat_Pat = seat_Pat >> 2;
        seating = seat_Pat;
        child_waiting --;
        wait_ms(300);                // to simulate people leaving in an orderly fashion
    }

    while (adult_waiting != 0){
        seat_Pat = seat_Pat >> 1;
        seating = seat_Pat;
        adult_waiting--;
        wait_ms(300);                // to simulate people leaving in an orderly fashion
    }

    adult_waiting = 0;              // reset counters
    child_waiting = 0;
    wait_ms(600);                   //  fire department pulled up and said ya yall good 
    resetLEDs();
    
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
		
	//Initially turn off all LEDs and set all the counters to 0
	
	resetLEDs();

	//Interrupt handlers
	
	childReqEntry.rise(&childReqEntry_handler);
    AdultReqEntry.rise(&adultReqEntry_handler);
    fireAlarm.rise(&fireAlarm_handler);
    
	//main body that handles current waiting room, barber seat arrangements, and wall clock
	while(1){

        if (barber1 == 0){                              // Checks to see if barber 1 chair is free
            if (seating != 0 && adult_waiting > 0){     // checks to see if the are any adults waiting in the seating area. True when there is an adult waiting
                barber1 = 1;                            // lights Barber 1 chair on
                time_Barb1 = 13;                        // sets the timer to include the 12 sec haircut and 1 sec rest time
                adult_waiting--;                        // decrease the number of adults waiting in the waiting area
                seat_Pat = seat_Pat>>1;                 // updats the seating pattern of the waiting area
            }
        }
        
        else if (barber1 == 1){                         // Logic for when Barber 1 is actively ocupied
            time_Barb1--;                               // decreases cut and rest time counter
            if (time_Barb1 == 0){                       // when timer hits 0, seat is cleared
                barber1 = 0;
            }
        }

    // same logic as Barber 1
        if (barber2 == 0){
            if (seating != 0 && adult_waiting > 0){
                barber2 = 1;
                time_Barb2 = 13;
                adult_waiting--;
                seat_Pat = seat_Pat>>1;
            }
        }
        
        else if (barber2 == 1){
            time_Barb2--;
            if (time_Barb2 == 0){
                barber2 = 0;
            }
        }

    // Logic for the child only Barber 3
        if (barber3 == 0){                              // Logic for when Barber 3 chair is empty
            if (seating != 0 && child_waiting > 0){     // Checks to see if there is a child waiting in the waiting area
                barber3 = 1;                            // sets Barber 3 light on when a child is in the waiting area and moves to barber seat
                time_Barb3 = 13;                        // sets the Barber 3 clock
                child_waiting--;                        // decreases the child waiting count
                seat_Pat = seat_Pat>>1;                 // updates the seating pattern to reflect child moving from the waiting area to barber 3 seat
            }
        }
        
        else if (barber3 == 1){                         // Logic for when Barber 3 seat is occupied
            time_Barb3--;                               // Decreased the barber 3 clock timer
            if (time_Barb3 == 0){                       // When Barber 3 timer hits 0, seat is clears and seating pattern is updated to reflect parent
                barber3 = 0;                            // leaving with the child
                seat_Pat = seat_Pat>>1;
            }
        }

        seating = seat_Pat; // update seating display;

		wait_ms(1000);       // to simulate the 1 sec pass time

        wallClock = !wallClock;     // clock ticks
    }

}
