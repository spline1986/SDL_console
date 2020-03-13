#include <string>
#include <unicode/unistr.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "console.h"

static void cl_convert(Uint8 *arr, Uint32 color) {
	Uint8 *p = reinterpret_cast<Uint8*>(&color);
	arr[0] = p[2];
	arr[1] = p[1];
	arr[2] = p[0];
}

Console::Console(std::string title, std::string filename, int w, int h) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError()
				  << std::endl;
	} else {
		Console::load_font(filename);
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
								  SDL_WINDOWPOS_UNDEFINED,
								  ch_width * w, ch_height * h, 0);
		if (window == nullptr) {
			std::cout << "Window could not be created! SDL_Error: "
					  << SDL_GetError() << std::endl;
		} else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == nullptr) {
				std::cout << "Renderer could not be created! SDL Error: "
						  << SDL_GetError();
			}
		}
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	set_wide_ascii();
	width = w;
	height = h;
	x = 0;
	y = 0;
}

Console::~Console() {
	SDL_FreeSurface(font);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Console::set_wide_ascii() {
	for (unsigned int i = 0; i < 128; i++)
		chars[i] = i;
	for (unsigned int i = 128; i < 176; i++)
		chars[i] = 912 + i;
	for (unsigned int i = 224; i < 240; i++)
		chars[i] = 864 + i;
	chars[241] = 1025;
	chars[242] = 1105;
}

void Console::load_font(std::string filename) {
	font = IMG_Load(filename.c_str());
	if (font == nullptr) {
		std::cout << "Unable to load image " << filename.c_str()
				  << "! SDL_image Error: " << IMG_GetError();
	} else {
		ch_width = font->w / 16;
		ch_height = font->h / 16;
	}
}

int Console::get_char_index(unsigned int c) {
	for (int i = 0; i < 256; i++)
		if (chars[i] == c)
			return i;
	return -1;
}

void Console::setbg(Uint32 color) {
	cl_convert(bg, color);
}

void Console::setfg(Uint32 color) {
	cl_convert(fg, color);
}

int Console::get_w() {
	return width;
}

int Console::get_h() {
	return height;
}

int Console::get_ch_w() {
	return ch_width;
}

int Console::get_ch_h() {
	return ch_height;
}

void Console::clear() {
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = ch_width * width;
	rect.h = ch_height * height;
	SDL_SetRenderDrawColor(renderer, bg[0], bg[1], bg[2], 0xff);
	SDL_RenderFillRect(renderer, &rect);
	x = 0;
	y = 0;
}

void Console::printxy(int x, int y, std::string text) {
	icu::UnicodeString s = icu::UnicodeString::fromUTF8(text);
	int i = x;
	int j = y;
	for (char n = 0; n < s.length(); n++) {
		int c = get_char_index(s[n]);
		int x1 = (c % 16) * ch_width;
		int y1 = (c / 16) * ch_height;
		for (int x2 = 0; x2 < ch_width; x2++)
			for (int y2 = 0; y2 < ch_height; y2++) {
				Uint8 *pixel = reinterpret_cast<Uint8*>(font->pixels) +
							   (y1 + y2) * font->pitch + (x1 + x2) *
							   font->format->BytesPerPixel;
				SDL_Rect rect;
				rect.x = i * ch_width + x2;
				rect.y = j * ch_height + y2;
				rect.w = 1;
				rect.h = 1;
				if (pixel[0] != 0) {
					SDL_SetRenderDrawColor(renderer, fg[0], fg[1], fg[2], pixel[0]);
					SDL_RenderFillRect(renderer, &rect);
				} else {
					SDL_SetRenderDrawColor(renderer, bg[0], bg[1], bg[2], 0xff);
					SDL_RenderFillRect(renderer, &rect);
				}
			}
		i++;
		if (i > width - 1) {
			i = 0;
			j++;
		}
	}
}

void Console::writexy(int x, int y, std::string text, bool back) {
	icu::UnicodeString s = icu::UnicodeString::fromUTF8(text);
	int i = x;
	for (char n = 0; n < s.length(); n++) {
		int c = get_char_index(s[n]);
		int x1 = (c % 16) * ch_width;
		int y1 = (c / 16) * ch_height;
		for (int x2 = 0; x2 < ch_width; x2++)
			for (int y2 = 0; y2 < ch_height; y2++) {
				Uint8 *pixel = reinterpret_cast<Uint8*>(font->pixels) +
							   (y1 + y2) * font->pitch + (x1 + x2) *
							   font->format->BytesPerPixel;
				SDL_Rect rect;
				rect.x = i + x2;
				rect.y = y + y2;
				rect.w = 1;
				rect.h = 1;
				if (pixel[0] != 0) {
					SDL_SetRenderDrawColor(renderer, fg[0], fg[1], fg[2], pixel[0]);
					SDL_RenderFillRect(renderer, &rect);
				} else if (back) {
					SDL_SetRenderDrawColor(renderer, bg[0], bg[1], bg[2], 0xff);
					SDL_RenderFillRect(renderer, &rect);
				}
			}
		i = i + ch_width;
	}
}

void Console::blit(int x, int y, SDL_Surface *surface) {
	SDL_Texture* texture = nullptr;
	SDL_Rect rect;
	int size[2];
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, nullptr, nullptr, &size[0], &size[1]);
	rect.x = x;
	rect.y = y;
	rect.w = size[0];
	rect.h = size[1];
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Console::flush() {
	SDL_RenderPresent(renderer);
}
