#include <dos.h>
#include <conio.h>

/* 
	// --- Visual helper for adresses in vram --- //
	char0____ char2____      char79___
	0000 0001 0002 0003 .... 0158 0159 | Line 0
	0160 0161 0162 0163 .... 0318 0319 | Line 1
	0320 0321 0322 0323 .... 0478 0479 | Line 2
	....
	3840 3841 3842 3843 .... 3998 3999 | Line 24
	// --- Helper for coding characters in text mode --- //
	2n: 1st byte -> character code (ASCII)
	2n+1: 2nd byte -> 4 bits of background color, 4 bits of foreground colour
	// Single colour is coded in 4 bits, giving total of 16 colours to choose from.
	// Thus to code full byte, we can use values of 0-15 for both colors
	// and shift the background color value by 4 bytes before doing OR operation.
*/

#define ESC_KEY 1
#define ARROW_LEFT_KEY 75
#define ARROW_RIGHT_KEY 77
#define ARROW_UP_KEY 72
#define ARROW_DOWN_KEY 80
#define PLUS_KEY 13
#define MINUS_KEY 12

unsigned char *video_memory = (unsigned char *)0xB8000000L;

int main()
{	
	// should be EVEN number only, as each displayed char in text mode is 2 bytes. 2000th byte is exact middle.
	int char_offset = 2000;
	
	unsigned char key_pressed = 0;
	
	// Set initial character and it's colour scheme.
	char current_char = 'P';
	unsigned char foreground_color = 0xE;
	unsigned char background_color = 0x0;
	
	clrscr();
	_setcursortype(_NOCURSOR);

	while(1){
		// Puts the desired character into video memeory for display.
		video_memory[char_offset] = current_char;
		// Sets the font and background color of desired character.
		video_memory[char_offset+1] = (background_color << 4) | foreground_color;
		
		delay(100);
		key_pressed = inportb(0x60);
		
		if(key_pressed == ESC_KEY){
			break;
		}
		switch(key_pressed){
			case ARROW_LEFT_KEY:
				if(char_offset%160 != 0){
					char_offset = char_offset - 2;
				}
				break;
			case ARROW_RIGHT_KEY:
				if(char_offset%160 != 158){
					char_offset = char_offset + 2;
				}
				break;
			case ARROW_UP_KEY:
				if(char_offset > 158){
					char_offset = char_offset - 160;
				}
				break;
			case ARROW_DOWN_KEY:
				if(char_offset < 3840){
					char_offset = char_offset + 160;
				}
				break;
			case PLUS_KEY:
				if(foreground_color < 15){
					foreground_color++;
				}
				else{
					foreground_color = 0;
				}
				break;
			case MINUS_KEY:
				if(background_color < 15){
					background_color++;
				}
				else{
					background_color = 0;
				}
				break;
		}
		clrscr();
	}
	clrscr();
	_setcursortype(_NORMALCURSOR);
	return 0;
}