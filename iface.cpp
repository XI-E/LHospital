int interface::validate_menu::input(const char * ch)
{
	char *endptr;
	int a = (int) strtol(ch, &endptr, 10);
	if(!validation::vint(ch) || a < lowest_choice || a > greatest_choice)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void interface::validate_menu::set_menu_limits(int a, int b)
{
	lowest_choice = a;
	greatest_choice = b;
}

int interface::validate_menu::lowest_choice = 0;
int interface::validate_menu::greatest_choice = 0;

int interface::back_func::set_backbit()
{
	backbit = 1;
	return 1;
}

int interface::back_func::backbit = 0;

int interface::log_this(char * message)
{
	Date dnow = system::get_date();
	Time tnow = system::get_time();
	char text[300];
	sprintf(text, "$ [%u-%u-%u %u:%u:%u +0530]: ", dnow.day, dnow.month, dnow.year, tnow.hour, tnow.minute, tnow.second);
	strcat(text, message);
	ofstream fout;
	fout.open("log.txt", ios::out | ios::app);
	if(!fout)
		return 0;
	fout << text << endl;
	if(fout.fail())
		return 0;
	fout.close();
	return 1;
}

interface::interface(){}