#include "ui.hpp"

node_printer::node_printer(char s[])
{
	next = NULL;
	pos = coord(1, 1);
	strcpy(str, s);
	tcolor = LIGHTGRAY; //TODO
	bcolor = BLACK;
}

node_printer::~node_printer()
{
	delete next;
	next = NULL;
}

void node_printer::setnext(node_printer *n)
{
	next = n;
}

void node_printer::setpos(coord p)
{
	pos = p;
}

void node_printer::setstr(char s[])
{
	strcpy(str, s);
}

void node_printer::settcolor(int c)
{
	tcolor = c;
}

void node_printer::setbcolor(int c)
{
	bcolor = c;
}

node_printer * node_printer::getnext()
{
	return next;
}

coord node_printer::getpos()
{
	return pos;
}

char * node_printer::getstr()
{
	return str;
}

int node_printer::gettcolor()
{
	return tcolor;
}

int node_printer::getbcolor()
{
	return bcolor;
}

void node_printer::print()
{
	textcolor(tcolor);
	textbackground(bcolor);
	cprintf("%s", str);
	textcolor(ui::tcolor);
	textbackground(ui::bcolor);
}



printer::printer(int w, int h)
{
	head = new node_printer;
	current = head;
	width = w;
	height = h;
	corner_top_left = coord (1,1);
}

printer::~printer()
{
	delete head;
	head = NULL;
	current = NULL;
}

void printer::settcolor(int c)
{
	current->settcolor(c);
}

void printer::setbcolor(int c)
{
	current->setbcolor(c);
}

void printer::setcorner_top_left(coord c)
{
	corner_top_left = c;
	gotoxy(c.getx(), c.gety());
}

void printer::setheight(int h)
{
	height = h;
}

void printer::setwidth(int w)
{
	node_printer *temp = current;
	current = head;

	while(current != NULL)
	{
		coord pos = current->getpos();
		pos.setx ( ((pos.getx()+1) * w) / width - 1);
		current->setpos(pos);
		current = current->getnext();
	}

	width = w;
	current = temp;
}

int printer::gettcolor()
{
	return current->gettcolor();
}

int printer::getbcolor()
{
	return current->getbcolor();
}

coord printer::getcorner_top_left()
{
	return corner_top_left;
}

int printer::getheight()
{
	return height;
}

int printer::getwidth()
{
	return width;
}

printer & printer::operator<<(int s)
{
	state = s;
	return *this;
}

printer & printer::operator<<(char s[])
{
	int x = 0, 
		y = wherey() - corner_top_left.gety();

	if(state & ui::centeralign)
	{
		x += (width - strlen(s)) / 2;
	}
	else if(state & ui::rightalign)
	{
		x += (width - strlen(s));
	}

	current->setpos( coord(x,y) );

	current->setstr(s);

	gotoxy( x + corner_top_left.getx()
		   ,y + corner_top_left.gety());
	current -> print(); 

	node_printer *prev = current;
	current = new node_printer;
	prev->setnext(current);

	return *this;
}

//Starts printing from where current points to
void printer::print()
{
	for(int l = 0; l < height; l++)
	{
		if(current == NULL) return;

		textcolor( current->gettcolor() );
		textbackground( current->getbcolor() );
		coord &pos = current->getpos();
		gotoxy( pos.getx() + corner_top_left.getx()
			   ,pos.gety() + corner_top_left.gety());
		cprintf("%s", current->getstr());

		current = current -> getnext();
	}
}

void printer::gotobegin()
{
	current = head;
}

void printer::hide()
{
	gotoxy(corner_top_left.getx(), corner_top_left.gety());
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			cprintf(" ");
		}
		gotoxy(corner_top_left.getx(), wherey() + 1);
	}
}