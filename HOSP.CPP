#include "ui/ui.hpp"
#include <fstream.h>
#include <string.h>
#include <dir.h>
#include <stdio.h>
#include "patient.hpp"
#include "if.hpp"
#include <math.h>

enum emp_type {INVALID, OTHERS, DOCTOR, NURSE, RECEPTIONIST};
const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class id_to_emp
{
		unsigned long id;
		int employee_type;
	public:
		id_to_emp(unsigned long, int);
		id_to_emp();
		static int convert(unsigned long);
};

class employee : public person{
		int generate_id();
		static int generate_id_status;
	public:
		employee(str, int, Date, address, phone, unsigned long, Time, Time, str = "", str = "");	//for all those with user accounts(doctors, nurses, receptionists), last 2 arguments are to be provided as well
		employee();	//default constructor
		int get_age();		//overridden function
		unsigned long get_salary();
		void set_salary(unsigned long);
		Time get_shift(int);
		void set_shift(int, Time);
		unsigned long get_id();
		transaction * get_last_10_transactions();
		static int get_generate_id_status();
		userid account;
	protected:
		unsigned long id;
		unsigned long salary;
		Time shift_start;
		Time shift_end;
};

class doctor : public employee{
	public:
		doctor(str, int, Date, address, phone, unsigned long, Time, Time, int, int, str, str);
		doctor();	//default constructor
		int * get_speciality();
		long * get_patients();
	private:
		int speciality[2];				// Doctor's specialization
		long patients[10];				// Patients currently under care, can take only 10 at once
};

class nurse : public employee{
	public:
		nurse(str, int, Date, address, phone, unsigned long, Time, Time, str, str);
		nurse();		//default constructor
		long * get_patients();
	private:
		long patients[5];
};

class receptionist : public employee
{
	public:
		receptionist(str, int, Date, address, phone, unsigned long, Time, Time, str, str);
		receptionist();
//		doctor assign_doctor(patient);
};

class pharmacy{
	public:
		static void sale(int, int, int);
		static void purchase(int, int);
};

class hospital
{
	public:
		static float get_bal();
		static transaction deduct_money(float, char*, Date, Time);
		static transaction add_money(float, char*, Date, Time);
		static transaction* get_transaction();
		static void read_balance();

		static patient get_patient_by_id(int);
		static void write_patient(patient);
		static void charge_patient(int, transaction);
		static void discharge_patient(int);

		static medicine get_med_by_code(int);

		static int get_employee_by_id(unsigned long, void *);
//		static int get_employee_by_id(unsigned long, doctor &);	//The new get_employee_by_id hasn't been tested properly, so until that is done, these functions are gonna remain commented
//		static int get_employee_by_id(unsigned long, nurse &);
//		static int get_employee_by_id(unsigned long, receptionist &);
		static int write_employee(void *);
//		static int write_employee(doctor);						//same as above for write function
//		static int write_employee(nurse);
//		static int write_employee(receptionist);
		static int pay_salary(unsigned long, Date, Time);
		static int pay_all_salaries();

		static int get_date_difference(Date, Date);
		static int count_leap_years(Date);
		static int date_validity(const char *);
		static int date_validity(Date);
		static Date str_to_date(const char *);

	private:
		hospital();
		static int read_from(unsigned long, char *, int, char *);
		static double balance;
};

//////////////////////////////////
/// Function definitions for class id_to_emp

id_to_emp::id_to_emp(unsigned long inp1, int inp2)
{
	id = inp1;
	if(!id)
	{
		employee_type = INVALID;
	}
	else
	{
		employee_type = inp2;
	}
	mkdir("employee");
	ofstream fout;
	fout.open("employee/id_list.dat", ios::binary | ios::ate);
	if(!fout)
	{
//		cerr<<"File id_list.dat couldn't be opened...";
	}
	else
	{
		fout.seekp(id * sizeof(id_to_emp), ios::beg);
		fout.write((char *) this, sizeof(id_to_emp));
		if(fout.fail())
		{
//			cerr<<"Error while writing to id_list.dat";
		}
	}
}

id_to_emp::id_to_emp()
{
	id = employee_type = 0;
}

int id_to_emp::convert(unsigned long ID)
{
	id_to_emp a;
	ifstream fin;
	fin.open("employee/id_list.dat", ios::binary);
	if(!fin)
	{
		interface::log_this("id_to_emp::convert() : File id_list.dat not found!!");
		return INVALID;
	}
	fin.seekg( (ID * sizeof(id_to_emp)) );
	fin.read((char *) &a, sizeof(id_to_emp));
	if(fin.fail())
	{
		interface::log_this("id_to_emp::convert() : Error while reading from file id_list.dat");
		return INVALID;
	}
	fin.close();
	if(a.id != ID)
	{
		interface::log_this("id_to_emp::convert() : (For dev only)Error in the code... Recheck it!!");
		return INVALID;
	}
	return a.employee_type;
}

//////////////////////////////////
/// Function definitions for class employee

int employee::generate_id()
{
	mkdir("employee");
	unsigned long max_id;
	ifstream fin;
	fin.open("employee/max_id.dat", ios::binary);
	if(!fin)
		max_id = 0;
	else
	{
		fin.read((char *) &max_id, sizeof(unsigned long));
		if(fin.fail())
		{
//			cerr<<"ERROR WHILE READING FROM FILE!!! ";
			id = 0;
			return 0;
		}
	}
	fin.close();
	++max_id;
	id = max_id;
	ofstream fout;
	fout.open("employee/max_id.dat", ios::binary);
	fout.write((char *) &max_id, sizeof(unsigned long));
	if(fout.fail())
	{
//		cerr<<"ERROR WHILE WRITING TO FILE!!! ";
		return 0;
	}
	else
		return 1;
}

int employee::generate_id_status = 1;

employee::employee(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp9, str inp10) : person(inp1, inp2, inp3, inp4, inp5), account(inp9, inp10)
{
	if(!generate_id_status)
	{
//		cerr<<"ID cannot be generated for this employee..";
	}
	else
	{
		employee::generate_id_status = generate_id();
		id_to_emp i1(id, OTHERS);
		salary = inp6;
		shift_start = inp7;
		shift_end = inp8;
	}
}

employee::employee() : person()
{
	id = 0;
}

int employee::get_age()
{
	////////////////Updating age to present age///////////
	set_dob(dob);		//This function is used here to invoke calc_age() in it only(because calc_age is directly not accessible)
	employee temp;
	if(hospital::get_employee_by_id(id, &temp))		//if employee's file exists on disk
		hospital::write_employee(this);				//overwrite that file
	return age;
}

unsigned long employee::get_salary(){
	return salary;
}

void employee::set_salary(unsigned long inp)
{
	salary = inp;
}

Time employee::get_shift(int inp){
	switch(inp){
		case START:
			return shift_start;
		case END:
			return shift_end;
		default:
			return Time(0,0,0);
	}
}

void employee::set_shift(int inp1, Time inp2)
{
	switch (inp1)
	{
		case START:
			shift_start = inp2;
			return;
		case END:
			shift_end = inp2;
			return;
		default:
			return;
	}
}

unsigned long employee::get_id()
{
	return id;
}

transaction * employee::get_last_10_transactions()
{
	transaction t[10];
	str temp;
	sprintf(temp, "employee/%d/trans.dat", id);
	ifstream fin ( temp ,ios::binary | ios::ate );
	fin.seekg(( (-10) * sizeof(transaction) ), ios::end);
	for(int i = 0; i < 10; ++i)
	{
		fin.read((char *) &t[i], sizeof(transaction));
	}
	return t;
}

int employee::get_generate_id_status()
{
	return generate_id_status;
}

/////////////////////////////////////////
//// Doctor, Nurse and Receptionist class member defs

doctor::doctor(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp10, int inp11, str inp12, str inp13) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp12, inp13)
{
	id_to_emp i1(get_id(), DOCTOR);
	speciality[0] = inp10;
	speciality[1] = inp11;

	for(int i = 0; i < 10; i++){
		patients[i] = 0;
	}
}

doctor::doctor() : employee()
{
	speciality[0] = speciality[1] = GEN + 1;	//storing an invalid value in speciality
	for(int i = 0; i < 10; ++i)
	{
		patients[i] = 0;
	}
}

int * doctor::get_speciality()
{
	return speciality;
}

long * doctor::get_patients()
{
	return patients;
}

nurse::nurse(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp10, inp11)
{
	id_to_emp i1(get_id(), NURSE);
	for(int i = 0; i < 5; i++){
		patients[i] = 0;
	}
}

nurse::nurse() : employee()
{
	for(int i = 0; i < 5; ++i)
	{
		patients[i] = 0;
	}
}

long * nurse::get_patients()
{
	return patients;
}

receptionist::receptionist(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp10, inp11)
{
	id_to_emp i1(get_id(), RECEPTIONIST);
}

receptionist::receptionist() : employee()
{}

/////////////////////////////////////////
//// Pharmacy function definitions


void pharmacy::sale(int pat_id, int code, int quantity){
	patient a = hospital::get_patient_by_id(pat_id);
	for(int i = 0; i < 50; i++){
		if(a.get_med(i,0) == code){
			quantity += a.get_med(i,1);
			a.set_med(i, code, quantity);
		}
		if(i>0){
			if(a.get_med(i, 0) == 0 &&
				a.get_med(i - 1, 0) != 0){
					a.set_med(i, code, quantity);
				}
		}
	}
}
/*
void pharmacy::purchase(int code, int quantity){
	medicine &a = get_med_by_code(code);
	str b = strcat( strcat("Medicine (", a.name) , ") Purchase" );
	hospital::deduct_money( (a.price * quantity) , b );
} */

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

void hospital::discharge_patient(int pat_id){
	patient temp = hospital::get_patient_by_id(pat_id);

	if(temp.get_id() == 0){
		// pass an error ---------------------------------------------------
	}


	//header.clear();

	str temp_string;
	sprintf(temp_string, "Discharging patient #%d :: %s", temp.get_id(), temp.get_name());
	//header << ui::centeralign << temp_string;

	str temp_path;
	sprintf(temp_path, "patient/%d/trans.dat", temp.get_id());
	ifstream patient_file ( temp_path , ios::in | ios::binary);

	long bal;

	int i = 1;

	transaction trans;

	while(!patient_file.eof()){
		patient_file.read( (char*) &trans , sizeof(transaction) );
		//body.clear();
		if(trans.amount != 0){
			str temp_string2;
			sprintf(temp_string2, "$%d", trans.amount);
		//	body << i << ". " << trans.reason << ui::rightalign << temp_string2;
		}
		bal += trans.amount;
	}

	i++;

	Date adm_date = Date(temp.get_admission_date(DAY),
			     temp.get_admission_date(MONTH),
			     temp.get_admission_date(YEAR) );

	int stay_length = hospital::get_date_difference(adm_date, system::get_date() );
	float stay_charge = 200 * (stay_length);

	str temp_string3;
	sprintf(temp_string3, "$%d", stay_charge);
	//body << i << ". " << "Stay for " << stay_length << " days" << ui::rightalign << temp_string3;


	//body << ui::endl << ui::rightalign << "Final amount : $" << bal;

	if(patient_file.fail()){
		// pass an error ---------------------------------------------------
	}

	temp.discharge();
	temp.set_discharge_date( system::get_date() );

	hospital::write_patient(temp);
}

medicine hospital::get_med_by_code(int inp_code){
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

////////////////////////////////////////////
//////// Function definitions for interface

void interface::init(){
	window.hide();
	window.display();
	window.settcolor(WHITE);
	window << ui::centeralign << "LHOSPITAL";
	window.settcolor(ui::tcolor);
	window.setfooter_tcolor(GREEN);

	Date current_date = system::get_date();
	Time current_time = system::get_time();

	str curr_date, curr_time;
	sprintf(curr_date, "%d/%d/%d", current_date.day, current_date.month, current_date.year);
	sprintf(curr_time, "%d:%d", current_time.hour, current_time.minute);

	window << box::setheader << curr_date << box::setheader << ui::rightalign 
			<< curr_time << box::setfooter << ui::centeralign
		    << "Everything looks OK";
	int id;
	do
	{
		id = interface::login_screen();
		if(id && id_to_emp::convert(id) != OTHERS || id == 1)	//so that general employees (except administrator) do
		{														// not accidentally login(as they have been assigned
			interface::clear_error();							// username and password as "", "")
			break;
		}
	}while(1);
	if(id == 1)	//if user logging in is administrator
	{
		int choice = 0;

		choice = interface::menu();

		switch(choice){
			case 1:
				interface::patient_management();
				break;
			case 2:
				interface::employee_management();
				break;
			case 3:
				interface::stock_management();
				break;
		}
	}
	else
	{
		switch(id_to_emp::convert(id))
		{
			case INVALID:
				interface::error("You have an invalid id generated. Create a new account");
				break;
			case DOCTOR:
				interface::doctor_screen();
				break;
			case NURSE:
				interface::nurse_screen();
				break;
			case RECEPTIONIST:
				interface::receptionist_screen();
				break;
		}
	}
}

int interface::login_screen()
{
	const int login_screen_height = 9;
	coord c(ui::scr_width / 3, ui::scr_height / 3);
	box login_box (c, ui::scr_width / 3, login_screen_height);

	str uid, pwd;

	login_box.settcolor_input(YELLOW);
	login_box << "User ID : ";
	login_box >> uid;
	login_box << ui::endl << "Password : ";
	login_box >> box::setpassword >> pwd;
	login_box << ui::endl;
	login_box.setexit_button("Login");
	login_box.loop();
	login_box.hide();
	unsigned long max_id;
	ifstream fin;
	fin.open("employee/max_id.dat", ios::binary);
	if(!fin)
		max_id = 1;
	else
	{
		fin.read((char *) &max_id, sizeof(unsigned long));
		if(fin.fail())
		{
			interface::error("ERROR WHILE READING FROM FILE!!! ");
			return 0;
		}
	}
	fin.close();
	str temp;
	for(unsigned long id = 1; id <= max_id; ++id)
	{
		employee x;
		if(!hospital::get_employee_by_id(id, &x))
		{
			str errmsg;
			sprintf(errmsg, "Error in reading file of id %lu", id);
			interface::error(errmsg);
			return 0;
		}
		if(!strcmp(x.account.get_username(), uid) && x.account.login(pwd))
		{
			interface::clear_error();
			return id;
		}
	}
	interface::error("Invalid login details!!");
	return 0;
}

int interface::menu(){
	coord c(ui::scr_width / 3, ui::scr_height / 3);
	box menu (c, ui::scr_width / 3, ui::scr_height / 2.2);

	int ch;
	menu << "1. Patient management"
			<< ui::endl << "2. Employee management"
			<< ui::endl << "3. Stock management"
			<< ui::endl << ui::endl << "Choice : ";
	menu.setdefault(1);
	menu.settcolor_input(YELLOW);
	validate_menu::set_menu_limits(1, 3);
	menu >> validate_menu::input >> ch;

	menu << ui::endl;
	menu.setexit_button("Submit");

	menu.loop();
	menu.hide();

	return ch;
}

void interface::patient_management(){/*
	int ch = 0;

	while(ch < 1 || ch > 3){
		coord c(ui::scr_width / 3, ui::scr_height / 3);
		box menu (c, ui::scr_width / 3, ui::scr_height / 2.2);

		menu << "1. Patient admission"
				<< ui::endl << "2. Patient discharge"
				<< ui::endl << "3. Edit patient details"
				<< ui::endl << ui::endl << "Choice : ";
		menu.setdefault(1);
		menu.settcolor_input(YELLOW);
		menu >> ch;

		menu << ui::endl;
		menu.setexit_button("Submit");

		menu.loop();
		menu.hide();
	}

	switch(ch){
		case 1:
		{*//*
			coord c(ui::scr_width / 2, ui::scr_height / 3);
			box form (c, ui::scr_width / 2, ui::scr_height / 1.5);
			form.settcolor_input(YELLOW);

			str inp_name, inp_sex_str, inp_dob_str
				, inp_phone, inp_guard_name, inp_emer_contact
				, inp_emer_phone, inp_insur_expiry, inp_admdate_str;

			address inp_adr;
			disease inp_dis;
			insurance inp_insur;

			form << "Enter data for the patient :" << ui::endl
					<< ui::endl << "Name : ";
			form >> inp_name;

			form << ui::endl << "Sex : ";
			form >> inp_sex_str;
			form << ui::endl << "Key - M/F/T = Male/Female/Trans"
					<< ui::endl << "Date of Birth : ";

			form.setdefault("25/12/1991");
			form >> inp_dob_str;


			form << ui::endl << "Address"
					<< ui::endl << ui::endl
					<< "\tHouse # : ";
			form.setdefault("221B");
			form >> inp_adr.house_no;

			form << ui::endl << "\tStreet : ";
			form.setdefault("Baker Street");
			form >> inp_adr.street;

			form << ui::endl << "\tDistrict : ";
			form.setdefault("Idk");
			form >> inp_adr.district;

			form << ui::endl << "/tState : ";
			form.setdefault("London(?)");
			form >> inp_adr.state;


			form << ui::endl << ui::endl
					<< "Phone : ";
			form.setdefault("1234567890");
			form >> inp_phone;


			form << ui::endl << "Disease"
					<< ui::endl << ui::endl
					<< "\tName : ";
			form.setdefault("Melanoma");
			form >> inp_dis.name;

			form << ui::endl << "Type : ";
			form.setdefault(0);
			form >> inp_dis.type;

			form << ui::endl << "\tType key : " << ui::endl
					<< "\t0 - Brain\t1 - Heart" << ui::endl
					<< "\t2 - Skin\t3 - Lung" << ui::endl
					<< "\t4 - Bone\t5 - Eye" << ui::endl
					<< "\t6 - Throat\t7 - Teeth" << ui::endl
					<< "\t8 - Stomach\t9 - Blood" << ui::endl
					<< "\t10 - General/full body condition"
					<< ui::endl << "\tSymptoms"
					<< ui::endl << "\tSymptom 1 : ";

			form >> inp_dis.symptoms[0];

			form << ui::endl << "\tSymptom 2 : ";
			form >> inp_dis.symptoms[1];

			form << ui::endl << "\tSymptom 3 : ";
			form >> inp_dis.symptoms[2];

			form << ui::endl << "\tSymptom 4 : ";
			form >> inp_dis.symptoms[3];


			form << ui::endl << ui::endl
					<< "Guardian name : ";
			form.setdefault("Dr. John Watson");
			form >> inp_guard_name;

			form << ui::endl << "Emergency Contact : ";
			form.setdefault("Irene Adler");
			form >> inp_emer_contact;

			form << ui::endl << "Emer. Cont. Phone : ";
			form.setdefault("1234567890");
			form >> inp_emer_phone;


			form << ui::endl << "Insurance"
					<< ui::endl << ui::endl
					<< "\tProvder : ";
			form.setdefault("LIC");
			form >> inp_insur.provider;

			form << ui::endl << "\tAmount ($) : ";
			form.setdefault(30000);
			form >> inp_insur.amount;

			form << ui::endl << "\tExpiry";
			form.setdefault("25/12/2022");
			form >> inp_insur_expiry;
			inp_insur.expiry = hospital::str_to_date(inp_insur_expiry);


			form << ui::endl << ui::endl
					<< "Admission Date : ";
			char dnow[11];
			Date current_date = system::get_date();
			sprintf(dnow, "%d/%d/%d", current_date.day
									, current_date.month
									, current_date.year);
			form.setdefault(dnow);
			form >> inp_admdate_str;

			form << ui::endl << ui::endl;
			form.setexit_button("Submit");

			form.loop();

			form.hide();

			patient temp_pat = patient(inp_name, hospital::str_to_sex(inp_sex_str)
										, hospital::str_to_date(inp_dob_str), inp_adr
										, inp_phone, inp_dis, inp_guard_name
										, inp_emer_contact, inp_emer_phone
										, inp_insur, hospital::str_to_date(inp_admdate_str));

			hospital::write_patient(temp_pat);
		*//*break;
		}

		case 2:
		{}

		case 3:
		{
			int choice;

			while(choice < 1 || choice > 5){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box menu (c, ui::scr_width / 3, ui::scr_height / 1.5);

				menu << "Choose item to edit:"
						<< ui::endl << "1. Disease/condition"
						<< ui::endl << "2. Guardian name"
						<< ui::endl << "3. Emergency contact"
						<< ui::endl << "4. Emergency contact no."
						<< ui::endl << "5. Insurance information"
						<< ui::endl << ui::endl << "Choice : ";
				menu.setdefault(1);
				menu.settcolor_input(YELLOW);
				menu >> choice;

				menu << ui::endl;
				menu.setexit_button("Submit");

				menu.loop();
				menu.hide();
			}
			patient temp_patient;
			switch(choice){
				case 1:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 1.5);

					edit_screen <<  "Enter disease/condition for " << temp_patient.get_name()
								<< ui::endl << "Disease : ";
					disease temp = temp_patient.get_dis();
					edit_screen.setdefault(temp.name);
					edit_screen >> temp.name;
					edit_screen << ui::endl << "Type : ";
					edit_screen.setdefault(temp.type);
					edit_screen >> temp.type;
					edit_screen << ui::endl << "Type key : " << ui::endl
								<< "0 - Brain\t1 - Heart" << ui::endl
								<< "2 - Skin\t3 - Lung" << ui::endl
								<< "4 - Bone\t5 - Eye" << ui::endl
								<< "6 - Throat\t7 - Teeth" << ui::endl
								<< "8 - Stomach\t9 - Blood" << ui::endl
								<< "10 - General/full body condition"
								<< ui::endl << ui::endl
								<< "Symptoms" << ui::endl
								<< "Symptom 1 : ";
					edit_screen.setdefault(temp.symptoms[0]);
					edit_screen >> temp.symptoms[0];
					edit_screen << ui::endl << "Symptom 2 : ";
					edit_screen.setdefault(temp.symptoms[1]);
					edit_screen >> temp.symptoms[1];
					edit_screen << ui::endl << "Symptom 3 : ";
					edit_screen.setdefault(temp.symptoms[2]);
					edit_screen >> temp.symptoms[2];
					edit_screen << ui::endl << "Symptom 4 : ";
					edit_screen.setdefault(temp.symptoms[3]);
					edit_screen >> temp.symptoms[3];

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp_patient.set_dis(temp);
					hospital::write_patient(temp_patient);

					break;
				}

				case 2:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 3);

					edit_screen <<  "Enter name of guardian for " << temp_patient.get_name()
								<< ui::endl << "Guardian Name : ";
					str temp;
					edit_screen.setdefault(temp_patient.get_guardian_name());
					edit_screen >> temp;

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp_patient.set_guardian_name(temp);
					hospital::write_patient(temp_patient);

					break;
				}

				case 3:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 3);

					edit_screen <<  "Enter emergency contact no. for " << temp_patient.get_name()
								<< ui::endl << "Contact no. : ";
					str temp;
					edit_screen.setdefault(temp_patient.get_emergency_contact());
					edit_screen >> temp;

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp_patient.set_emergency_contact(temp);
					hospital::write_patient(temp_patient);

					break;
				}

				case 4:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 3);

					edit_screen <<  "Enter emergency contact no. for " << temp_patient.get_name()
								<< ui::endl << "Contact no. : ";
					phone temp;
					edit_screen.setdefault(temp_patient.get_emer_contact_no());
					edit_screen >> temp;

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp_patient.set_emer_contact_no(temp);
					hospital::write_patient(temp_patient);

					break;
				}

				case 5:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 3);

					edit_screen <<  "Enter insurance information for " << temp_patient.get_name()
								<< ui::endl << "Provider : ";
					insurance temp = temp_patient.get_insur_info();
					edit_screen.setdefault(temp.provider);
					edit_screen >> temp.provider;
					edit_screen << ui::endl << "Amount (in $) :";
					edit_screen.setdefault(temp.amount);
					edit_screen >> temp.amount;
					edit_screen << ui::endl << "Expiry date (MM/DD/YYYY):";
					char temp_date[11];
					edit_screen >> hospital::date_validity >> temp_date;

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp.expiry = hospital::str_to_date(temp_date);
					temp_patient.set_insur_info(temp);
					hospital::write_patient(temp_patient);

					break;
				}

			}

			break;
		}
	}

*/}

void interface::employee_management()
{
	/*
	1. View employee data
	2. Add new employee
	3. Remove existing employee
	4. Edit employee data
	5. Pay salary to individual employee
	6. Pay salary to all employees
	7. Back
	*/
	const int menu_corner_top_left_y = 5;
	coord c(ui::scr_width * 0.2, menu_corner_top_left_y);
	int ch;
	box menu(c, ui::scr_width * 0.6, ui::scr_height - 6 );
	menu.settcolor(GREEN);
	menu << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	menu.settcolor(ui::tcolor);
	menu << "1. View employee data" << ui::endl
		 << "2. Add new employee" << ui::endl
		 << "3. Remove existing employee" <<ui::endl
		 << "4. Edit employee data" <<ui::endl
		 << "5. Pay salary to individual employee" <<ui::endl
		 << "6. Pay salary to all employees" <<ui::endl
		 << "7. Back" <<ui::endl
		 << ui::endl <<"Enter your choice: ";
	menu.settcolor_input(YELLOW);
	validate_menu::set_menu_limits(1, 7);
	menu >> validate_menu::input >> ch;
	menu << ui::endl;
	menu.setexit_button("Submit");
	menu.loop();
	menu.hide();
	box menu2(c, ui::scr_width * 0.6, ui::scr_height - 6 );
	menu2.clear();
	menu2.settcolor(GREEN);
	menu2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	menu2.settcolor(WHITE);
	int menu2_height;
	switch (ch)
	{
		case 1:
		{
			menu2_height = 10;
			menu2.setheight(menu2_height);
			menu2 << "View employee data" << ui::endl;
			menu2.settcolor(ui::tcolor);
			menu2 << "Enter employee's id: ";
			unsigned long id;
			void * temp;
			menu2.settcolor_input(YELLOW);
			menu2 >> id;
			menu2 << ui::endl;
			menu2.setexit_button("Submit");
			menu2.loop();
			menu2.hide();
			if(!hospital::get_employee_by_id(id, temp))
			{
				interface::error("Error while reading from file!");
			}
			else
			{
				employee *e = (employee *) temp;
				box menu3( menu2.getcorner_top_left(), menu2.getwidth(), menu2.getheight() );
				menu3.setheight( menu.getheight() );
				menu3.settcolor(GREEN);
				menu3 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
				menu3.settcolor(WHITE);
				menu3 << "Employee Details: " << ui::endl;
				menu3.settcolor(ui::tcolor);
				menu3 << "ID: " << e->get_id() << ui::endl;
				menu3 << "Name: " << e->get_name() << ui::endl;
				menu3 << "Age: " << e->get_age() << ui::endl;
				menu3 << "Sex: " << (sex)e->get_sex() << ui::endl;
				menu3 << "Date of Birth: " << e->get_dob() << ui::endl;
				menu3 << "Address: " << e->get_address() << ui::endl;
				menu3 << "Phone no.: " << e->get_phone() << ui::endl;
				menu3 << "Salary: " << e->get_salary() << ui::endl;
				menu3 << "Shift timings: Starts - " << e->get_shift(START) << ui::endl;
				menu3 << "-------------: Ends - " << e->get_shift(END) <<ui::endl;
				switch( id_to_emp::convert( e->get_id() ) )
				{
					case INVALID:	//Test this case, menu3.hide() not working properly
					{
						menu3.clear();
						int menu3_height = 9;
						menu3.setheight(menu3_height);
						menu3.settcolor(GREEN);
						menu3 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
						menu3.settcolor(WHITE);
						menu3 << "Employee Details: " << ui::endl;
						menu3.settcolor(RED);
						menu3 << "Invalid ID!!" << id_to_emp::convert( e->get_id() );
						menu3.settcolor(ui::tcolor);
						menu3.setexit_button("Back");
						menu3.loop();
						menu3.hide();
						break;
					}
					case OTHERS:
					case RECEPTIONIST:	//there are no extra data members in class receptionist
					{
						menu3.setexit_button("Back");
						menu3.loop();  //	menu3.clear();	int w = window.getwidth(), m = menu3.getwidth(); menu3<<w<<' '<<m;	getch();
						menu3.hide();
						break;
					}
					case DOCTOR:
					{
						doctor *d = (doctor *)temp;
						menu3.hide();
						menu3.setcorner_top_left( coord( 1, menu3.getcorner_top_left().y ) );
						menu3.display();
						menu3.f << ( ui::top | ui::left ) << (char)204
								<< ( ui::bottom | ui::left ) << (char)204;
						menu3.f.display();
						box sidemenu( menu3.getcorner_top_left() + coord( menu3.getwidth() - 1, 0 ), ( ui::scr_width - menu3.getwidth() + 1 ),  menu3.getheight() );
						sidemenu.f << ( ui::top | ui::left ) << (char)203
								   << ( ui::bottom | ui::left ) << (char)202
								   << ( ui::top | ui::right ) << (char)185
								   << ( ui::bottom | ui::right ) << (char)185;
						sidemenu.f.display();
						sidemenu << "Speciality(s)" << ui::endl;
						for(int i = 0; i < 2 && d->get_speciality()[i] <= GEN; ++i)
						{
							sidemenu << i + 1 << ". " << (body_parts)d->get_speciality()[i] << ui::endl;
						}
						if(!i)
						{
							sidemenu << "None" << ui::endl;
						}
						sidemenu << "Patients currently under care:" << ui::endl;
						for(i = 0; d->get_patients()[i] && i < 10; ++i)
						{
							sidemenu << i + 1 << ". " << hospital::get_patient_by_id( d->get_patients()[i] ).get_name() << ui::endl;
						}
						if(!i)
						{
							sidemenu << "None" << ui::endl;
						}
						menu3.loop();
						menu3.hide();
						sidemenu.hide();
						window.f.display();
						break;
					}
					case NURSE:
					{
						nurse *n = (nurse *)temp;
						menu3.hide();
						menu3.setcorner_top_left( coord( 1, menu3.getcorner_top_left().y ) );
						menu3.display();
						menu3.f << ( ui::top | ui::left ) << (char)204
								<< ( ui::bottom | ui::left ) << (char)204;
						menu3.f.display();
						box sidemenu( menu3.getcorner_top_left() + coord( menu3.getwidth() - 1, 0 ), ( ui::scr_width - menu3.getwidth() + 1 ),  menu3.getheight() );
						sidemenu.f << ( ui::top | ui::left ) << (char)203
								   << ( ui::bottom | ui::left ) << (char)202
								   << ( ui::top | ui::right ) << (char)185
								   << ( ui::bottom | ui::right ) << (char)185;
						sidemenu.f.display();
						
						break;
					}
				}
			}
			break;
		}
		case 2:
		{
			menu2 << "Add new employee" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 3:
		{
			menu2 << "Remove existing employee" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 4:
		{
			menu2 << "Edit employee data" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 5:
		{
			menu2 << "Pay salary to individual employee" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 6:
		{
			menu2 << "Pay salary to all employees" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 7:
		{
			menu2 << "This program will exit now. You wanna go back, you're gonna have to start the program again." << ui::endl;
			break;
		}
	}
}

void interface::stock_management(){
	coord c(ui::scr_width / 3, ui::scr_height / 3);
	box menu (c, ui::scr_width / 3, ui::scr_height / 2.2);

	int ch = 0;

	while (ch < 1 || ch > 3){
		menu << "1. Sale"
				<< ui::endl << "2. Purchase"
				<< ui::endl << "3. Stock check"
				<< ui::endl << ui::endl << "Choice : ";
		menu.setdefault(1);
		menu.settcolor_input(YELLOW);
		menu >> ch;

		menu << ui::endl;
		menu.setexit_button("Submit");

		menu.loop();
		menu.hide();

		interface::error("Invalid choice!!");
	}

	interface::clear_error();

	switch(ch){
		case 1:
		{

			medicine temp;
			temp.code = 0;

			while(temp.code == 0){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box sale_menu (c, ui::scr_width / 3, ui::scr_height / 3);
				sale_menu.settcolor_input(YELLOW);
				sale_menu << ui::centeralign << "Medicine Sale";
				sale_menu << "Code : ";
				sale_menu.setdefault(42);
				sale_menu >> temp.code;
				sale_menu << ui::endl;
				sale_menu.setexit_button("Submit");
				sale_menu.loop();
				sale_menu.hide();

				temp = hospital::get_med_by_code(temp.code);
			}
			
			int quantity = -2;
			patient temp_patient;
			long pat_id;

			while(quantity < 0 || quantity > 100){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box sale_menu (c, ui::scr_width / 3, ui::scr_height / 2);
				sale_menu.settcolor_input(YELLOW);
				sale_menu << ui::centeralign << "Medicine Sale";
				sale_menu << "Name : " << temp.name
							<< ui::endl << "Price : $" << temp.price
							<< ui::endl << ui::endl
							<< "Patient ID : ";
				sale_menu.setdefault(786);
				sale_menu >> pat_id;
				sale_menu << ui::endl << "Quantity : ";
				sale_menu.setdefault(1);
				sale_menu >> quantity;
				sale_menu.setexit_button("Submit");
				sale_menu.loop();
				sale_menu.hide();

				temp_patient = hospital::get_patient_by_id(pat_id);
				if(temp_patient.get_id() == 0){
					quantity = -1;
					interface::error("Invalid patient ID!!");
					continue;
				}
				interface::error("Invalid quantity!!");
			}

			interface::clear_error();

			temp.stock -= quantity;

			for(int i = 0; i < 50; i++){
				if(temp_patient.get_med(i, 0) == temp.code ||
						temp_patient.get_med(i,0) == 0){
							temp_patient.set_med(i, temp.code, temp_patient.get_med(i, 1) + quantity);
						}
			}

			hospital::write_patient(temp_patient);		
			//hospital::write_med(temp);

			break;
		}

		case 2:
		{
			medicine temp;
			temp.code = 0;

			while(temp.code == 0){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box purchase_menu (c, ui::scr_width / 3, ui::scr_height / 3);
				purchase_menu.settcolor_input(YELLOW);
				purchase_menu << ui::centeralign << "Medicine Purchase";
				purchase_menu << "Code : ";
				purchase_menu.setdefault(42);
				purchase_menu >> temp.code;
				purchase_menu << ui::endl;
				purchase_menu.setexit_button("Submit");
				purchase_menu.loop();
				purchase_menu.hide();

				temp = hospital::get_med_by_code(temp.code);
			}

			int quantity;

			while(quantity < 0 || quantity > 5000){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box purchase_menu (c, ui::scr_width / 3, ui::scr_height / 2);
				purchase_menu.settcolor_input(YELLOW);
				purchase_menu << ui::centeralign << "Medicine Sale";
				purchase_menu << "Name : " << temp.name
							<< ui::endl << "Price : $" << temp.price
							<< ui::endl << ui::endl << "Quantity : ";
				purchase_menu.setdefault(1);
				purchase_menu >> quantity;
				purchase_menu.setexit_button("Submit");
				purchase_menu.loop();
				purchase_menu.hide();

				interface::error("Invalid quantity!!");
			}

			interface::clear_error();

			temp.stock += quantity;
			hospital::deduct_money(temp.price * quantity, "Medicine purchase", system::get_date(), system::get_time());
			//hospital::write_med(temp);
		}
		
		case 3:
		{
			medicine temp;
			temp.code = 0;

			while(temp.code == 0){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box stock_menu (c, ui::scr_width / 3, ui::scr_height / 3);
				stock_menu.settcolor_input(YELLOW);
				stock_menu << ui::centeralign << "Medicine Sale";
				stock_menu << "Code : ";
				stock_menu.setdefault(42);
				stock_menu >> temp.code;
				stock_menu << ui::endl;
				stock_menu.setexit_button("Submit");
				stock_menu.loop();
				stock_menu.hide();

				temp = hospital::get_med_by_code(temp.code);
			}

			coord c(ui::scr_width / 3, ui::scr_height / 3);
			box stock_menu (c, ui::scr_width / 3, ui::scr_height / 2);
			stock_menu.settcolor_input(YELLOW);
			stock_menu << ui::centeralign << "Medicine Details";
			stock_menu << "Name : " << temp.name
						<< ui::endl << "Price : $" << temp.price
						<< ui::endl << "Dosage : " << temp.dosage << " ml"
						<< ui::endl << "Quantity in stock : " << temp.stock
						<< ui::endl;
			stock_menu.setexit_button("Okay");
			stock_menu.loop();
			stock_menu.hide();
		}
	}

}

void interface::doctor_screen()
{
	coord c(1, 4);
	box profile(c, (ui::scr_width * 3 / 5), ui::scr_height - 5);
	box menu(( c + coord((ui::scr_width * 3 / 5) - 1, 0)), (ui::scr_width * 2 / 5) + 1, ui::scr_height - 5);
	profile.f << ( ui::top | ui::left ) << (char)204
			  << ( ui::bottom | ui::left ) << (char)204
			  << ( ui::top | ui::right ) << (char)203
			  << ( ui::bottom | ui::right ) << (char)202;
	profile.f.display();
	menu.f << ( ui::top | ui::left ) << (char)203
		   << ( ui::bottom | ui::left ) << (char)202
		   << ( ui::top | ui::right ) << (char)185
		   << ( ui::bottom | ui::right ) << (char)185;
	menu.f.display();
	profile.settcolor(GREEN);
	profile << ui::centeralign << "Personal Details" << ui::endl;
	profile.settcolor(ui::tcolor);
	profile << "Name: ";
	profile.setexit_button("yay");	profile.loop();menu << "yay";
	menu.hide();
	profile.hide();
	window.f.display();
}
void interface::nurse_screen()
{
	coord c(1, 4);
	box profile(c, (ui::scr_width * 3 / 5), ui::scr_height - 5);
	box menu(( c + coord((ui::scr_width * 3 / 5) - 1, 0)), (ui::scr_width * 2 / 5) + 1, ui::scr_height - 5);
	profile.f << ( ui::top | ui::left ) << (char)204
			  << ( ui::bottom | ui::left ) << (char)204
			  << ( ui::top | ui::right ) << (char)203
			  << ( ui::bottom | ui::right ) << (char)202;
	profile.f.display();
	menu.f << ( ui::top | ui::left ) << (char)203
		   << ( ui::bottom | ui::left ) << (char)202
		   << ( ui::top | ui::right ) << (char)185
		   << ( ui::bottom | ui::right ) << (char)185;
	menu.f.display();
	profile.settcolor(GREEN);
	profile << ui::centeralign << "Personal Details" << ui::endl;
	profile.settcolor(ui::tcolor);
	profile << "Name: ";
	profile.setexit_button("yay");	profile.loop();menu << "yay";
	menu.hide();
	profile.hide();
	window.f.display();
}
void interface::receptionist_screen()
{
	coord c(1, 4);
	box profile(c, (ui::scr_width * 3 / 5), ui::scr_height - 5);
	box menu(( c + coord((ui::scr_width * 3 / 5) - 1, 0)), (ui::scr_width * 2 / 5) + 1, ui::scr_height - 5);
	profile.f << ( ui::top | ui::left ) << (char)204
			  << ( ui::bottom | ui::left ) << (char)204
			  << ( ui::top | ui::right ) << (char)203
			  << ( ui::bottom | ui::right ) << (char)202;
	profile.f.display();
	menu.f << ( ui::top | ui::left ) << (char)203
		   << ( ui::bottom | ui::left ) << (char)202
		   << ( ui::top | ui::right ) << (char)185
		   << ( ui::bottom | ui::right ) << (char)185;
	menu.f.display();
	profile.settcolor(GREEN);
	profile << ui::centeralign << "Personal Details" << ui::endl;
	profile.settcolor(ui::tcolor);
	profile << "Name: ";
	profile.setexit_button("yay");	profile.loop();menu << "yay";
	menu.hide();
	profile.hide();
	window.f.display();
}

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

void interface::error(char* err){
	window.clear_footer();
	window.setfooter_tcolor(RED);
	window << box::setfooter << ui::centeralign
		<< err;
}

void interface::clear_error(){
	window.clear_footer();
	window.setfooter_tcolor(GREEN);
	window << box::setfooter << ui::centeralign
		<< "Everything looks OK";
}

int interface::log_this(char * message)
{
	Date dnow = system::get_date();
	Time tnow = system::get_time();
	str text;
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

box interface::window;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

double hospital::balance = 1000.0;

void main()
{
	clrscr();
/*/////Administrator object creator////////////////
	address yay = {0, 0, 0, 0, 0};
	employee x("Administrator", 3, Date(), yay, "", 0, Time(), Time(), "admin", "password");
	hospital::write_employee(&x);
/////////////////////////////////////////////*/
	interface::log_this("Program initiated\n-----------------------------------\n");
	interface::init();
	interface::log_this("Program terminated\n-----------------------------------\n");
	getch();
}