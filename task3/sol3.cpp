#include <dos.h>
#include <conio.h>

// Global time variables
volatile unsigned char g_hour = 0;
volatile unsigned char g_minute = 0;
volatile unsigned char g_second = 0;
volatile unsigned char g_msecond = 0;

// Millisecond display mode flag
volatile int enchancedm = 0;

// Pointers to original interrupt vectors to restore them on exit
void interrupt (*old_int8)(...);
void interrupt (*old_int9)(...);

// Helper function to write a 2-digit number to video memory
void print_number(int number, int offset) {
    // Text mode video memory base address (0xB800:0000)
    char far *vram = (char far *)MK_FP(0xB800, 0x0000);
    
    vram[offset] = (number / 10) + '0';       // Tens digit (ASCII)
    vram[offset + 1] = 0x07;                  // Attribute (light grey text on black background)
    vram[offset + 2] = (number % 10) + '0';   // Units digit (ASCII)
    vram[offset + 3] = 0x07;                  // Attribute (light grey text on black background)
}

// Custom keyboard interrupt handler (0x09)
void interrupt new_int9(...) {
    // Read scan code from the keyboard port (0x60)
    unsigned char scan_code = inportb(0x60);
    
    // F12 scan code is 0x58 
    if (scan_code == 0x58) {
        enchancedm = !enchancedm; // Toggle millisecond display mode
    }
    
    // Call the original interrupt handler
    old_int9();
}

// Custom timer interrupt handler (0x08)
void interrupt new_int8(...) {
    int off = 2000; // Offset for the middle of the screen
    char far *vram = (char far *)MK_FP(0xB800, 0x0000); // Access video memory
    
    print_number(g_hour, off);
    vram[off + 4] = ':'; vram[off + 5] = 0x07;
    
    print_number(g_minute, off + 6);
    vram[off + 10] = ':'; vram[off + 11] = 0x07;
    
    print_number(g_second, off + 12);
    
    if (enchancedm) {
        vram[off + 16] = ':'; vram[off + 17] = 0x07;
        print_number(g_msecond, off + 18);
    } else {
        // Hide milliseconds by overwriting them with spaces
        vram[off + 16] = ' '; vram[off + 17] = 0x07;
        vram[off + 18] = ' '; vram[off + 19] = 0x07;
        vram[off + 20] = ' '; vram[off + 21] = 0x07;
    }
    
    // Call the original timer interrupt handler
    old_int8();
}

int main() {
    struct dostime_t time;
    
    // 1. Save original interrupt vectors
    old_int8 = getvect(0x08);
    old_int9 = getvect(0x09);
    
    // 2. Install custom interrupt handlers
    setvect(0x08, new_int8);
    setvect(0x09, new_int9);

    clrscr();
    
    // 3. Main programme loop
    while (1) {
        // Safely retrieve current time using DOS function
        _dos_gettime(&time);
        
        // Update global variables
        g_hour = time.hour;
        g_minute = time.minute;
        g_second = time.second;
        g_msecond = time.hsecond; 
        

        unsigned char key_pressed = inportb(0x60); // Read scan code from keyboard port
		if(key_pressed == 0x01){ // ESC key scan code is 0x01
			break;
		}

    }
    
    // 4. Cleanup: Restore original interrupt vectors
    setvect(0x08, old_int8);
    setvect(0x09, old_int9);
    clrscr();
    return 0;
}