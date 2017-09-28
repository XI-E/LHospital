#include "ui.hpp"

void test_frame();
void test_printer();

void main()
{
	clrscr();

	ui::init();

	test_printer();
	
	getch();
}

void test_frame()
{
	frame f;
	f.display();

	getch();

	f << ui::top << 't'
	  << ui::left << 'l'
	  << ui::bottom << 'b'
	  << ui::right << 'r';

	f.setcolor(LIGHTBLUE);

	f.display();

	getch();

	f << (ui::top | ui::left) << (char) 201
	  << (ui::bottom | ui::left) << (char) 200
	  << (ui::top | ui::right) << (char) 187
	  << (ui::bottom | ui::right) << (char) 188
	  << ui::top << (char) 205
	  << ui::bottom << (char) 205
	  << ui::left << (char) 186
	  << ui::right << (char) 186;

	f.setcolor(ui::tcolor);

	f.display();

	getch();

	f.setheight(ui::scr_height/2);
	getch();

	f.setwidth(ui::scr_width/3);
	getch();

	f.setcorner_top_left(coord( (ui::scr_width-f.getwidth()) / 2, (ui::scr_height-f.getheight()) / 2));
	getch();

	f.setvisibility_mode(frame::nosides);
}

void test_printer()
{
	printer p;
	p << ui::centeralign << "Heading is here\n"
	  << ui::leftalign << "LEFT align"
	  << ui::rightalign << "RIGHT align";

	p.settcolor(BLACK);
	p.setbcolor(CYAN);

	p << ui::centeralign << "WAAH!";

	clrscr();
	gotoxy(1,1);
	p.setcorner_top_left(coord(10,5));
	p.setheight(ui::scr_height/4);
	p.setwidth(ui::scr_width/2);
	p.gotobegin();
	p.print();	

	getch();
}