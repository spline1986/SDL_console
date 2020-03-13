#include <iostream>

#include "console.h"

using namespace std;

int main() {
	Console *con = new Console("Console Demo", "data/font.png", 80, 25);
	bool quit = false;
	SDL_Event e;
	SDL_Surface *logo = IMG_Load("data/spline.png");
	int x = 0, y = 0;
	int dx = 2, dy = 2;
	while(!quit) {
		con->setbg(cl_grey);
		con->setfg(cl_yellow);
		con->clear();
		con->printxy((con->get_w() - 18) / 2, con->get_h() / 2, "Console write test");
		con->setfg(cl_cyan);
		con->writexy(con->get_w() * con->get_ch_w() - x - 118, con->get_h() * con->get_ch_h() - y - 33,
					 "Graphical write test");
		con->blit(x, y, logo);
		con->flush();
		x = x + dx;
		y = y + dy;
		if (x < 1 || x > con->get_w() * con->get_ch_w() - 118)
			dx = dx * -1;
		if (y < 1 || y > con->get_h() * con->get_ch_h() - 34)
			dy = dy * -1;
		if (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT)
				quit = true;
		SDL_Delay(1000 / 30);
	}
	delete con;
	return 0;
}
