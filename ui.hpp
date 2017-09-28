#ifndef UI_HPP
#define UI_HPP

#include <conio.h>
#include <stdarg.h>
#include <string.h>

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
		enum align
		{
			leftalign = 1,
			centeralign = 2,
			rightalign = 4
		};
		static int scr_height;
		static int scr_width;
		static void init();
		static int tcolor;
		static int bcolor;
};

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

class node_printer
{
	node_printer *next;
	coord pos;	//Where to place this chunk
	char str[80];
	int tcolor;
	int bcolor;

	public:
		node_printer(char []  = ""); //Param will be assigned as str
		~node_printer();   //Dtor

		void setnext(node_printer *);
		void setpos(coord);
		void setstr(char []);
		void settcolor(int);
		void setbcolor(int);

		node_printer * getnext();
		coord getpos();
		char * getstr();
		int gettcolor();
		int getbcolor();

		void print();
};

class printer
{
	node_printer *head;
	node_printer *current;

	//Height and width of area in which it will print
	int height;
	int width;
	coord corner_top_left; 

	int state; //For internal purposes

	public:
		printer(int = ui::scr_width, int = ui::scr_height - 1);
		~printer();

		void settcolor(int); //TODO
		void setbcolor(int);
		void setcorner_top_left(coord);
		void setheight(int);
		void setwidth(int);

		int gettcolor();
		int getbcolor();
		coord getcorner_top_left();
		int getheight();
		int getwidth();

		printer & operator<<(int); //Sets state
		
		//Will add param as str of the node current points to
		printer & operator<<(char []);

		void print();
		void gotobegin(); //Sets printer to print from beginning
};

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

void test_frame();

#endif