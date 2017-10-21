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

    return 0;
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
    c.sety(c.gety() + a);
    gotoxy(c.getx(), c.gety());

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
    gotoxy(c.getx(), c.gety() + getoffset());
    textcolor(gettcolor());
    textbackground(getbcolor());
    for(string_node *current = head; current; current = current->next)
    {
        cprintf("%c", current->data);
    }
    gotoxy(init.getx(), init.gety());
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
    c.sety(c.gety() + offset);
    gotoxy(c.getx(), c.gety());

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
    gotoxy(c.getx(), c.gety() + getoffset());
    cprintf(getstr());
    gotoxy(init_pos.getx(), init_pos.gety());
}

void list_layout::print(int print_mode)
{
    coord init_pos(wherex(), wherey());
    for(list_layout_node *curr = head; curr; curr = curr->getnext())
    {
        coord c = curr->getpos();
        int new_y = c.gety() - lines_scrolled;

        if(new_y <= 0 || new_y > height) continue;

        gotoxy(c.getx(), new_y);
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
    gotoxy(init_pos.getx(), init_pos.gety());
}

list_layout::list_layout()
{
    head = NULL,
    current = NULL;

    tcolor = LIGHTGRAY;
    bcolor = BLACK;
    tcolor_selected = BLACK;
    bcolor_selected = LIGHTGRAY;

    height = ui::scr_height - 1;
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
    new_node->settcolor(tcolor);
    new_node->setbcolor(bcolor);

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
}

void list_layout::setbcolor(int c)
{
    bcolor = c;
}

void list_layout::settcolor_selected(int c)
{
    tcolor_selected = c;
}

void list_layout::setbcolor_selected(int c)
{
    bcolor_selected = c;
}

void list_layout::display()
{
    print(DISPLAY);
}

void list_layout::hide()
{
    print(HIDE);
}