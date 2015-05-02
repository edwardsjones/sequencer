/* COMP2215 Task 6  Model Answer */

#include "os.h"
#include "audio.h"
#include <util/delay.h>


int blink(int);
int update_dial(int);
int collect_delta(int);
int check_switches(int);
void increment_song();
void decrement_song();
void play_current_song();

/* Lines have to be shorter than this. Note: buffer is on stack. */
#define LINE_BUFF_LEN 55    
#define SONGS 9

FIL File;  						/* FAT File */

int song_number = 0;
int position = 0;
int done = 0;

void main(void) {

    os_init();
    os_add_task( blink,            30, 1);
    os_add_task( collect_delta,   500, 1);
    os_add_task( check_switches,  100, 1);
     
    sei();
    for(;;){}
    
}

int collect_delta(int state) {
	position += os_enc_delta();
	return state;
}


int check_switches(int state) {
	
	if (get_switch_press(_BV(SWE))) {
                increment_song();
	}
		
	if (get_switch_press(_BV(SWW))) {
                decrement_song();
	}
		
	if (get_switch_long(_BV(SWC))) {
            play_current_song();
	}

	return state;	

}

void increment_song() {
    if (song_number < 8)
        song_number++;
}

void decrement_song() {
    if (song_number > 0)
        song_number--;
}

int blink(int state) {
	static int light = 0;
	uint8_t level;
	
	if (light < -120) {
		state = 1;
	} else if (light > 254) {
		state = -20;
	}


	/* Compensate somewhat for nonlinear LED 
       output and eye sensitivity:
    */
	if (state > 0) {
		if (light > 40) {
			state = 2;
		}
		if (light > 100) {
			state = 5;
		}
	} else {
		if (light < 180) {
			state = -10;
		}
		if (light < 30) {
			state = -5;
		}
	}
	light += state;

	if (light < 0) {
		level = 0;
	} else if (light > 255) {
		level = 255;
	} else {
		level = light;
	}
	
	os_led_brightness(level);
	return state;
}

void play_current_song() {

        int8_t i, j;
        char line[SONGS][LINE_BUFF_LEN];
        i = 0;
        j = 0;
        f_mount(&FatFs, "", 0);
        if (f_open(&File, "tab.txt", FA_READ) == FR_OK) {
            for (i = 0; i < 9; i++) {
                f_gets(line[i], LINE_BUFF_LEN, &File);
            }
            f_close(&File);
        }

        while (line[song_number][j] != '\n') {        
            
            f_mount(&FatFs, "", 0);

            if (line[song_number][j] == 49) {            
                f_open(&File,"s1.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            } else if (line[song_number][j] == 50) {
                f_open(&File,"s2.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            } else if (line[song_number][j] == 51) {
                f_open(&File,"s3.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            } else if (line[song_number][j] == 52) {
                f_open(&File,"s4.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            } else if (line[song_number][j] == 53) {
                f_open(&File,"s5.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            } else if (line[song_number][j] == 54) {
                f_open(&File,"s6.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            } else if (line[song_number][j] == 55) {
                f_open(&File,"s7.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            } else if (line[song_number][j] == 56) {
                f_open(&File,"s8.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            } else if (line[song_number][j] == 57) {
                f_open(&File,"s9.wav", FA_READ);
                audio_load(&File);
                f_close(&File);
            }

            j++;

/*          f_mount(&FatFs, "", 0);    
            for (j = 0; j < n; j++) {
                if (f_open(&File, "results.txt", FA_WRITE) == FR_OK) {
                        f_lseek(&File, f_size(&File));
                        f_puts(line[j], &File);
                        f_close(&File);
                }
            }
*/
        }

}
