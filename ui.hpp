#ifndef UI_HPP
#define UI_HPP
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <stdlib.h>

class manipulator
{
	static int index;
	int own_index;

	public:
		manipulator();
		int operator==(manipulator);
};

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
		static manipulator endl;
};

struct coord
{
	int x;
	int y;

	coord(int = 1,int = 1);
	coord & operator+=(coord);
	coord & operator-=(coord);
	coord operator+(coord);
	coord operator-(coord);
};
/*
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
		void hide(); //Hides printed data
};
*/

class list_layout_node
{
    list_layout_node *next;     //Pointer to next node
    coord pos;                  //Position where to print
    int tcolor;                 //Text colour
    int bcolor;                 //Background colour
    char str[100];              //String to print

    public:
        list_layout_node();     //Ctor
        ~list_layout_node();    //Dtor

        //Setters
        void setnext(list_layout_node *);
        void setpos(coord);
        void settcolor(int);
        void setbcolor(int);
        void setstr(const char *);

        //Getters
        list_layout_node * getnext();
        coord getpos();
        int gettcolor();
        int getbcolor();
        const char * getstr();
};

struct string_node
{
    string_node *next;
    string_node *prev;
    char data;

    string_node();
};

class interactive : public list_layout_node
{
	interactive *prev;    //ptr to previous node
	interactive *next;    //ptr to next node
	int offset;		   	  //offset to y position when printing
	public:
		interactive();
		~interactive();
		virtual int input(int);

		void setoffset(int);
		int getoffset();

		enum
        {
            GOTONEXT,
			GOTOPREV,
			CLICKED
		};
		
		enum keys
		{
			TAB,
			ENTER,
			BACKSPACE,
			SHIFT_TAB,
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		static int getkey();
};

class text_box : public interactive
{
    public:
        int input(int = 0);   //Takes input,stores it
                       //& returns GOTONEXT or
					   //GOTOPREV; parameter is offset
					   //y coordinate
		void print_str(string_node*); //Prints string
};

class button : public interactive
{
	int tcolor_selected; //tcolor when selected
	int bcolor_selected; //bcolor when selected

	public:
		button();
		void settcolor_selected(int);
		void setbcolor_selected(int);
		int gettcolor_selected();
		int getbcolor_selected();

		int input(int = 0); //Returns CLICKED,
							//GOTONEXT or 
							
		void print(int = 0); //Prints the button
						 //pmt indicates if btn is selected
						 //or not
};

class list_layout
{
    list_layout_node *head;
	list_layout_node *current;

	coord corner_top_left; //top left corner of container
	
	//Following are used as temporary
	//placeholders till data is written to the nodes
	coord pos;
	int tcolor;
	int bcolor;
	int tcolor_selected;
	int bcolor_selected;

	//FOR SCROLLING IMPLEMENTATION
	int height; //height of container; to implement scrolling
	int width;
	int lines_scrolled;

	enum print_modes
	{
		DISPLAY, 
		HIDE
	};
	void print(int = DISPLAY);
    public:
		list_layout();
		
		//Set a label
		list_layout& operator<<(coord);
		list_layout& operator<<(const char *);

		//Sets a text box at the position indicated by
		//coord and returns a pointer to it
		interactive * settext_box(coord);

		interactive * setbutton(coord, const char *);
		//								^text the btn displays

		void settcolor(int);
		void setbcolor(int);
		void settcolor_selected(int);
		void setbcolor_selected(int);
		void setcorner_top_left(coord);
		void setheight(int);
		void setwidth(int);
		void setlines_scrolled(int);
		void setpos(coord);

		int getheight();
		int getwidth();
		int getlines_scrolled();
		coord getpos();
		coord getcorner_top_left();
		
		void display();
		void hide();
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

	void print(int = 1); //0 = hide, 1 = display

	public:
		enum visibility_modes
		{
			all = 1,
			nosides = 2
		};

		frame(coord = coord(1,1), int = ui::scr_width, int = ui::scr_height - 1);

		void display();
		void hide();
		
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

class box
{
	coord corner_top_left;
	int height;
	int width;
	int padding;

	int tcolor;
	int bcolor;

	int wrap(char[], int length, int = 0);
	//                           ^It will set first parameter
	//							  to have only one line
	list_layout layout;

	coord pos_pointer;

	public:
		frame f;

		box(int = ui::scr_width, int = ui::scr_height - 1);

		//GETTERS
		coord getcorner_top_left();
		int getheight();
		int getwidth();
		int getpadding();

		//SETTERS
		void setcorner_top_left(coord);
		void setheight(int);
		void setpadding(int);
		void settcolor(int);
		void setbcolor(int);

		box & operator<<(char *);
		box & operator<<(char);
		box & operator<<(int);
		box & operator<<(long);
		box & operator<<(double);
		box & operator<<(manipulator);
};

/*
class body
{
	coord corner_top_left;
	int height;
	int width;
	int padding[2];
	printer p;
	int wrap(char[], int length);
	int tcolor;
	int bcolor;

	public:
		body(int = ui::scr_width, int = ui::scr_height - 1);
		body & operator<<(int); //state
		body & operator<<(char []); //str
		void print();
		void hide();

		void setcorner_top_left(coord);
		void setheight(int);
		void setwidth(int);
		void setpaddingx(int);
		void setpaddingy(int);
		void settcolor(int);
		void setbcolor(int);

		coord getcorner_top_left();
		int getheight();
		int getwidth();
		int getpaddingx();
		int getpaddingy();
		int gettcolor();
		int getbcolor(); 	
};
*/

#endif /* UI_HPP */