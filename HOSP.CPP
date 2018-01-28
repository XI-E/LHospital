#include "hosp.hpp"
#include "iface.hpp"
#include "emp.hpp"
#include <fstream.h>

//////////////////////////////////////////////
///////////////////////////////////////////////// Function definitions for class hospital

float hospital::get_balance(){
	return balance;
}

transaction hospital::deduct_money(float amt, char* reason, Date d, Time t){
	hospital::balance -= amt;

	ofstream hosp_finances ("transactions.dat", ios::out | ios::binary | ios::app);

	transaction temp = transaction( (-1)*amt, d, t, reason);

	hosp_finances.write( (char*) (&temp) , sizeof(transaction) );

	hosp_finances.close();

	return temp;
}

transaction hospital::add_money(float amt, char* reason, Date d, Time t){
	hospital::balance += amt;

	ofstream hosp_finances ("transactions.dat", ios::out | ios::binary | ios::app);

	transaction temp = transaction( (-1)*amt,d, t, reason);

	hosp_finances.write( (char*) (&temp) , sizeof(transaction) );

	hosp_finances.close();

	return temp;
}

transaction* hospital::get_transaction(){
	transaction a[10];

	ifstream hosp_finances ("transactions.dat", ios::in | ios::binary);

	hosp_finances.seekg( (-1) * sizeof(transaction) , hosp_finances.end );

	for(int i = 0; i < 10; i++){
		hosp_finances.read( (char *) &a[i] , sizeof(transaction) );
		hosp_finances.seekg( hosp_finances.tellg() - ( 2 * sizeof(transaction) ) );
	}

	return a;
}

patient hospital::get_patient_by_id(long id){
	patient a;

	str temp;

	sprintf(temp, "patient/%lu/base.dat", id);

	int i = hospital::read_from(id, (char *) &a, sizeof(patient), temp);

	if(!i){
		interface::error("File read error!!");
		getch();
	}

	return a;

}

void hospital::write_patient(patient a){
	str temp, temp2;
	sprintf(temp, "patient/%lu/base.dat", a.get_id());
	sprintf(temp2, "patient/%lu", a.get_id());
	mkdir("patient");
	mkdir(temp2);
	ofstream patient_file ( temp , ios::out | ios::binary );

	if(patient_file){
		patient_file.write( (char*) &a , sizeof(patient) );
	}
	else{
		interface::error("Patient file access failure!!");
	}
	if(patient_file.fail()){
		interface::error("Patient file write failure!!");
	}
	patient_file.close();
}

void hospital::charge_patient(int pat_id, transaction trans){
	patient temp_pat = hospital::get_patient_by_id(pat_id);

	str temp;
	sprintf(temp, "patient/%d/trans.dat", temp_pat.get_id());
	ofstream patient_file ( temp , ios::out | ios::binary | ios::app );
	patient_file.write( (char*) &trans , sizeof(transaction) );
	patient_file.close();

	hospital::write_patient(temp_pat);
}

void hospital::discharge_patient(patient temp){
	temp.discharge();
	temp.set_discharge_date( system::get_date() );
	hospital::write_patient(temp);
}

float hospital::calc_bill(int stay){
	return stay * ::stay_charge;
}

medicine hospital::get_med_by_code(int inp_code){
	fstream meds ("stock/med.dat", ios::in | ios::binary);

	medicine temp;
	
	if(inp_code < 1 || inp_code > 100){
		temp.code = 0;
		temp.price = 0;
		temp.dosage = 0;
		temp.stock = 0;
		strcpy(temp.name, "Shell Medicine");

		interface::error("Invalid medicine code!!");

		return temp;
	}

	for(int i = 0; i<100; i++){
		meds.read((char*) &temp, sizeof(medicine));
		if(temp.code == inp_code){
			break;
		}
	}

	return temp;
}

void hospital::write_med(medicine inp_med){
	fstream med_file ("stock/med.dat", ios::in | ios::out | ios::binary);
	med_file.seekg(0);

	int success = 0;
	
	while (!success){
		medicine a;
		med_file.read( (char*) &a, sizeof(medicine) );
		if(a.code==inp_med.code){
			med_file.seekg( med_file.tellg() - sizeof(medicine) );
			med_file.write( (char*) &a, sizeof(medicine) );
			success++;
		}
	}

}

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

int hospital::get_date_difference(Date dt1, Date dt2)
{

	long int n1 = dt1.year*365 + dt1.day;

	for (int i=0; i<dt1.month - 1; i++){
		n1 += monthDays[i];
	}
	n1 += hospital::count_leap_years(dt1);

	long int n2 = dt2.year*365 + dt2.day;

	for (i=0; i<dt2.month - 1; i++){
			n2 += monthDays[i];
	}
	n2 += count_leap_years(dt2);

	return (n2 - n1);
}

int hospital::count_leap_years(Date d)
{
	int years = d.year;

	if (d.month <= 2){
		years--;		// checking whether to count the current year
	}

	return (years / 4) - (years / 100) + (years / 400);
}

int hospital::date_validity(const char * inp_date){
	return hospital::date_validity(hospital::str_to_date(inp_date));
}

int hospital::date_validity(Date inp_date){
	 if(inp_date.year % 4 == 0 && inp_date.month == 2 &&
		inp_date.day == 29){
			return 1;
	}
	if (
		inp_date.month > 12 || 
		inp_date.day > monthDays[inp_date.month - 1])
	{
		return 0;
	}
	else{
		return 1;
	}
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

Date hospital::str_to_date(const char * inp_date){
	int counter = 0;
	int count = 0;
	int input[3];
	input[0] = input[1] = input[2] = 0;
	while(counter < 3){
		char ch[12];
		ch[0] = '/';
		for(int i = 1; i < 7; i++){
			ch[i] = inp_date[count];
			count++;
			if(ch[i] == '/' || ch[i] == '\\' || ch[i] == 0 || ch[i] == '-'){
				if(ch[i] == 0 && count < 11){
					interface::error("Invalid date!");
					return Date (99, 99, 9999);
				}
				ch[i] = '/';
				int temp = i-1, temp2 = 0;
				while(ch[temp] != '/'){
					input[counter] += ( pow(10, temp2) * ((int)ch[temp] - (int)'0') );
					temp--;
					temp2++;
				}
				counter++;
			}
		}
	}

	return Date(input[0], input[1], input[2]);
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

int hospital::str_to_sex(char* s){
	if(	strcmp(s, "M") 	)	{ return 0; }
	else if(	strcmp(s, "F") 	)	{ return 1; }
	else { return 2; }
}

int hospital::read_from(unsigned long ID, char * dest, int size, char * temp)
{
	ifstream fin ( temp , ios::in | ios::binary );
	if(!fin)
	{
		char errmsg[200];
		sprintf(errmsg, "hospital::read_from() : Employee with id %lu not found\nFunction aborted", ID);
		interface::log_this(errmsg);
		return 0;
	}
	fin.read( dest, size );
	if(fin.fail())
	{
		interface::log_this("hospital::read_from() : Error while reading from file (fin.fail())\nFunction aborted");
		return 0;
	}
	fin.close();
	return 1;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

double hospital::balance = 10000000.0;
