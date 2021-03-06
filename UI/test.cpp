//No need to use ui::init() explicitly

#include "ui/ui.hpp"
#include "ui/test.hpp"

void test_weird_error()
{
	int shit = 14;
	box menu2(coord(2, 4), 40, 10 );
	menu2 << "Enter your shit: ";
	menu2 >> shit;
	menu2.setexit_button("Submit my shit");
	menu2.loop();

	menu2.clear();
	menu2 << "Your shit's coming up!" << ui::endl; getch();
	menu2 << "Here's your shit: ";
	menu2 << shit;
	menu2 << ". Deal with it!" << ui::endl;

	getch();
}

int exit_func()
{
	char c = getch();
	int x = wherex(), y = wherey();

	gotoxy(1, ui::scr_height - 1);
	if(c != '1')
	{
		cprintf("Returning 0"); getch();
		gotoxy(x, y);
		return 0;
	}
	else
	{
		cprintf("Returning 1"); getch();
		gotoxy(x, y);
		return 1;
	}
}

void test_back()
{
	box window;

	int a, b;
	window << "Here's some sample text" << ui::endl;
	window << "Enter some fake data I don't care about" << ui::endl;
	
	window << "Fake #1: "; window >> a;
	window << "Fake #2: "; window >> b;
	window.setexit_button("A fake button");

	window.setback_func(exit_func);

	window.loop();
}

void test_all()
{
	ui::clrscr();
	box menu2(coord(2, 4), 40, 10 );

	menu2.settcolor(GREEN);
	menu2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	menu2.settcolor(WHITE);
	int menu2_height;
	menu2_height = 10;
//	menu2.setheight(menu2_height);
	menu2 << "View employee data" << ui::endl;
	menu2.settcolor(ui::tcolor);
//	menu2 << "Enter employee's id: ";
	unsigned long id;
	menu2 >> id;
	menu2 << ui::endl;
	menu2.setexit_button("Submit");
	menu2.loop();
	
	menu2.clear();
	menu2.setheight(15);
	menu2.settcolor(GREEN);
	menu2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	menu2.settcolor(WHITE);
	menu2 << "Employee Details: " << ui::endl;
	menu2.settcolor(ui::tcolor);
		getch();
		menu2.hide();
		getch();
		menu2.display();
		getch();
	menu2 << "ID: " << 1 << ui::endl;
		getch();
		menu2.hide();
		getch();
		menu2.display();
		getch();

	char name[40], pwd[40];
	int age;
	long phn;
	float amt;
	char date[30];

	box window;
	window.settcolor(CYAN);
	window << ui::centeralign << "LHOSPITAL";
	window << ui::endl << ui::endl;
	window.settcolor(ui::tcolor);
	window.setfooter_tcolor(GREEN);

	window << box::setheader << "28/10/2017"
		   << box::setheader << ui::rightalign << "11:45 PM"
		   << box::setfooter << ui::centeralign
		   << "Everything looks OK";

	window << "Fill the following form: " << ui::endl;

	coord c(ui::scr_width/4, ui::scr_height/3);
	box b(c, ui::scr_width / 3, 10);

	b.settcolor_input(YELLOW);
	b << "Enter details: " << ui::endl
	  << "Name: "; b >> name;
	b << "Age: "; b >> age;
	b << "Phone num: "; b >> phn;
	b << "Date: ";
	b.setdefault("27/10/2017");
	b >> date;
	b << "Amount: "; b >> amt;
	b << "Password: "; b >> box::setpassword >> pwd;

	b.f.setvisibility_mode(frame::nosides);

	b.f.display();
	b.setexit_button("Submit");
	b.loop();

	b.hide();

	window << "You entered the following data: " << ui::endl
	  << "Name: " << name << ui::endl
	  << "Age: " << age << ui::endl
	  << "Phone num: " << phn << ui::endl
	  << "Date: " << date << ui::endl
	  << "Amount: " << amt << ui::endl
	  << "Password: " << pwd << ui::endl;
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

void test_frame()
{
	frame f;
	f.display();

	getch();

	f << ui::top << 't'
	  << ui::left << 'l'
	  << ui::bottom << 'b'
	  << ui::right << 'r';

	f.settcolor(LIGHTBLUE);

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

	f.settcolor(ui::tcolor);

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
