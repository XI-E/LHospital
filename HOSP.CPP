#include "hosp.hpp"
#include "iface.hpp"
#include "emp.hpp"
#include <fstream.h>

//////////////////////////////////////////////
///////////////////////////////////////////////// Function definitions for class hospital

float hospital::get_bal(){
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

patient hospital::get_patient_by_id(int id){
	if(!id)
	{
		patient null;
		return null;
	}
	str temp;
	sprintf(temp, "patient/%d/base.dat", id);
	ifstream patient_file ( temp , ios::in | ios::binary );

	if(!patient_file){
		// pass an error ---------------------------------------------------
		patient b;
		return b;
	}

	patient a;
	patient_file.read( (char*) &a , sizeof(patient) );
	patient_file.close();

	return a;
}

void hospital::write_patient(patient a){
	str temp, temp2;
	sprintf(temp, "patient/%d/base.dat", a.get_id());
	sprintf(temp2, "patient/%d", a.get_id());
	mkdir(temp2);
	ofstream patient_file ( temp , ios::out | ios::binary );

	if(patient_file){
		patient a;
		patient_file.write( (char*) &a , sizeof(patient) );
	}
	if(patient_file.fail()){
		// pass an error ---------------------------------------------------
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

medicine get_med_by_code(int inp_code){
	fstream meds ("hospital/medicine.dat", ios::in | ios::binary);

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

int hospital::get_employee_by_id(unsigned long ID, void * target)
{
	str temp;
	int size_of_target;
	switch(id_to_emp::convert(ID))
	{
		case INVALID:
//			cerr<<"Invalid id";
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
/*
int hospital::get_employee_by_id(unsigned long ID, doctor &target)
{
	doctor null;
	str temp;
	sprintf(temp, "employee/doctor/%lu/base.dat", ID);
	int i = hospital::read_from( ID, (char*) &target, sizeof(doctor), temp );
	if(!i)
	{
		target = null;
		return 0;
	}
	return 1;
}

int hospital::get_employee_by_id(unsigned long ID, nurse &target)
{
	nurse null;
	str temp;
	sprintf(temp, "employee/nurse/%lu/base.dat", ID);
	int i = hospital::read_from( ID, (char*) &target, sizeof(nurse), temp );
	if(!i)
	{
		target = null;
		return 0;
	}
	return 1;
}

int hospital::get_employee_by_id(unsigned long ID, receptionist &target)
{
	receptionist null;
	str temp;
	sprintf(temp, "employee/receptionist/%lu/base.dat", ID);
	int i = hospital::read_from( ID, (char*) &target, sizeof(receptionist), temp );
	if(!i)
	{
		target = null;
		return 0;
	}
	return 1;
}
*/
int hospital::write_employee(void * a)
{
	mkdir("employee");
	str temp;
	int size_of_target;
	employee *x = (employee *) a;
	const unsigned long ID = x->get_id();
	switch(id_to_emp::convert(ID))
	{
		case INVALID:
//			cerr<<"Invalid id";
			return 0;
		case OTHERS:
			sprintf(temp, "employee/%lu", ID);
			size_of_target = sizeof(employee);
			break;
		case DOCTOR:
			sprintf(temp, "employee/doctor/%lu", ID);
			size_of_target = sizeof(doctor);
			break;
		case NURSE:
			sprintf(temp, "employee/nurse/%lu", ID);
			size_of_target = sizeof(nurse);
			break;
		case RECEPTIONIST:
			sprintf(temp, "employee/receptionist/%lu", ID);
			size_of_target = sizeof(receptionist);
			break;
	}
	mkdir(temp);
	strcat(temp, "/base.dat");
	ofstream fout ( temp , ios::out | ios::binary);
	if(!fout)
	{
//		cerr<<"Employee data file could not be created or accessed";
		return 0;
	}
	fout.write( (char *) a , size_of_target );
	if(fout.fail())
	{
//		cerr<<"Error while writing to file";
		return 0;
	}
	return 1;
}
/*
int hospital::write_employee(doctor a)
{
	mkdir("employee");
	mkdir("employee/doctor");
	str temp;
	sprintf(temp, "employee/doctor/%lu", a.get_id());
	mkdir(temp);
	strcat(temp, "/base.dat");
	ofstream fout ( temp , ios::out | ios::binary);
	if(!fout)
	{
//		cerr<<"Employee data file could not be created or accessed";
		return 0;
	}
	fout.write( (char*) &a , sizeof(doctor) );
	if(fout.fail())
	{
//		cerr<<"Error while writing to file";
		return 0;
	}
	return 1;
}

int hospital::write_employee(nurse a)
{
	mkdir("employee");
	mkdir("employee/nurse");
	str temp;
	sprintf(temp, "employee/nurse/%lu", a.get_id());
	mkdir(temp);
	strcat(temp, "/base.dat");
	ofstream fout ( temp , ios::out | ios::binary);
	if(!fout)
	{
//		cerr<<"Employee data file could not be created or accessed";
		return 0;
	}
	fout.write( (char*) &a , sizeof(nurse) );
	if(fout.fail())
	{
//		cerr<<"Error while writing to file";
		return 0;
	}
	return 1;
}

int hospital::write_employee(receptionist a)
{
	mkdir("employee");
	mkdir("employee/receptionist");
	str temp;
	sprintf(temp, "employee/receptionist/%lu", a.get_id());
	mkdir(temp);
	strcat(temp, "/base.dat");
	ofstream fout ( temp , ios::out | ios::binary);
	if(!fout)
	{
//		cerr<<"Employee data file could not be created or accessed";
		return 0;
	}
	fout.write( (char*) &a , sizeof(receptionist) );
	if(fout.fail())
	{
//		cerr<<"Error while writing to file";
		return 0;
	}
	return 1;
}
*/
int hospital::pay_salary(unsigned long id, Date d1, Time t1)
{
	employee e;
	str temp;
	
	if(!hospital::get_employee_by_id(id, &e))
	{
//		cerr<<"Employee not found or error while reading file";
		return 0;
	}
	unsigned long inp1;
	char inp2[100] = "Salary paid to ";
	inp1 = e.get_salary();
	strcat(inp2, e.get_name());
	transaction t = hospital::deduct_money(inp1, inp2, d1, t1);

	strcat(temp, "/trans.dat");
	ofstream fout ( temp ,ios::binary | ios::app );
	if(!fout)
	{
//		cerr<<"Employee data file could not be created or accessed";
		return 0;
	}
	fout.write((char *) &t, sizeof(transaction));
	if(fout.fail())
	{
//		cerr<<"Error while writing to file";
		return 0;
	}
	return 1;
}

int hospital::pay_all_salaries()
{
	Date d1;
	d1 = system::get_date();
	Time t1;
	t1 = system::get_time();
	unsigned long max_id;
	ifstream fin;
	fin.open("employee/max_id.dat", ios::binary);
	if(!fin)
	{
//		cerr<<"No employees found or cannot access file";
		return 0;
	}
	else
	{
		fin.read((char *) &max_id, sizeof(unsigned long));
		if(fin.fail())
		{
//			cerr<<"Error while reading file!!";
			return 0;
		}
		if(!employee::get_generate_id_status())
		{
			++max_id;
		}
		for(unsigned long i = 1; i <= max_id; ++i)
		{
			int a = hospital::pay_salary(i, d1, t1);
			if(!a)
			{
//				cerr<<"Failed to pay all salaries...";
				return 0;
			}
		}
	}
	return 1;
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

int hospital::read_from(unsigned long ID, char * dest, int size, char * temp)
{
	ifstream fin ( temp , ios::in | ios::binary );
	if(!fin)
	{
//		cerr<<"Employee with id "<<ID<<" not found...";
		return 0;
	}
	fin.read( (char*) dest, size );
	if(fin.fail())
	{
//		cerr<<"Error while reading from file";
		return 0;
	}
	fin.close();

	return 1;
}

int hospital::count_leap_years(Date d)
{
	int years = d.year;

	if (d.month <= 2){
		years--;		// checking whether to count the current year
	}

	return (years / 4) - (years / 100) + (years / 400);
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

int hospital::date_validity(const char * inp_date){
	return hospital::date_validity(hospital::str_to_date(inp_date));
}

int hospital::date_validity(Date inp_date){
	if (
		inp_date.month > 12 || 
		inp_date.day > monthDays[inp_date.month - 1]
	)
	{
		return 0;
	}
	else{
		return 1;
	}
}

Date hospital::str_to_date(const char * inp_date){
	int counter = 0;
	int count = 0;
	int input[3];
	input[0] = input[1] = input[2] = 0;
	while(counter < 3){
		char ch[10];
		for(int i = 0; i < 9; i++){
			ch[i] = inp_date[count];
			count++;
			if(ch[i] == '/' || ch[i] == '\\' || ch[i] == 0){
				if(ch[i] == 0 && count < 10){
					interface::error("Invalid date entry!!");
					return system::get_date();
				}
				int temp = i;
				while(temp){
					input[counter] += ( pow(10, temp) * ch[i-temp] );
					temp--;
				}
				counter++;
			}
		}
	}
	return Date(input[0], input[1], input[2]);
}

int hospital::str_to_sex(char* s){
	if(	strcmp(s, "M") 	)	{ return 0; }
	else if(	strcmp(s, "F") 	)	{ return 1; }
	else { return 2; }
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

double hospital::balance = 1000.0;