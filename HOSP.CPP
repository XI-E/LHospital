
int hospital::get_employee_by_id(unsigned long ID, void * target)
{
	if(target == NULL)
	{
		interface::log_this("hospital::get_employee_by_id() : NULL pointer supplied to function\nFunction aborted");
		return 0;
	}
	str temp;
	int size_of_target;
	switch(id_to_emp::convert(ID))
	{
		case INVALID:
			interface::log_this("hospital::get_employee_by_id() : Invalid id supplied to function\nFunction aborted");
			return 0;
		case OTHERS:
			sprintf(temp, "employee/%lu/base.dat", ID);
			size_of_target = sizeof(employee);
			break;
		case DOCTOR:
			sprintf(temp, "employee/doctor/%lu/base.dat", ID);
			size_of_target = sizeof(doctor);
			break;
		case NURSE:
			sprintf(temp, "employee/nurse/%lu/base.dat", ID);
			size_of_target = sizeof(nurse);
			break;
		case RECEPTIONIST:
			sprintf(temp, "employee/receptionist/%lu/base.dat", ID);
			size_of_target = sizeof(receptionist);
			break;
	}
	int i = hospital::read_from( ID, (char*) target, size_of_target, temp );
	if(!i)
	{
		target = NULL;
		return 0;
	}
	return 1;
}

int hospital::write_employee(void * a)
{
	if(a == NULL)
	{
		interface::log_this("hospital::write_employee() : NULL pointer supplied to function\nFunction aborted");
		return 0;
	}
	mkdir("employee");
	str temp;
	int size_of_target;
	employee *x = (employee *) a;
	const unsigned long ID = x->get_id();
	switch(id_to_emp::convert(ID))
	{
		case INVALID:
			interface::log_this("hospital::write_employee() : Object with ID zero cannot be written to file\nFunction aborted");
			return 0;
		case OTHERS:
			sprintf(temp, "employee/%lu", ID);
			size_of_target = sizeof(employee);
			break;
		case DOCTOR:
			mkdir("employee/doctor");
			sprintf(temp, "employee/doctor/%lu", ID);
			size_of_target = sizeof(doctor);
			break;
		case NURSE:
			mkdir("employee/nurse");
			sprintf(temp, "employee/nurse/%lu", ID);
			size_of_target = sizeof(nurse);
			break;
		case RECEPTIONIST:
			mkdir("employee/receptionist");
			sprintf(temp, "employee/receptionist/%lu", ID);
			size_of_target = sizeof(receptionist);
			break;
	}
	mkdir(temp);
	strcat(temp, "/base.dat");
	ofstream fout ( temp , ios::out | ios::binary);
	if(!fout)
	{
		interface::log_this("hospital::write_employee() : Employee data file could not be created or accessed\nFunction aborted");
		return 0;
	}
	fout.write( (char *) a , size_of_target );
	if(fout.fail())
	{
		interface::log_this("hospital::write_employee() : Error while writing to file (fout.fail())\nFunction aborted");
		return 0;
	}
	return 1;
}

int hospital::pay_salary(unsigned long id, Date d1, Time t1)
{
	void * e = malloc( sizeof(doctor) );
	if(e == NULL)
	{
		interface::log_this("hospital::pay_salary() : Not enough memory to allocate buffer void * temp = malloc( sizeof(doctor) )");
		interface::error("Out of memory!! Check log");
		getch();
		return 0;
	}
	str temp;
	switch(id_to_emp::convert(id))
	{
		case INVALID:
			interface::log_this("hospital::pay_salary() : Invalid id supplied to function\nFunction aborted");
			return 0;
		case OTHERS:
			sprintf(temp, "employee/%lu/trans.dat", id);
			break;
		case DOCTOR:
			sprintf(temp, "employee/doctor/%lu/trans.dat", id);
			break;
		case NURSE:
			sprintf(temp, "employee/nurse/%lu/trans.dat", id);
			break;
		case RECEPTIONIST:
			sprintf(temp, "employee/receptionist/%lu/trans.dat", id);
			break;
	}
	if(!hospital::get_employee_by_id(id, e))
	{
		interface::log_this("hospital::pay_salary() : Employee not found or error while reading file\nFunction aborted");
		free(e);
		return 0;
	}
	unsigned long inp1;
	char inp2[100] = "Salary paid to ";
	employee * emp = (employee *)e;
	inp1 = emp->get_salary();
	strcat(inp2, emp->get_name());
	transaction t = hospital::deduct_money(inp1, inp2, d1, t1);
	free(e);

	ofstream fout ( temp ,ios::binary | ios::app );
	if(!fout)
	{
		interface::log_this("hospital::pay_salary() : Employee data file could not be created or accessed\nFunction aborted");
		return 0;
	}
	fout.write((char *) &t, sizeof(transaction));
	if(fout.fail())
	{
		interface::log_this("hospital::pay_salary() : Error while writing to file (fout.fail())\nFunction aborted");
		return 0;
	}
	return 1;
}

int hospital::pay_all_salaries()
{
	Date d1 = system::get_date();
	Time t1 = system::get_time();
	unsigned long max_id;
	ifstream fin;
	fin.open("employee/max_id.dat", ios::binary);
	if(!fin)
	{
		interface::log_this("hospital::pay_all_salaries() : No employees found or cannot access file max_id.dat\nFunction aborted");
		return 0;
	}
	else
	{
		fin.read((char *) &max_id, sizeof(unsigned long));
		if(fin.fail())
		{
			interface::log_this("hospital::pay_all_salaries() : Error while reading file max_id.dat(fin.fail())\nFunction aborted");
			return 0;
		}
		if(!employee::get_generate_id_status())
		{	//if generate_id_status is zero, then no id is generated after max_id + 1
			//Thus, the following loop should run max_id + 1 times
			++max_id;
		}
		for(unsigned long i = 1; i <= max_id; ++i)
		{
			int a = hospital::pay_salary(i, d1, t1);
			if(!a)
			{
				str log_msg;
				sprintf(log_msg, "hospital::pay_all_salaries() : Failed to pay salary of id %lu...\nSkipped", i);
				interface::log_this(log_msg);
			}
		}
	}
	return 1;
}

int hospital::time_validity(const char * inp_time)
{
	return time_validity( str_to_time(inp_time) );
}

int hospital::time_validity(Time t)
{
	if( t.hour > 24 || t.minute > 59 || t.second > 59)
	{
		return 0;
	}
	return 1;
}

Time hospital::str_to_time(const char * inp_time)
{
///////In this function invalid time(25:00:00) is returned if time is in incorrect format//////////
	char inp[3][3] = {"25", "0", "0"};
	int inp_x = 0, inp_y = 0;
	Time null(25, 0, 0);
	if( strlen(inp_time) > 8 || strlen(inp_time) < 5 || inp_time[strlen(inp_time) - 1] == ':')
	{
		return null;
	}
	for(int i = 0; i < strlen(inp_time); ++i)
	{
		if(inp_time[i] == ':' && inp_y != 0)
		{
			inp[inp_x][inp_y] = '\0';
			++inp_x;
			inp_y = 0;
			continue;
		}
		else if( (inp_y == 0 && inp_time[i] == ':') || inp_y > 1 
				|| (inp_time[i] < '0' || inp_time[i] > '9') )
		{
			return null;
		}
		inp[inp_x][inp_y] = inp_time[i];
		++inp_y;
	}
	char *endptr;
	null.hour = (unsigned int) strtol(inp[0], &endptr, 10);
	null.minute = (unsigned int) strtol(inp[1], &endptr, 10);
	null.second = (unsigned int) strtol(inp[2], &endptr, 10);
	return null;
}
