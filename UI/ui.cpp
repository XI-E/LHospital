#include "ui.hpp"

int manipulator::index = 0;

manipulator::manipulator()
{
	own_index = index;
	index++;
}

int manipulator::operator==(manipulator m)
{
	return own_index == m.own_index;
}

int ui::scr_height = 0,
	ui::scr_width = 0,
	ui::tcolor = LIGHTGRAY,
	ui::bcolor = BLACK;
manipulator ui::endl;

void ui::init()
{
	textcolor(ui::tcolor);
	textbackground(ui::bcolor);

	struct text_info info;
	gettextinfo(&info);

	//height and width of screen
	scr_width = (int) info.screenwidth;
	scr_height = (int) info.screenheight;
}

coord::coord(int X, int Y)
{
	x = X;
	y = Y;
}

coord & coord::operator+=(coord b)
{
	x += b.x;
	y += b.y;

	return *this;
}

coord & coord::operator-=(coord b)
{
	x -= b.x;
	y -= b.y;

	return *this;
}

coord coord::operator+(coord b)
{
	return coord(*this) += b ;
}

coord coord::operator-(coord b)
{
	return coord(*this) -= b;
}