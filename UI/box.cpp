#include "ui.hpp"

info_tbox::info_tbox()
{
    tbox = NULL;
    data_store = NULL;
    type = OTHER;
}

void info_tbox::setdata()
{
    char *fstr;
    switch(type)
    {
        case INT:
        {
            fstr = "%d";
            break;
        }
        case LONG:
        {
            fstr = "%ld";
            break;
        }
        case STRING:
        {
            char *s = (char *) data_store;
            strcpy(s, tbox->getstr());
            return;
        }
        case CHAR:
        {
            fstr = "%c";
            break;
        }
        case DOUBLE:
        {
            fstr = "%g";
            break;
        }
        case FLOAT:
        {
            fstr = "%f";
            break;
        }
        default:
            return;
    }

    sscanf(tbox->getstr(), fstr, data_store);
}

/*
* Wraps a string with specified number of
* characters (length) in each line. Returns number of lines
*/
int box::wrap(char str[], int length, int return_one_line)
{
	int num_lines = 1;
    char out_str[300] = "";
    
    int pos_old_newline = -1,
        pos_curr_newline = -1;

    int len_str = strlen(str);

    //Iterating upto len_str because the '\0' at the end of the string
    //would be interpreted as a newline
    for(int i = 0; i <= len_str; i++)
    {
        if(str[i] == '\n' || i == len_str)
        {
            pos_old_newline = pos_curr_newline;
            pos_curr_newline = i;

            if(pos_curr_newline != len_str) num_lines++;
            
            int chars_read = 0,
                read,
                written = 0;

            char word[30];

            str[pos_curr_newline] = '\0';

            char *line = str + pos_old_newline + 1;
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

            if(pos_curr_newline != len_str)
                sprintf(out_str + strlen(out_str), "\n"); 
                str[pos_curr_newline] = '\n';
        }
    }
    
    //An extra space is at the end of the string which has to be removed
    //out_str[strlen(out_str) - 1] = '\0';
    sprintf(str, "%s", out_str);
    
    if(!return_one_line)   return num_lines;

    len_str = strlen(str);

    for(i = 0; i <= len_str; i++)   
    {
        if(i == len_str)
        {
            break;
        }
        else if(str[i] == '\n')
        {
            str[i] = '\0';
            break;
        }
    }

    return num_lines;
}

void box::set_tbox(int data_type, void *ptr)
{
    text_box *new_tbox = 
        (text_box *) layout.settext_box(pos_pointer);
    
    pos_pointer.y++;
    pos_pointer.x = layout.getcorner_top_left().x;

    list_interactive[index_interactive]
        = (interactive *) new_tbox;
    info_tbox &t = list_tbox[index_tbox];
    index_interactive++;
    index_tbox++;

    t.tbox = new_tbox;
    t.type = data_type;
    t.data_store = ptr;
}

box::box(coord c, int w, int h) : f(c, w, h)
{
    width = w;
    height = h;
    padding = 1;

    corner_top_left = c;

    f << (ui::top | ui::left) << (char) 201
      << (ui::bottom | ui::left) << (char) 200
      << (ui::top | ui::right) << (char) 187
      << (ui::bottom | ui::right) << (char) 188
      << ui::top << (char) 205
      << ui::bottom << (char) 205
      << ui::left << (char) 186
      << ui::right << (char) 186;

    layout.setwidth(w - 2 - 2 * padding);
    layout.setheight(h - 2 - 2 * padding);
    //                   ^bcoz of frame
    layout.setcorner_top_left(c + 
        coord(1 + padding, 1 + padding));

    pos_pointer = layout.getcorner_top_left();

    for(int i = 0; i < 30; i++)
    {
        list_interactive[i] = NULL;
    }
    exit_btn = NULL;
    index_interactive = index_tbox = 0;
    center_toggle = 0;

    f.display();
}

coord box::getcorner_top_left()
{
    return corner_top_left;
}

int box::getheight()
{
    return height;
}

int box::getwidth()
{
    return width;
}

int box::getpadding()
{
    return padding;
}

void box::setcorner_top_left(coord c)
{
    corner_top_left = c;
    f.setcorner_top_left(c);
    c += coord(1 + padding, 1 + padding);
    layout.setcorner_top_left(c);

    pos_pointer = c;
}

void box::setheight(int h)
{
    height = h;
    f.setheight(h);
    layout.setheight(h - 2 - 2 * padding);
}

void box::setpadding(int p)
{
    padding = p;
}

void box::settcolor(int c)
{
    layout.settcolor(c);
}

void box::setbcolor(int c)
{
    layout.setbcolor(c);
}

void box::settcolor_selected(int c)
{
    layout.settcolor_selected(c);
}

void box::setbcolor_selected(int c)
{
    layout.setbcolor_selected(c);
}

void box::settcolor_input(int c)
{
    layout.settcolor_input(c);
}

void box::setbcolor_input(int c)
{
    layout.setbcolor_input(c);
}

box & box::operator<< (char *inp_str)
{
    char string[100];
    char *str = string;
    strcpy(string, inp_str);

    coord c = layout.getcorner_top_left();

    if(center_toggle)
    {
        int len = strlen(string);
        center_toggle = 0;
        if(len <= layout.getwidth())
        {
            if(pos_pointer.x != c.x)
            {
                pos_pointer.y++;
            }
            pos_pointer.x = c.x + (layout.getwidth() - len)/2;
            layout << pos_pointer << str;
            pos_pointer.y++;
            pos_pointer.x = c.x;
            return *this;
        }
    }
    
    int num_lines;

    if(pos_pointer.x != c.x)
    {
        int remaining_space = layout.getwidth() - 
        (pos_pointer.x - layout.getcorner_top_left().x);
        char s[100];
        strcpy(s, str);
        num_lines = wrap(s, remaining_space, 1);

        layout << pos_pointer << s;

        if(num_lines > 1)   
        {
            pos_pointer.x = c.x;
            pos_pointer.y++;
        }
        else
        {
            pos_pointer.x += strlen(s);
        }

        if (num_lines == 1 || 
            str[strlen(str) - 1] == '\n')   return *this;

        str += strlen(s); //There's an extra space at the end of s
    }

    num_lines = wrap(str, layout.getwidth());

    int len_str = strlen(str),
        pos_curr_newline = -1;

    for(int i = 0; i < len_str; i++)
    {
        if(str[i] == '\n')
        {
            pos_curr_newline = i;

            str[pos_curr_newline] = '\0';
            layout << pos_pointer << str;
            pos_pointer.y++;

            str += strlen(str) + 1;
        }
    }

    if(i == len_str - 1)    return *this;

    layout << pos_pointer << str;
    pos_pointer.x += strlen(str);

    return *this;
}

box & box::operator<<(char ch)
{
    char str[] = {ch, '\0'};
    return (*this) << str;
}

box & box::operator<<(int i)
{
    return (*this) << (long) i;
}

box & box::operator<<(long l)
{
    char str[100];
    sprintf(str,"%ld", l);
    return (*this) << str;
}

box & box::operator<<(double d)
{
    char str[100];
    sprintf(str, "%g", d);
    return (*this) << str;
}

box & box::operator<<(float f)
{
    char str[100];
    sprintf(str, "%f", f);
    return (*this) << str;
}

box & box::operator<<(manipulator m)
{
    if(m == ui::endl)
    {
        pos_pointer.y++;
        pos_pointer.x = layout.getcorner_top_left().x;
    }
    else if(m == ui::centeralign)
    {
        center_toggle = 1;
    }
    return *this;
}

box & box::operator>>(char *&s)
{
    set_tbox(info_tbox::STRING, (void *) s);
    return *this;
}

box & box::operator>>(char &ch)
{
    set_tbox(info_tbox::CHAR, (void *) &ch);
    return *this;
}

box & box::operator>>(int &i)
{
    set_tbox(info_tbox::INT, (void *) &i);
    return *this;
}

box & box::operator>>(long &l)
{
    set_tbox(info_tbox::LONG, (void *) &l);
    return *this;
}

box & box::operator>>(double &d)
{
    set_tbox(info_tbox::DOUBLE, (void *) &d);
    return *this;
}

box & box::operator>>(float &f)
{
    set_tbox(info_tbox::FLOAT, (void *) &f);
    return *this;
}

void box::setexit_button(char *str)
{
    coord c = layout.getcorner_top_left();
    if(pos_pointer.x != c.x)
        pos_pointer.y++;
    
    pos_pointer.x = c.x + (layout.getwidth() - strlen(str)) / 2;

    button * new_btn = 
        (button *) layout.setbutton(pos_pointer, str);

    pos_pointer.y++;
    pos_pointer.x = c.x;

    exit_btn = new_btn;
    list_interactive[index_interactive]
        = (interactive *) new_btn;
    index_interactive++;
}

void box::loop()
{
    int j = 0,
    lines_scrolled = layout.getlines_scrolled(),
    height = layout.getheight(),
    index_last_interactive = index_interactive - 1,
    &ili = index_last_interactive;

    while(1)
    {
        coord c = list_interactive[j]->getpos(),
              ctl = layout.getcorner_top_left();
        if(c.y - ctl.y - lines_scrolled + 1 > height)
		{
			lines_scrolled = c.y - ctl.y - height + 1;
		}
		else if(c.y - lines_scrolled < ctl.y)
		{
            lines_scrolled = 
                c.y - ctl.y;
		}

		layout.setlines_scrolled(lines_scrolled);
        int response = 
            list_interactive[j]->input(-lines_scrolled);

        if(response == interactive::GOTONEXT)
        {
            if(j < ili) j++; else j = 0;
        }
        else if(response == interactive::GOTOPREV)
        {
            if(j > 0) j--; else j = ili;
        }
        else if(response == interactive::CLICKED)
        {
            break;
        }
    }

    for(int i = 0; i < index_tbox; i++)
    {
        list_tbox[i].setdata();
    }
}

void box::display()
{
    layout.display();
    f.display();
}

void box::hide()
{
    layout.hide();
    f.hide();
}

void box::clear()
{
    layout.hide();
    layout.clear();
    pos_pointer = layout.getcorner_top_left();
    index_interactive = index_tbox = 0;
    exit_btn = NULL;
    f.display();
}