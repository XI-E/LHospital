#include "ui.hpp"

void test_printer();
void test_body();
void test_textbox();
void test_listlayout();

void main()
{
	clrscr();

	ui::init();

	char name[30]; char *n = name;
	int age;
	long phn;
	float amt;
		 
	box b;
	b << "Enter details: " << ui::endl
	  << "Name: "; 
	b.settcolor(YELLOW); b >> n;
	b.settcolor(ui::tcolor); b << "Age: ";
	b.settcolor(YELLOW); b >> age;
	b.settcolor(ui::tcolor); b << "Phone num: ";
	b.settcolor(YELLOW); b >> phn;
	b.settcolor(ui::tcolor); b << "Amount: ";
	b.settcolor(YELLOW); b >> amt;
	b.settcolor(ui::tcolor); b.setexit_button("Submit");
	b.loop();

	b.clear();

	b << "You entered the following data: " << ui::endl
	  << "Name: " << name << ui::endl
	  << "Age: " << age << ui::endl
	  << "Phone num: " << phn << ui::endl
	  << "Amount: " << amt << ui::endl;

	getch();
}

void test_listlayout()
{
	list_layout l;
	l.setpos(coord(2,1));
	l.setheight(6);
	
	interactive *list[10];
	
	//Setting the text boxes
	for(int i = 0; i < 9; i++)
	{
		char s[] = {'A'+i, ':', ' ', '\0'};
		l.settcolor(LIGHTGRAY);
		l << coord(2, i + 1) << s;
		l.settcolor(RED);
		list[i] = l.settext_box(coord(5, i + 1));
	}

	l.settcolor(LIGHTGRAY);
	list[9] = l.setbutton(coord(3, i + 1), "Submit");

	//Rudimentary scrolling
	i = 100;
	int j = 0;

	int lines_scrolled = l.getlines_scrolled(),
		height = l.getheight();

	coord pos_topleft(2,1);
	int y = pos_topleft.y;
	while(i--)
	{
		coord c = list[j]->getpos();
		if(c.y - lines_scrolled > height)
		{
			lines_scrolled = c.y - height;
		}
		else if(c.y - lines_scrolled < y)
		{
			lines_scrolled = c.y - y;
		}

		l.setlines_scrolled(lines_scrolled);
		int response = list[j]->input(-lines_scrolled);

		if(response == interactive::GOTONEXT)
		{
			if(j < 9) j++; else j = 0;
		}
		else if(response == interactive::GOTOPREV)
		{
			if(j > 0) j--; else j = 9;
		}
		else if(response == interactive::CLICKED)
		{
			coord init_pos(wherex(), wherey());
			gotoxy(1, ui::scr_height-1);
			cprintf("%s%d", "Clicked ", i);
			gotoxy(init_pos.x, init_pos.y);
		}
	}
}

void test_textbox()
{
	text_box t;
	t.setpos(coord(1,1));
	for(int i = 0; i < 5; i++)
	{
		int a = t.input();

		int x = wherex(), y = wherey();
		gotoxy(1, ui::scr_height-1);
		if(a == interactive::GOTONEXT)
		{
			cout << "GOTONEXT";
		}
		else if(a == interactive::GOTOPREV)
		{
			cout << "GOTOPREV";
		}
		else
		{
			cout << "UNDEFINED";
		}

		gotoxy(x, y);
	}
}

/*
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

void test_body()
{
	body b;
	b.setwidth(ui::scr_width/2);
	b.setcorner_top_left(coord(5,2));
	b << ui::left
	  << "Well, this is a sentence, you know! \nThis is the next line. Hopefully this will get wrapped up as it has gotten too long \nBye!"
	;
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
*/