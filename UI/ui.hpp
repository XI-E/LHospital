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

//! For running ui::init() before main (initialising basic stuff)
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

//! Represents the node of a list representing the layout
/*!
 Represents all the information of an element that will be
 printed on the screen. Also points to the next element of the
 screen that will be printed next to it
*/
class list_layout_node
{
    list_layout_node *next;     //!< Pointer to next node
    coord pos;                  //!< Position where to print
    int tcolor;                 //!< Text colour
    int bcolor;                 //!< Background colour
	char str[100];              //!< String to print

	//! How to print the string; mainly for passwords
	int print_type;				

    public:
        list_layout_node();     //!< Ctor
        ~list_layout_node();    //!< Dtor

        //@{ Setter functions
        void setnext(list_layout_node *);
        void setpos(coord);
        void settcolor(int);
        void setbcolor(int);
		void setstr(const char *);
		void setprint_type(int);
		//@}

        //@{ Getter functions
        list_layout_node * getnext();
        coord getpos();
        int gettcolor();
        int getbcolor();
		const char * getstr();
		int getprint_type();
		//@}
		
		//! Used to distinguish will be printed i.e.
		//! as is or hidden (as passwords)
		enum print_types
		{
			DEFAULT,
			PASSWORD
		};
};

//! A node of the representation of string as a linked list
struct string_node
{
    string_node *next;	//!< Pointer to next node
    string_node *prev;	//!< Pointer to previous node
    char data;			//!< Character stored in string

    string_node();		//!< Ctor
};

//! Represents all interactive information
/*!
 Basically a parent class of all the classes that
 represent the elements of the layout the user can
 interact with.
 Used so that all those elements can be clubbed together
 and the input be taken.
*/
class interactive : public list_layout_node
{
	interactive *prev;    	//!< ptr to previous node
	interactive *next;    	//!< ptr to next node
	int offset;		   	  	//!< offset to y position when printing
	public:
		interactive();		//!< Ctor
		~interactive();		//!< Dtor

		//! Empty input function that will be overridden by children
		/*!
		 \param offset The offset to y position
		 \return Action that was performed by the user
		*/
		virtual int input(int offset);

		//! Setter function
		void setoffset(int);

		//! Getter function
		int getoffset();

		//! Actions that are performed by user; returned from input func.
		enum actions
        {
            GOTONEXT,
			GOTOPREV,
			CLICKED
		};
		
		//! Keys that user can press to navigate the form
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

		//! Gets key from user and returns code
		/*
		 \return Keyname corresponding to enum keys
		*/
		static int getkey();
};

//! Represents a text box
/*!
 Inherits from interactive as a text box can be interacted
 with. Gets data from user and stores it as a string that
 can be further converted to the required data type
*/
class text_box : public interactive
{
	//! Represents if the data entered in the text box
	//! should be displayed as is or replaced with asterisks
	int is_password;

	public:
		text_box(); //!< Ctor

		//! Takes input and returns user action
		/*!
		 /param offset Offset of y coordinate to print
		 /return Action performed by user
		*/
        int input(int offset = 0);

		//! Prints string represented by a linked list
		/*
		 Takes in the head pointer of the linked list
		 string and prints the string by iterating through
		 the list. Has no other side effects.
		 /param head ptr to head of the linked list
		*/
		void print_str(string_node *head);

		//! Setter function
		void setis_password(int);
};

//! Represents a button that can be clicked
/*!
 Inherits from interactive as a button can be interacted with.
 A user can click the button while it's input function is
 running which will return the user action
*/
class button : public interactive
{
	int tcolor_selected; //!< tcolor when selected
	int bcolor_selected; //!< bcolor when seilected

	public:
		button(); //!< Ctor

		//@{ Setter functions
		void settcolor_selected(int);
		void setbcolor_selected(int);
		//@}

		//@{ Getter functions
		int gettcolor_selected();
		int getbcolor_selected();
		//@}

		//! Input function
		/*!
		 Effectively allows the button to be clicked
		 /param offset Offset of y coordinate to print
		 /return Action performed by the user
		*/
		int input(int offset = 0);

		//! Prints the button
		/*!
		 /param isselected Indicates if button is selected or not
		*/						
		void print(int isselected = 0);
};

//! Represents the layout of the page
/*!
 Incorporates elements like simple nodes as well as other
 interactive elements. This layout can be contained within
 a specific height and the overflowing content can reached
 by scrolling which is also implemented here.
*/
class list_layout
{
	//@{ Pointers to implement a linked list to elements
    list_layout_node *head;	//!< ptr to head node
	list_layout_node *current; //!< ptr to current node
	//@}

	coord corner_top_left; //!< top left corner of container
	
	/*!
     Following are used as temporary placeholders till data
	 is written to the nodes
	*/
	///@{
	coord pos;
	int tcolor;
	int bcolor;
	int tcolor_selected;
	int bcolor_selected;
	int tcolor_input;
	int bcolor_input;
	///@}

	//@{ For scrolling implementation
	int height; //!< Height of the layout
	int width; //!< Width of the layout
	int lines_scrolled; //!< Lines currently scrolled
	//@}

	//! For better verbosity at internal level
	enum print_modes
	{
		DISPLAY, 
		HIDE
	};

	//! Prints the layout
	/*!
	 Prints the layout by iterating through the internal
	 linked list maintained. Has no other side effects
	 /param print_mode How to print the data
	*/
	void print(int print_mode = DISPLAY);
    public:
		list_layout(); //!< Ctor
		
		//@{ Set an element (node)
		list_layout& operator<<(coord); //!< Set coord of node

		//! Set data held by the node
		list_layout& operator<<(const char *);
		//@}

		//! Set a text box
		/*!
		 Sets a text box at the position indicated by pos and
		 returns a pointer to it
		 /param pos Position at which to set text box
		 /param is_pass If the text box has a password, set to 1
		 /return pointer to the text box set (casted to interactive *)
		*/
		interactive * settext_box(coord pos, int is_pass = 0);

		//! Set a button
		/*!
		 Sets a button at the position indicated by pos and 
		 returns a pointer to it
		 /param pos Position at which to set the button
		 /param txt The text the button displays
		*/
		interactive * setbutton(coord pos, const char *txt);

		//@{ Setter functions
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
		//@}

		//@{ Getter functions
		int getheight();
		int getwidth();
		int getlines_scrolled();
		coord getpos();
		coord getcorner_top_left();
		//@}
		
		void display();	//!< Display the layout
		void hide(); //!< Hide the layout
		void clear(); //!< Deletes contents of the layout
};

//! Represents a border

class frame
{
	char border_chars[8];
	int tcolor;
	int bcolor;
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
		int gettcolor();
		int getbcolor();
		char getborder_char(int);
		int getside_visibility(int);

		void setheight(int);
		void setwidth(int);
		void settcolor(int);
		void setbcolor(int);
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
	int bcolor;
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
