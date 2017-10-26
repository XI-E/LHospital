#include "ui.hpp"

/*
* Wraps a string with specified number of
* characters (length) in each line. Returns number of lines
*/
int body::wrap(char str[], int length)
{
	char *line = strtok(str, "\n");
	
	int num_lines = 0;
	char out_str[300] = "";
	
	while(line != NULL)
	{
		num_lines++;

		int chars_read = 0,
			read,
			written = 0;

		char word[30];

		while(sscanf(line + chars_read, "%s%n", word, &read) > 0)
		{
			int word_len = strlen(word);
			if(written + word_len  > length)
			{
				num_lines++;
				sprintf(out_str + strlen(out_str), "\n%s ", word);
				written = word_len + 1;
			}
			else
			{
				sprintf(out_str + strlen(out_str), "%s ", word);
				written += word_len + 1;
			}
			chars_read += read;
		}
		sprintf(out_str + strlen(out_str), "\n");

		line = strtok(NULL, "\n");
	}
	
	sprintf(str, "%s", out_str);
	return num_lines;
}

body::body(int w, int h)
{
	height = h;
	width = w;
	padding[0] = 1, padding[1] = 1;
	p.setwidth(width - 2 * padding[0]);
	p.setheight(height - 2 * padding[1]);	
}

body & body::operator<< (int s)
{
	p << s;
	return *this;
}

body & body::operator<< (char str[])
{
	int num_lines = wrap(str, p.getwidth());
	char *line = strtok(str, "\n");
	for(int i = 0; i < num_lines; i++)
	{
		p << line;
		p << "\n";
		line = strtok(NULL, "\n");
	}
	line = strtok(NULL, "\n");
	return *this;
}

void body::print()
{
    p.print();
}

void body::hide()
{
    p.hide();
}

void body::setcorner_top_left(coord c)
{
	corner_top_left = c;
	c.setx(c.getx() + padding[0]);
	c.sety(c.gety() + padding[1]);
	p.setcorner_top_left(c);
}

void body::setheight(int h)
{
	height = h;
	p.setheight(h - 2 * padding[1]);
}

void body::setwidth(int w)
{
	width = w;
	p.setwidth(w - 2 * padding[0]);
}

void body::setpaddingx(int px)
{
	padding[0] = px;
}

void body::setpaddingy(int py)
{
	padding[1] = py;
}

coord body::getcorner_top_left()
{
	return corner_top_left;
}

int body::getheight()
{
	return height;
}

int body::getwidth()
{
	return width;
}

int body::getpaddingx()
{
	return padding[0];
}

int body::getpaddingy()
{
	return padding[1];
}