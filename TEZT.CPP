#include <iostream.h>
#include <conio.h>

class ui
{
	ui();	//object of this class shouldn't be created
	public:
		enum dir
		{
			left = 1,
			top = 2,
			right = 4,
			bottom = 8,
			all = 16 //When all sides need to be modified
		};
		static int scr_height;
		static int scr_width;
		static void init();
		static int tcolor;
		static int bcolor;
};

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

class coord
{
	int x;
	int y;

	public:
		coord();
		coord(int, int);

		int getx();
		int gety();

		//An invalid int supplied is ignored
		void setx (int);
		void sety (int);
};

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

class frame
{
	char border_chars[8];
	int color;
	int sides_visibility[8];
	int frame_visibility;
	coord corner_top_left; //top left
	int height;  //height and width includes border
	int width;
	int state;	//Used by << Operator
	void setside_visibility(int, int); //state, side
	int convert(int); //Eg: ios::top | ios::left -> 0
					   //    ios::bottom | ios::left -> 3	

	public:
		enum visibility_modes
		{
			all = 1,
			nosides = 2
		};

		frame(coord = coord(1,1), int = ui::scr_height, int = ui::scr_width);
		void display(int = 1); //1 = display, 0 = hide
		void setvisibility_mode(int);
		frame & operator<<(int); //Sets state
		frame & operator<<(char); //Sets border_char according
								//to state

		int getheight();
		int getwidth();
		coord getcorner_top_left();
		int getframe_visibility(); //Returns 1 if visible; 0 = not visible
		int getcolor();
		char getborder_char(int);
		int getside_visibility(int);

		void setheight(int);
		void setwidth(int);
		void setcolor(int);
		void setcorner_top_left(coord);
};

int frame::convert(int param)
{
	if(param & ui::top)
	{
		if(param & ui::left)
		{
			return 0;
		}
		else if(param & ui::right)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if(param & ui::bottom)
	{
		if(param & ui::left)
		{
			return 3;
		}
		else if(param & ui::right)
		{
			return 4;
		}
		else
		{
			return 5;
		}
	}
	else if(param & ui::left)
	{
		return 6;
	}
	else if(param & ui::right)
	{
		return 7;
	}

	return -1;
}

void frame::setside_visibility(int side, int visib)
{
	if( visib != 0 && visib != 1)
		return;		//No effect for invalid visibility

	if(side & ui::all)
	{
		for(int i = 0; i < 8; i++)
			sides_visibility[i] = visib;
		return;
	}

	int a = frame::convert(side);
	if(a == -1) return; //-1 indicates invalid side

	sides_visibility[a] = visib;
}

int frame::getside_visibility(int side)
{
	int a = convert(side);

	if(a == -1) return -1; //Wrong side selected

	return sides_visibility[a];
}


frame::frame(coord topleft, int h, int w)
{
	for(int i = 0; i < 8; i++)
	{
		border_chars[i] = '*';
		sides_visibility[i] = 1;
	}
	color = LIGHTGRAY;
	frame_visibility = 0;
	height = h;
	width = w;
	state = 0;
	corner_top_left = topleft;
}

void frame::display(int param)
{
	textcolor(frame::color);

	coord &c = corner_top_left;
	int x = c.getx(),
		y = c.gety();

	char visible_chars[8];
	frame_visibility = param;

	int arr[] = {
				ui::top,
				ui::bottom,
				ui::left,
				ui::right,
				ui::top | ui::left,
				ui::top | ui::right,
				ui::bottom | ui::left,
				ui::bottom | ui::right
			};

	char &top = visible_chars[0],
		 &bottom = visible_chars[1],
		 &left = visible_chars[2],
		 &right = visible_chars[3],
		 &top_left = visible_chars[4],
		 &top_right = visible_chars[5],
		 &bottom_left = visible_chars[6],
		 &bottom_right = visible_chars[7];

	for(int i = 0; i < 8; i++)
	{
		if(param == 1 && getside_visibility(arr[i]))
		{
			visible_chars[i] = getborder_char(arr[i]);
		}
		else
		{
			visible_chars[i] = ' ';
		}
	}

	gotoxy(x,y);

	cprintf("%c", top_left);

	for(i = 1; i < width - 1; i++)
	{
		cprintf("%c", top);
	}
	cprintf("%c", top_right);

	for(i = 1; i < height - 2; i++)
	{
		gotoxy(x, y + i); cprintf("%c", left);
		gotoxy(x + width - 1, y + i); cprintf("%c", right);
	}

	gotoxy(x, y + height - 2);
	cprintf("%c", bottom_left);
	for(i = 1; i < width - 1; i++)
	{
		cprintf("%c", bottom);
	}
	cprintf("%c", bottom_right);

	gotoxy(x, y);
	textcolor(ui::tcolor);
}

void frame::setvisibility_mode(int param)
{
	frame::setside_visibility(frame::all, 1);
	if(param & nosides)
	{
		frame::setside_visibility(ui::left, 0);
		frame::setside_visibility(ui::right, 0);
	}
	frame::display();
}

//Operator << is used to set border char
frame & frame::operator<<(int side)
{
	int a = frame::convert(side);

	if(a == -1)	return *this; //-1 indicates error

	state = a;

	return *this;
}

frame & frame::operator<<(char border_char)
{
	border_chars[frame::state] = border_char;
	return *this;
}

int frame::getheight()
{
	return height;
}

int frame::getwidth()
{
	return width;
}

coord frame::getcorner_top_left()
{
	return frame::corner_top_left;
}

int frame::getframe_visibility()
{
	return frame_visibility;
}

int frame::getcolor()
{
	return color;
}

char frame::getborder_char(int side)
{
	int a = convert(side);

	if(a == -1)	return '\0'; //Error

	return frame::border_chars[a];
}

void frame::setheight(int h)
{
	if(h > ui::scr_height) return;

	display(0);
	frame::height = h;
	display();
}

void frame::setwidth(int w)
{
	if(w > ui::scr_width) return;

	display(0);
	frame::width = w;
	display();
}

void frame::setcolor(int c)
{
	color = c;
	display();
}

void frame::setcorner_top_left(coord c)
{
	display(0);
	frame::corner_top_left = c;
	display(1);
}

void main()
{
	clrscr();

	ui::init();
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
	getch();
}