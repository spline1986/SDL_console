#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const Uint32 cl_black = 0x000000;
const Uint32 cl_red = 0xa80000;
const Uint32 cl_green = 0x00a800;
const Uint32 cl_brown = 0xa85400;
const Uint32 cl_blue = 0x0000a8;
const Uint32 cl_magenta = 0xb007bf;
const Uint32 cl_cyan = 0x00a8a8;
const Uint32 cl_lgrey = 0xa8a8a8;
const Uint32 cl_lgray = cl_lgrey;
const Uint32 cl_grey = 0x545454;
const Uint32 cl_gray = cl_grey;
const Uint32 cl_lred = 0xfc5454;
const Uint32 cl_lgreen = 0x54fc54;
const Uint32 cl_yellow = 0xfcfc54;
const Uint32 cl_lblue = 0x5454fc;
const Uint32 cl_lmagenta = 0xfc54fc;
const Uint32 cl_lcyan = 0x54fcfc;
const Uint32 cl_white = 0xfcfcfc;

class Console {
public:
	Console(std::string title, std::string filename, int w, int h);
	~Console();
	void set_wide_ascii();
	void setfg(Uint32 color);
	void setbg(Uint32 color);
	int get_w();
	int get_h();
	int get_ch_w();
	int get_ch_h();
	void clear();
	void printxy(int x, int y, std::string text);
	void writexy(int x, int y, std::string text, bool back);
	void blit(int x, int y, SDL_Surface *surface);
	void flush();
private:
	int x, y;
	int fps = 30;
	int ch_width, ch_height;
	int width, height;
	unsigned int chars[256];
	Uint8 bg[3];
	Uint8 fg[3];
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	SDL_Surface *font = nullptr;
	void load_font(std::string filename);
	int get_char_index(unsigned int c);
};
