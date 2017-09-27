#include "ui.hpp"

int ui::scr_height = 0,
	ui::scr_width = 0,
	ui::tcolor = LIGHTGRAY,
	ui::bcolor = BLACK;

void ui::init()
{
	_setcursortype(_NOCURSOR); //hides cursor
	textcolor(ui::tcolor);
	textbackground(ui::bcolor);

	struct text_info info;
	gettextinfo(&info);

	//height and width of screen
	scr_width = (int) info.screenwidth;
	scr_height = (int) info.screenheight;
}

coord::coord()
{
	x = y = 1;
}

coord::coord(int a, int b)
{
	x = a;
	y = b;
}

int coord::getx()
{
	return x;
}

int coord::gety()
{
	return y;
}

void coord::setx(int a)
{
	if(a < 0 || a > ui::scr_width)
		return;

	x = a;
}

void coord::sety(int a)
{
	if(a < 0 || a > ui::scr_height)
		return;

	y = a;
}