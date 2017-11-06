#include <fstream.h>
#include <string.h>
#include <dir.h>
#include <stdio.h>
#include "patient.hpp"
#include "ui/ui.hpp"

enum emp_type {INVALID, OTHERS, DOCTOR, NURSE, RECEPTIONIST};

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
		unsigned long get_salary();
		Time get_shift(int);
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
	private:
		int speciality[2];				// Doctor's specialization
		int patients[10];				// Patients currently under care, can take only 10 at once
};

class nurse : public employee{
	public:
		nurse(str, int, Date, address, phone, unsigned long, Time, Time, str, str);
		nurse();		//default constructor
	private:
		int patients[5];
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

		static int get_employee_by_id(unsigned long, employee &);
		static int get_employee_by_id(unsigned long, doctor &);
		static int get_employee_by_id(unsigned long, nurse &);
		static int get_employee_by_id(unsigned long, receptionist &);
		static int write_employee(employee);
		static int write_employee(doctor);
		static int write_employee(nurse);
		static int write_employee(receptionist);
		static int pay_salary(unsigned long, Date, Time);
		static int pay_all_salaries();

		static int get_date_difference(Date, Date);
		static int count_leap_years(Date);

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
//		cerr<<"File id_list.dat not found!!";
		return INVALID;
	}
	fin.seekg( (ID * sizeof(id_to_emp)) );
	fin.read((char *) &a, sizeof(id_to_emp));
	if(fin.fail())
	{
//		cerr<<"Error while reading from file id_list.dat";
		return INVALID;
	}
	fin.close();
	if(a.id != ID)
	{
//		cerr<<"(For dev only)Error in the code... Recheck it!!";
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

unsigned long employee::get_salary(){
	return salary;
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
{}

nurse::nurse(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp10, inp11)
{
	id_to_emp i1(get_id(), NURSE);
	for(int i = 0; i < 5; i++){
		patients[i] = 0;
	}
}

nurse::nurse() : employee()
{}

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
		//	body << ui::leftalign << i << ". " << trans.reason << ui::rightalign << temp_string2;
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
	//body << ui::leftalign << i << ". " << "Stay for " << stay_length << " days" << ui::rightalign << temp_string3;


	//body << ui::endl << ui::rightalign << "Final amount : $" << bal;

	if(patient_file.fail()){
		// pass an error ---------------------------------------------------
	}

	temp.discharge();
	temp.set_discharge_date( system::get_date() );

	hospital::write_patient(temp);
}

int hospital::get_employee_by_id(unsigned long ID, employee &target)
{
	employee null;
	str temp;
	sprintf(temp, "employee/%lu/base.dat", ID);
	int i = hospital::read_from( ID, (char*) &target, sizeof(employee), temp );
	if(!i)
	{
		target = null;
		return 0;
	}
	return 1;
}

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

int hospital::write_employee(employee a)
{
	mkdir("employee");
	str temp;
	sprintf(temp, "employee/%lu", a.get_id());
	mkdir(temp);
	strcat(temp, "/base.dat");
	ofstream fout ( temp , ios::out | ios::binary);
	if(!fout)
	{
//		cerr<<"Employee data file could not be created or accessed";
		return 0;
	}
	fout.write( (char*) &a , sizeof(employee) );
	if(fout.fail())
	{
//		cerr<<"Error while writing to file";
		return 0;
	}
	return 1;
}

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

int hospital::pay_salary(unsigned long id, Date d1, Time t1)
{
	employee x;
	int i = hospital::get_employee_by_id(id, x);
	if(!i)
	{
//		cerr<<"Employee not found or error while reading file";
		return 0;
	}
	unsigned long inp1 = x.get_salary();
	char inp2[100] = "Salary paid to ";
	strcat(inp2, x.get_name());
	transaction t = hospital::deduct_money(inp1, inp2, d1, t1);

	str temp;
	sprintf(temp, "employee/%lu", x.get_id());
	mkdir(temp);
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

const int monthDays[12] = {31, 28, 31, 30, 31, 30,
	31, 31, 30, 31, 30, 31};

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

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
double hospital::balance = 1000.0;
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
void main()
{}