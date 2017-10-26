#include <stdlib.h>

#include "ledutil.h"
#include "mon.h"

// main program
int main(int argc, char * argv[]){
	// initalize the monitor
    mon_init();
    // initalize the display
    led_init();

    // counter
	int i;

	// variables to keep track of data
	int kbps = 0;
	// history of speeds
	// range for download speeds
	int kbps_range_d[32] = {0};
	// range for upload speeds
	int kbps_range_u[32] = {0};
	// pointer to current time
	int kbps_p = 0;
	// highest seen download in range
	int highest_d = 1;
	// highest seen upload in range
	int highest_u = 1;
	// timer variable 
	// this will count up to time_before_refresh
	// and then loop back to 0
	int timer = 0;
	// contant for timer to count up to
	int time_before_refresh = 7;

	// variables for current download, and upload
	int d, u;

	while (1){
		// get new info if timer is at 0
		if (timer == 0){
			// get current speeds
   			current_KBps(&d, &u);
   			// put them in the range
			kbps_range_d[kbps_p] = d;
			kbps_range_u[kbps_p] = u;
			// update pointer
			kbps_p = (kbps_p + 1) % 32;
			// get new top speeds
	        highest_d = 1;
	        highest_u = 1;
	        // linear find max
            for (i = 0; i < 32; i++){
                if (highest_d < kbps_range_d[i]){
                    highest_d = kbps_range_d[i];
                }
                if (highest_u < kbps_range_u[i]){
                	highest_u = kbps_range_u[i];
                }
			}
			// reset the screen
		 	clear_screen();
		 	// rebuild the display
            for (i = 0; i < 32; i++){
            	// top row of display acts as a scale,
            	// the higher the highest upload is, the more pixels that are filled in
            	// start from right most pixel and work backwords
            	// every pixel represends 256kbps
                if (highest_u > 256*i){
		            set_pixel(31-i, 0, 1);
		        }
		        // same idea as above
		        if (highest_d > 256*i){
		        	set_pixel(31-i, 8, 1);
		        }
		        // main display
		        // rows 1-7
		        // every column is its own line, height relative to highest seen upload speed
                if (kbps_range_u[(i+kbps_p)%32] > 50){
			    	fill_rectangle(i, 7 - 6*((double)kbps_range_u[(i+kbps_p)%32]/(double)highest_u), i+1, 
			    		8, 1);
			    }
			    // same idea as above
			    if (kbps_range_d[(i+kbps_p)%32] > 50){
			    	fill_rectangle(i, 16 - 7*((double)kbps_range_d[(i+kbps_p)%32]/(double)highest_d), i+1, 
			    		16, 1);
			    }
		    }	    
		}
		// increment timer
		timer = (timer + 1) % time_before_refresh;
		// always refresh the display
		refresh();
	}
}
