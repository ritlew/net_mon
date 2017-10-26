#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

// buffer for reading output from ifstat
char _read_buffer[500];

// reduces the output from ifstat
// stores the upload kbps from ifstat in the up parameter
// stores the download kbps from ifstat in the down parameter
void buf_reduce(char* buf, int* down, int* up){
	// working buffer for parsing output
	char * end_buffer = (char *)malloc(100);
	// counter variables
	int i, j = 0;
	// flag for whether upload values has been seen or not
	int down_seen = 0;
	// loop through output
	for (i = 0; i < strlen(buf); i++){
		// make sure the character isnt a space
		if (buf[i] != ' '){
			// if its a decimal, we have seen all the whole number parts of
			// the output we are looking for
			if (buf[i] == '.'){
				// if download has already been handled
				if (down_seen){
					// end the buffer
					end_buffer[j] = '\0';
					// set the variable
					*up = atoi(end_buffer);
					// get rid of the buffer
					free(end_buffer);
					// done
					return;
				// if download has not been handled yet
				} else {
					// end the buffer
					end_buffer[j] = '\0';
					// set the variable
					*down = atoi(end_buffer);
					// get ready to see upload
					down_seen = 1;
					// 
					j = 0;
					// there is 3 extra characters to skip
					i += 3;
				}
			} else {
				// place the char in the buffer, it should be a number
				end_buffer[j++] = buf[i];
			}
		}
	}
}

// read the data from the output of ifstat
void current_KBps(int* down, int* up){
	// zero out the buffer
    bzero(_read_buffer, 500);
    // read in
	read(STDIN_FILENO, _read_buffer, 500);
	// reduce the output which will set the two variables
	buf_reduce(_read_buffer, down, up);
}

// initalizes ifstat, the program we get the network information from
void mon_init(){
	//average pipe, fork, dup, exec structure
	pid_t pid;
    int fd[2];

    pipe(fd);
    pid = fork();

    if (pid == 0){
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        // ifstat gives upload and download speeds
        // "ifstat"
        // "-n" : dont display the header
        // "-i", "wlan0" : read the network interface "wlan0"
        // "0.1" : refresh every .1 seconds
        char* ifstat[] = {"ifstat", "-n", "-i", "wlan0", "0.1", NULL};
        execvp(ifstat[0], ifstat);
    } else { 
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
    }
}
