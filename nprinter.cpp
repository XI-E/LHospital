#include "ui.hpp"


list_layout_node::list_layout_node()
{
    next = NULL;
    tcolor = ui::tcolor;
    bcolor = ui::bcolor;
    strcpy(str, "");
}

list_layout_node::~list_layout_node()
{
    delete next;
    next = NULL;
}

//Setters
void list_layout_node::setnext(list_layout_node *n)
{
    next = n;
}

void list_layout_node::setpos(coord p)
{
    pos = p;
}

void list_layout_node::settcolor(int t)
{
    tcolor = t;
}

void list_layout_node::setbcolor(int b)
{
    bcolor = b;
}

void list_layout_node::setstr(const char * s)
{
    strcpy(str, s);
}

//Getters
list_layout_node * list_layout_node::getnext()
{
    return next;
}

coord list_layout_node::getpos()
{
    return pos;
}

int list_layout_node::gettcolor()
{
    return tcolor;
}

int list_layout_node::getbcolor()
{
    return bcolor;
}

const char * list_layout_node::getstr()
{
    return str;
}

string_node::string_node()
{
    next = NULL;
    prev = NULL;
    data = '\0';
}

interactive::interactive()
{
    prev = NULL;
    next = NULL;
}

interactive::~interactive()
{
    delete next;
    next = NULL;
    prev = NULL;
}

int interactive::input(int)
{
    return -1;
}

void interactive::setoffset(int o)
{
    offset = o;
}

int interactive::getoffset()
{
    return offset;
}

int interactive::getkey()
{
    char ch = getch();
    switch(ch)
    {
        case 9:     return TAB;
        case 13:    return ENTER;
        case 8:     return BACKSPACE;
        case 0:     break;
        default:    return ch;
    }

    ch = getch();

    unsigned char far *key_state_byte
    = (unsigned char far*) 0x00400017;
    int key_state = (int) *key_state_byte;

    switch(ch)
    {
        case 72:    return UP;
        case 80:    return DOWN;
        case 75:    return LEFT;
        case 77:    return RIGHT;
        case 15:    if (key_state & 2) return SHIFT_TAB;
        //           ^^ Checks if shift was pressed
    }

    return -1;
}

/*
* Despite trying, this function has grown quite large
* Basically, it allows the user to enter text in the box
* and stores it.
* Returns GOTONEXT or GOTOPREV as per user's request to 
* go to the next or the previous text box respectively
*/
int text_box::input(int a)
{
    coord c = getpos();
    setoffset(a);
    c.y += a;
    gotoxy(c.x, c.y);

    const char *string = getstr();
    char str[100];
    strcpy(str, string);

    string_node *head = new string_node,
                *current = head;

    int len = strlen(str);
    string_node *temp_prev = NULL;
    for(int i = 0; i < len ; i++)
    {
        current->data = str[i];
        current->next = new string_node;
        current->prev = temp_prev;
        temp_prev = current;
        current = current->next;
    }

    //At the end is a box with \0
    current->data = '\0';
    current->prev = temp_prev;
    current = head;

    int state_to_return = -1;

    while(1)
    {
        if(kbhit())
        {
            char ch = interactive::getkey();

            switch((int)ch)
            {
                case interactive::TAB :
                case interactive::ENTER :
                    state_to_return = GOTONEXT;
                    goto convert_to_str;
                case interactive::BACKSPACE :
                    if(current)
                    {
                        if(!current->prev)  break; //No character to be deleted

                        string_node *node_to_delete = current->prev;

                        if(node_to_delete->prev) node_to_delete->prev->next = current;
                        else                     head = current; //If the node to be deleted is the head

                        current->prev = node_to_delete->prev;

                        delete node_to_delete;

                        gotoxy(wherex() - 1, wherey());

                        print_str(head);
                    }
                    break;
                case SHIFT_TAB:
                    state_to_return = GOTOPREV;
                    goto convert_to_str;
                case UP:
                    state_to_return = GOTOPREV;
                    goto convert_to_str;
                case DOWN:
                    state_to_return = GOTONEXT;
                    goto convert_to_str;
                case LEFT:
                    if(current->prev)
                    {
                        current = current->prev; 
                        gotoxy(wherex()-1, wherey());   
                    }
                    break;
                case RIGHT: //Right arrow key
                    if(current->next)
                    {
                        current = current->next;
                        gotoxy(wherex()+1, wherey());
                    }
                    break;
                default:
                    if(isprint(ch))
                    {
                        /*
                        * When a new node is to be added, it is added behind 
                        * the current node
                        */

                        string_node *new_node = new string_node;
                        new_node->data = ch;
                        new_node->next = current;
                        new_node->prev = current->prev;

                        if(current->prev) current->prev->next = new_node;
                        else              head = new_node;
                        current->prev = new_node;

                        gotoxy(wherex()+1, wherey());
                        print_str(head);
                    }
            }
        }
    }

    convert_to_str:
    {
        char a[100]; int insert_pointer = 0;
        for(current = head; current; current = current->next)
        {
            a[insert_pointer] = current->data;
            insert_pointer++;
        }
        
        setstr(a);

        //Deleting the list
        current = head;
        head = NULL;
        while(current)
        {
            string_node *temp = current->next;
            delete current;
            current = temp;
        }

        return state_to_return;
    }

}

/*
* Prints the string as represented by a doubly
* linked list whose head is pointed to by the 
* parameter.
*/
void text_box::print_str(string_node *head)
{
    coord init = coord(wherex(), wherey());
    coord c = getpos();
    gotoxy(c.x, c.y + getoffset());
    textcolor(gettcolor());
    textbackground(getbcolor());
    for(string_node *current = head; current; current = current->next)
    {
        cprintf("%c", current->data);
    }
    gotoxy(init.x, init.y);
}

button::button()
{
    tcolor_selected = BLACK;
    bcolor_selected = LIGHTGRAY;
}

void button::settcolor_selected(int c)
{
    tcolor_selected = c;
}

void button::setbcolor_selected(int c)
{
    bcolor_selected = c;
}

int button::gettcolor_selected()
{
    return tcolor_selected;
}

int button::getbcolor_selected()
{
    return bcolor_selected;
}

int button::input(int offset)
{
    coord c = getpos();
    setoffset(offset);
    c.y += offset;
    gotoxy(c.x, c.y);

    print(1);

    int state_to_return;
    while(1)
    {
        if(kbhit())
        {
            char ch = interactive::getkey();
            switch((int) ch)
            {
                case interactive::ENTER :
                    state_to_return = interactive::CLICKED;
                    goto next;
                case interactive::DOWN :
                case interactive::TAB  :
                    state_to_return = interactive::GOTONEXT;
                    goto next;
                case interactive::UP :
                case interactive::SHIFT_TAB :
                    state_to_return = interactive::GOTOPREV;
                    goto next;
            }
        }
    }

    next:
    {
        if (
            state_to_return == interactive::GOTONEXT ||
            state_to_return == interactive::GOTOPREV
           )
        {
            print(0);
        }

        return state_to_return;
    }
}

void button::print(int isselected)
{
    if(isselected)
    {
        textcolor(tcolor_selected);
        textbackground(bcolor_selected);
    }
    else
    {
        textcolor(gettcolor());
        textbackground(getbcolor());
    }

    coord init_pos(wherex(), wherey());
    coord c = getpos();
    gotoxy(c.x, c.y + getoffset());
    cprintf(getstr());
    gotoxy(init_pos.x, init_pos.y);
}

void list_layout::print(int print_mode)
{
    coord init_pos(wherex(), wherey());
    for(list_layout_node *curr = head; curr; curr = curr->getnext())
    {
        coord c = curr->getpos();
        int new_y = c.y - lines_scrolled;

        if(new_y <= 0 || new_y > height) continue;

        gotoxy(c.x, new_y);
        textcolor(curr->gettcolor());
        textbackground(curr->getbcolor());
        if(print_mode == DISPLAY)
        {
            cprintf("%s", curr->getstr());
        }
        else if(print_mode == HIDE)
        {
            int len = strlen(curr->getstr());
            for(int i = 0; i < len; i++)
            {
                cprintf(" ");
            }
        }
    }
    gotoxy(init_pos.x, init_pos.y);
}

list_layout::list_layout()
{
    head = NULL,
    current = NULL;

    tcolor = ui::tcolor;
    bcolor = ui::bcolor;
    tcolor_selected = ui::bcolor;
    bcolor_selected = ui::tcolor;
    tcolor_input = tcolor;
    bcolor_input = bcolor;

    height = ui::scr_height - 1;
    width = ui::scr_width;
    lines_scrolled = 0;
}

list_layout& list_layout::operator<<(coord c)
{
    pos = c;
    return *this;
}

list_layout& list_layout::operator<<(const char *str)
{
    if(!head) //empty list
    {
        head = new list_layout_node;
        current = head;
    }
    else
    {
        list_layout_node *new_node = new list_layout_node;
        current->setnext(new_node);
        current = current->getnext();
    }

    current->setpos(pos);
    current->setstr(str);
    current->settcolor(tcolor);
    current->setbcolor(bcolor);

    print();

    return *this;
}

interactive * list_layout::settext_box(coord c)
{
    interactive *new_node = new text_box;
    new_node->setpos(c);
    new_node->settcolor(tcolor_input);
    new_node->setbcolor(bcolor_input);

    current->setnext(new_node);
    current = current->getnext();

    return new_node;
}

interactive * list_layout::setbutton(coord c, const char *s)
{
    button *new_node = new button;
    new_node->setpos(c);
    new_node->settcolor(tcolor);
    new_node->setbcolor(bcolor);
    new_node->settcolor_selected(tcolor_selected);
    new_node->setbcolor_selected(bcolor_selected);
    new_node->setstr(s);

    interactive *n = (interactive *) new_node;
    current->setnext(n);
    current = current->getnext();

    return n;
}

void list_layout::settcolor(int c)
{
    tcolor = c;
    tcolor_input = c;
}

void list_layout::setbcolor(int c)
{
    bcolor = c;
    bcolor_input = c;
}

void list_layout::settcolor_selected(int c)
{
    tcolor_selected = c;
}

void list_layout::setbcolor_selected(int c)
{
    bcolor_selected = c;
}

void list_layout::settcolor_input(int c)
{
    tcolor_input = c;
}

void list_layout::setbcolor_input(int c)
{
    bcolor_input = c;
}

void list_layout::setcorner_top_left(coord c)
{
    hide();

    coord offset = c - corner_top_left;
    //offset isn't a coordinate but it's just a pair of values

    for(list_layout_node *curr = head; curr; curr = curr->getnext())
    {
        coord a = curr->getpos();
        a += offset;
        curr->setpos(a);
    }

    corner_top_left += offset;

    display();
}

void list_layout::setheight(int h)
{
    hide();
    height = h;
    display();
}

void list_layout::setwidth(int w)
{
    width = w;
}

void list_layout::setlines_scrolled(int l)
{
    hide();
    lines_scrolled = l;
    display();
}

void list_layout::setpos(coord c)
{
    pos = c;
}

int list_layout::getheight()
{
    return height;
}

int list_layout::getwidth()
{
    return width;
}

int list_layout::getlines_scrolled()
{
    return lines_scrolled;
}

coord list_layout::getpos()
{
    return pos;
}

coord list_layout::getcorner_top_left()
{
    return corner_top_left;
}

void list_layout::display()
{
    print(DISPLAY);
}

void list_layout::hide()
{
    print(HIDE);
}

void list_layout::clear()
{
    list_layout_node *curr = head;
    head = current = NULL;

    while(curr)
    {
        list_layout_node *temp = curr->getnext();
        delete curr;
        curr = temp;
    }

    lines_scrolled = 0;
    pos = corner_top_left;
}

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

box::box(int w, int h) : f(coord(1,1), w, h)
{
    width = w;
    height = h;
    padding = 1;

    corner_top_left = coord(1,1);

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
    layout.setcorner_top_left(coord(3,3));

    pos_pointer = layout.getcorner_top_left();

    for(int i = 0; i < 30; i++)
    {
        list_interactive[i] = NULL;
    }
    exit_btn = NULL;
    index_interactive = index_tbox = 0;

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
        coord c = list_interactive[j]->getpos();
        if(c.y - lines_scrolled > height)
		{
			lines_scrolled = c.y - height;
		}
		else if(c.y - lines_scrolled < layout.getcorner_top_left().y)
		{
            lines_scrolled = 
                c.y - layout.getcorner_top_left().y;
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