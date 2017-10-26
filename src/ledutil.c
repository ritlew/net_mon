#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <pthread.h>
#include <wiringPi.h>

#include "ledutil.h"

// Contant pins
// variables are named after pin names on the schematic
const int RED1 = 4;
const int GREEN1 = 17;
const int BLUE1 = 27;
const int RED2 = 22;
const int GREEN2 = 18;
const int BLUE2 = 10;
const int CLOCK_PIN = 7;
const int A = 9;
const int B = 24;
const int C = 11;
const int LATCH_PIN = 25;
const int OE = 8;

// array for virtual screen
int screen[16][32];

// function for ctrl-c
void quick_abort(int signal){
	// reset all pins
	pinMode(RED1, INPUT);
	pinMode(GREEN1, INPUT);
	pinMode(BLUE1, INPUT);

	pinMode(RED2, INPUT);
	pinMode(GREEN1, INPUT);
	pinMode(BLUE2, INPUT);

	pinMode(A, INPUT);
	pinMode(B, INPUT);
	pinMode(C, INPUT);
	pinMode(CLOCK_PIN, INPUT);
	pinMode(LATCH_PIN, INPUT);
	pinMode(OE, INPUT);

	exit(0);
}

// activate clock pin
void led_clock(){
	digitalWrite(CLOCK_PIN, HIGH);
	delayMicroseconds(1);
	digitalWrite(CLOCK_PIN, LOW);
}

// activate latch pin
void led_latch(){
	digitalWrite(LATCH_PIN, HIGH);
	delayMicroseconds(1);
	digitalWrite(LATCH_PIN, LOW);
}

// sets row from integer
void set_row(int row){
	// mask off the integer bits
	int a_bit = row & 1;
	int b_bit = row & 2;
	int c_bit = row & 4;
	// set the pins
	digitalWrite(A, a_bit);
	digitalWrite(B, b_bit);
	digitalWrite(C, c_bit);
}

// sets the color of the top row (0-7)
void set_color_top(int color){
	int red = color & 1;
	int green = color & 2;
	int blue = color & 4;

	digitalWrite(RED1, red);
	digitalWrite(GREEN1, green);
	digitalWrite(BLUE1, blue);
}

// sets the color of the bottom row (8-15)
void set_color_bottom(int color){
	int red = color & 1;
	int green = color & 2;
	int blue = color & 4;

	digitalWrite(RED2, red);
	digitalWrite(GREEN1, green);
	digitalWrite(BLUE2, blue);
}

// draws the virtual screen onto the physical display
void* refresh(void* test){
	// counters
	int i, j;
	// for every row
	for (i = 0; i < 8; i++){
		// outputting
		digitalWrite(OE, HIGH);
		// current row
		set_row(i);
		// for every column
		for (j = 0; j < 32; j++){
			// set the colors
			set_color_top(screen[i][j]);
			set_color_bottom(screen[i+8][j]);
			// done with this column
			led_clock();
		}
		// done with this row
		led_latch();
		// done with output
		digitalWrite(OE, LOW);
		// wait for physical matrix to update
		delay(2);
	}
}

// this function will draw a rectangle into the virtual screen
// end points are not inclusive
void fill_rectangle(int left, int top, int right, int bottom, int color){
	// counters
	int i, j;
	// nested for loop to fill rectangle
	for (i = left; i < right; i++){
		for (j = top; j < bottom; j++){
			screen[j][i] = color;
		}
	}
}

// sets an individual pixel to a certain color
void set_pixel(int x, int y, int color){
	screen[y][x] = color;
}

// sets every pixel to blank (led not on)
void clear_screen(){
	int i, j;
	for (i = 0; i < 32; i++){
		for (j = 0; j < 16; j++){
		    screen[j][i] = 0;
	    }
	}
}

// initializes the pins, should be called first,
// though fill_rectangle, set_pixel, and clear_screen will still work without this
void led_init(){
	wiringPiSetupGpio();
	signal(SIGINT, quick_abort);

	pinMode(RED1, OUTPUT);
	pinMode(GREEN1, OUTPUT);
	pinMode(BLUE1, OUTPUT);

	pinMode(RED2, OUTPUT);
	pinMode(GREEN1, OUTPUT);
	pinMode(BLUE2, OUTPUT);

	pinMode(A, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(C, OUTPUT);
	pinMode(CLOCK_PIN, OUTPUT);
	pinMode(LATCH_PIN, OUTPUT);
	pinMode(OE, OUTPUT);
}
