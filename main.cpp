/* PROJECT:  MPRO Laboraufgabe 3
 *
 * DEVELOPERS: Jannik Kreucher
 *             Nick Diendorf
 *             Benedikt Wendling
 */
#include "mbed.h"


/*** PIN DEFINITIONS ***/
// Nucleo On-Board
#define LED_NUCLEO  PA_5
#define SW_NUCLEO   PC_13
// Velleman STEM Shield I/O
#define SEG_A	PB_0
#define SEG_B	PC_1
#define SEG_C	PA_10
#define SEG_D	PB_3
#define SEG_E	PB_5
#define SEG_F	PB_4
#define SEG_G	PB_10
#define SEG_DP	PA_8
#define SW_1    PA_9
#define SW_2    PC_7
#define SW_3    PB_6
#define SW_4    PA_7
#define SW_5    PA_6


#define LONG_PRESS	500
#define SHORT_PRESS	200


// 7 Segment Display Data  (dp,g,f,e,d,c,b,a)
const uint8_t segment_data[16] = {
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b01110111, // A
	0b01111100, // B
	0b00111001, // C
	0b01011110, // D
	0b01111001, // E
	0b01110101  // F
};

// display patterns
const uint8_t display_patterns[4][8] = {
	{1, 2, 3, 4, 5, 6, 1, 2},
	{6, 5, 4, 3, 2, 1, 6, 5},
	{1, 2, 3, 4, 5, 6, 1, 2},
	{6, 5, 4, 3, 2, 1, 6, 5}
};
// time patterns
const uint16_t time_patterns[4][8] = {
	{50, 100, 150, 200, 250, 300, 350, 400},
	{50, 100, 150, 200, 250, 300, 350, 400},
	{50, 100, 150, 200, 250, 300, 350, 400},
	{50, 100, 150, 200, 250, 300, 350, 400}
};




// I/O
BusOut busSegmentDisplay(SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_DP);
DigitalIn pinSwitchRoll(SW_1);
DigitalIn pinButtonCheat(SW_2);

// display vars
enum DisplayState {enDisplayStop, enDisplayRun, enDisplayRoll};
DisplayState display_state;

// button vars
uint8_t buttonTimer;




void display() {
	switch (display_state) {
		case enDisplayStop:
			break;
		
		case enDisplayRun:
			break;
		
		case enDisplayRoll:
			break;
	}
}




void check_buttons() {
	// check roll switch
	if(pinSwitchRoll) {
		if(display_state == enDisplayStop) {
			display_state = enDisplayRun;
		}
	}

	// check cheat button
	if(pinButtonCheat) {
		if(buttonTimer < LONG_PRESS+1) {
			if(buttonTimer == LONG_PRESS) {

			}
			buttonTimer++;
		}
	} else {
		buttonTimer = 0;
	}
}
	

int main() {	
	display_state = enDisplayStop;

	// set number: busSegmentDisplay=segment_data[display_patterns[pattern][index]];

    while(1) {

    }
}
