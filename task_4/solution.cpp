#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <iostream.h>

//--------------------------------------------------------------------------//
/* Key scan codes */
#define ESC_KEY 1
#define SPACE_KEY 57
#define ARROW_DOWN_KEY 80
#define ARROW_UP_KEY 72
#define ARROW_LEFT_KEY 75
#define ARROW_RIGHT_KEY 77

//--------------------------------------------------------------------------//

typedef unsigned char BYTE;
typedef unsigned int  WORD;
typedef unsigned int  UINT;
typedef unsigned long DWORD;
typedef unsigned long LONG;

//--------------------------------------------------------------------------//

struct BITMAPFILEHEADER
{
	UINT  bfType;
	DWORD bfSize;
	UINT  bfReserved1;
	UINT  bfReserved2;
	DWORD bfOffBits;
};

struct BITMAPINFOHEADER
{
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
};

struct RGBQUAD
{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
};

BITMAPFILEHEADER bmfh;
BITMAPINFOHEADER bmih;
RGBQUAD palette[256];
RGBQUAD palette_modified[256];
BYTE *video_memory = (BYTE*)0xA0000000L;
FILE *bitmap_file;

//--------------------------------------------------------------------------//
/* Switch graphic mode to chosen argument */
void set_video_mode(int mode) {
	REGPACK regs;
	regs.r_ax = mode;
	intr(0x10, &regs);
	return;
}

//--------------------------------------------------------------------------//

void set_video_palette_default() {
	outportb(0x03C8, 0);
	for (int i = 0; i < bmih.biClrUsed; i++) {
		outp(0x03C9, (palette[i].rgbRed) * 63 / 255);
		outp(0x03C9, (palette[i].rgbGreen) * 63 / 255);
		outp(0x03C9, (palette[i].rgbBlue) * 63 / 255);
		palette_modified[i].rgbRed = palette[i].rgbRed;
		palette_modified[i].rgbGreen = palette[i].rgbGreen;
		palette_modified[i].rgbBlue = palette[i].rgbBlue;
	}
	return;
}

void set_video_palette_modified() {
	outport(0x03C8, 0);
	for (int j = 0; j < bmih.biClrUsed; j++) {
		outp(0x03C9, palette_modified[j].rgbRed * 63 / 255);
		outp(0x03C9, palette_modified[j].rgbGreen * 63 / 255);
		outp(0x03C9, palette_modified[j].rgbBlue * 63 / 255);
	}
	return;
}

void negate_video_palette() {
	for (int i = 0; i < bmih.biClrUsed; i++) {
		palette_modified[i].rgbRed = ~palette_modified[i].rgbRed;
		palette_modified[i].rgbGreen = ~palette_modified[i].rgbGreen;
		palette_modified[i].rgbBlue = ~palette_modified[i].rgbBlue;
	}
	set_video_palette_modified();
	return;
}

void adjust_video_pallete_brightness(int brightness_offset) {
	for (int i = 0; i < bmih.biClrUsed; i++) {
		if (palette_modified[i].rgbRed+brightness_offset <= 255 && palette_modified[i].rgbRed + brightness_offset >= 0) {
			palette_modified[i].rgbRed += brightness_offset;
		}
		if (palette_modified[i].rgbGreen + brightness_offset <= 255 && palette_modified[i].rgbGreen + brightness_offset >= 0) {
			palette_modified[i].rgbGreen += brightness_offset;
		}
		if (palette_modified[i].rgbBlue + brightness_offset <= 255 && palette_modified[i].rgbBlue + brightness_offset >= 0) {
			palette_modified[i].rgbBlue += brightness_offset;
		}
	}
	set_video_palette_modified();
	return;
}

//--------------------------------------------------------------------------//

void display_image()
{
	for (long i = bmih.biWidth * (bmih.biHeight - 1); i >= 0; i -= bmih.biWidth) {
		fread(&video_memory[i], bmih.biWidth, 1, bitmap_file);
	}
	return;
}

void edit_image() {
	unsigned char key_pressed;
	while (1) {
		key_pressed = inportb(0x60);
		if (key_pressed == ESC_KEY) {
			break;
		}
		switch (key_pressed) {
		case SPACE_KEY:
			negate_video_palette();
			delay(200);
			break;
		case ARROW_DOWN_KEY:
			adjust_video_pallete_brightness(-1);
			delay(20);
			break;
		case ARROW_UP_KEY:
			adjust_video_pallete_brightness(1);
			delay(20);
			break;
		case ARROW_LEFT_KEY:
			set_video_palette_default();
			break;
		}
	}
}

//--------------------------------------------------------------------------//

void load_file(char* file_name) {
	bitmap_file = fopen(file_name, "rb");
	fread(&bmfh, sizeof(bmfh), 1, bitmap_file);
	fread(&bmih, sizeof(bmih), 1, bitmap_file);
	fread(&palette[0], bmih.biClrUsed * sizeof(RGBQUAD), 1, bitmap_file);
	set_video_palette_default();
	display_image();
	fclose(bitmap_file);
	return;
}

//--------------------------------------------------------------------------//

void main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Program requires exactly two arguments." << endl;
		return;
	}
	set_video_mode(0x13);
	load_file(argv[1]);
	edit_image();
	set_video_mode(0x03);
	return;
}