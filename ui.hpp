#ifndef UI_HPP
#define UI_HPP

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

#endif