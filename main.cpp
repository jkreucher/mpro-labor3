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
const uint8_t display_patterns[][] = {
	{1, 2, 3, 4, 5, 6},
	{6, 5, 4, 3, 2, 1}
};


// I/O
BusOut display_bus(SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_DP);



class SegmentDisplay {
	private:
		BusOut *display;
		uint8_t index, enable;
		Timer timer;
	public:
		SegmentDisplay(BusOut* displayBus) {
			display = displayBus;
			index = 0;
			pattern = 0;
			// start timer
			timer.start();
		}
		
		void show(uint8_t nibble) {
			// show number on 7 segment display
			*display = segment_data[nibble & 0xF];
		}
		
		void roll(uint8_t p) {
			pattern = p;
			enable = 1;
		}
		
		void stop() {
			enable = 0;
			index = 0;
		}
		
		void loop() {
			if(timer.elapsed_time() >= chrono::milliseconds(50)) {
				timer.reset();
				
				if(enable == 1) {
					// show pattern
					this.show(display_patterns[pattern][index]);
					index++;
					if(index >= sizeof(display_patterns[pattern])) index = 0;
				} else {
					// turn off
					*display = 0;
				}
			}
		}
};
	



int main() {	
	SegmentDisplay display(&display_bus);
	
	display.roll();

    while(1) {
		display.loop();
    }
}
