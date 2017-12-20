/*!
 \file ui.hpp
 \brief Contains prototypes of UI functions
*/

#ifndef UI_HPP
#define UI_HPP

#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

//! Validator function that's used for validating user input
typedef int (*validator_f)(const char *);

//! For running ui::init() before main
class init_lib_ui
{
	static int counter; //!< Ensures ui::init() is called only once
	public:
		init_lib_ui(); //!< Ctor
};

//! Static object of type init_lib_ui that is initialised
//! before main is run and thus, ui::init is called
static init_lib_ui init_obj_ui;

//! Manipulator class to manipulate UI functions
/*!
 Objects of this type would be used instead of an enum
 to avoid conflicts with int
 Every manipulator object is identified by its index while
 static index indicates the index to be assigned to the next
 manipulator
*/
class manipulator
{
	static int index; //!< index of a new manipulator object
	int own_index;	  //!< index of current manipulator

	public:
		manipulator(); //!< Ctor; assigns index
		int operator==(manipulator); //!< Returns 1 if indexes are same
};

//! Class containing basic UI functions and attributes
class ui
{
	ui();	//!< Private ctor; object of this class shouldn't be created
	public:
	
		//! Specifies the directions for modifying frame, etc.
		enum dir
		{
			left = 1,
			top = 2,
			right = 4,
			bottom = 8,
			all = 16 //!< When all sides need to be modified
		};
		static int scr_height; //!< Height of screen
		static int scr_width; //!< Width of screen
		static void init(); //!< Sets all static variables
		static void clrscr(); //!< Clears the contents off the screen
		static int tcolor; //!< text color
		static int bcolor; //!< background color
		static manipulator endl; //!< End line and move cursor to next line
		static manipulator centeralign; //!< Center align
		static manipulator rightalign; //!< Right align
};

//! Represents a coordinate
struct coord
{
	int x;	//!< x coordinate
	int y;	//!< y coordinate

	coord(int = 1,int = 1); //!< Sets the coordinate
	coord & operator+=(coord);
	coord & operator-=(coord);
	coord operator+(coord); 
	coord operator-(coord);
};


class list_layout_node
{
    list_layout_node *next;     //Pointer to next node
    coord pos;                  //Position where to print
    int tcolor;                 //Text colour
    int bcolor;                 //Background colour
	char str[100];              //String to print
	int print_type;				//How to print the string
								//Mainly for passwords

    public:
        list_layout_node();     //Ctor
        ~list_layout_node();    //Dtor

        //Setters
        void setnext(list_layout_node *);
        void setpos(coord);
        void settcolor(int);
        void setbcolor(int);
		void setstr(const char *);
		void setprint_type(int);

        //Getters
        list_layout_node * getnext();
        coord getpos();
        int gettcolor();
        int getbcolor();
		const char * getstr();
		int getprint_type();
		
		enum print_types
		{
			DEFAULT,
			PASSWORD
		};
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
			HOME,
			END,
			DELETE,
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		static int getkey();
};

class text_box : public interactive
{
	int is_password;
	public:
		text_box();
        int input(int = 0);   //Takes input,stores it
                       //& returns GOTONEXT or
					   //GOTOPREV; parameter is offset
					   //y coordinate
		void print_str(string_node*); //Prints string
		void setis_password(int);
};

class button : public interactive
{
	int tcolor_selected; //tcolor when selected
	int bcolor_selected; //bcolor when seilected

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
	int tcolor_input;
	int bcolor_input;

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
		interactive * settext_box(coord, int = 0);
		//								 ^is_password

		interactive * setbutton(coord, const char *);
		//								^text the btn displays

		void settcolor(int);
		void setbcolor(int);
		void settcolor_selected(int);
		void setbcolor_selected(int);
		void settcolor_input(int);
		void setbcolor_input(int);
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
		void clear();
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

/*
* Stores information related to a text box
* Such as what type to convert it's data to
* and where to store it
*/
struct info_tbox
{
	text_box * tbox;
	int type;
	void * data_store;
	int (*validator)(const char *);

	enum data_types
	{					//Stored as (void * is actually)
		INT, 			//int *
		LONG,			//long *
		UNSIGNED_LONG,  //unsigned long *
		STRING,			//char *
		CHAR,			//char *
		DOUBLE,			//double *
		FLOAT,			//float *
		PASSWORD,		//char *
		OTHER //Not supported at the moment
	};

	info_tbox();
	int setdata(); //Returns 1 on success; 0 on failure

};

/*
* Contains default validation functions of type
* int f(char *)
* that take in a string and return 1 if the string
* is valid and 0, otherwise
*/
class validation
{
	validation(); //Object of this class is not allowed
	public:	
		static int vint(const char *);
		static int vlong(const char *);
		static int vunsigned_long(const char *);
		static int vstring(const char *);
		static int vchar(const char *);
		static int vdouble(const char *);
		static int vfloat(const char *);

		static validator_f getvalidator(int, validator_f);
};

/*
* Represents a line with the three strings depiciting
* left, middle and right aligned stuff respectively
*/
struct line
{
	char left[100];
	char middle[100];
	char right[100];

	int width;
	int tcolor;
	coord corner_top_left;

	line();
	void display();
	void hide();
	void clear();

	private:
		void print(int);
};

class box
{
	int height;
	int width;
	int padding;

	int wrap(char[], int, int = 0);
	//                    ^It will set first parameter
	//					  to have only one line
	void set_tbox(int, void *);

	//List of text boxes and buttons
	interactive * list_interactive[30]; 
	button * exit_btn; //Clicking this button exits
					   //the loop
	info_tbox list_tbox[30];
	int index_interactive;
	int index_tbox;	

	int center_toggle;
	int default_toggle;
	int right_toggle;
	int header_toggle;
	int footer_toggle;
	int password_toggle;
	char default_text[100];
	int (*temp_validator)(const char *); //A function pointer

	line header;
	line footer;

	protected:
		//Pos of the pointer in the box
		coord pos_pointer;
		list_layout layout;
		coord corner_top_left;

	public:
		static manipulator setheader;
		static manipulator setfooter;
		static manipulator setpassword;

		frame f;

		box(coord = coord(1,1), int = ui::scr_width,
			 int = ui::scr_height - 1);

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
		void settcolor_selected(int);
		void setbcolor_selected(int);
		void settcolor_input(int);
		void setbcolor_input(int);

		box & operator<<(char *);
		box & operator<<(char);
		box & operator<<(int);
		box & operator<<(long);
		box & operator<<(unsigned long);
		box & operator<<(double);
		box & operator<<(float);
		box & operator<<(manipulator);

		box & operator>>(char *&);
		box & operator>>(char &);
		box & operator>>(int &);
		box & operator>>(long &);
		box & operator>>(unsigned long &);
		box & operator>>(double &);
		box & operator>>(float &);
		box & operator>>(manipulator);
		box & operator>>(int (*)(const char *));
		//Function pointer that takes in a char* and outputs int
		//Basically supposed to be a validation function for
		//the input field

		void setexit_button(char *);

		//Sets default for the next text box and
		//clears it after the next text box has been
		//set
		void setdefault(char *);
		void setdefault(char);
		void setdefault(int);
		void setdefault(long);
		void setdefault(unsigned long);
		void setdefault(double);
		void setdefault(float);

		//Sets the box to loop, effectively enabling
		//all the text boxes and buttons. Also enables
		//scrolling
		void loop();

		void display();
		void hide();
		void clear();

		void setheader_tcolor(int);
		void setfooter_tcolor(int);
		void clear_header();
		void clear_footer();
};

#endif /* UI_HPP */
