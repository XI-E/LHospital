#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <dir.h>
#include "base.hpp"
#include "patient.hpp"

//enum access_level {OTHERS, DOCTOR, NURSE, RECEPTIONIST};

class employee : public person{
	public:
	employee(str, int, Date, address, phone, unsigned long, Time, Time, int, str = "", str = "");  //for all those with user accounts(doctors, nurses, receptionists), last 2 arguments are to be provided as well
	unsigned long get_salary();
	Time get_shift(int);
	transaction * get_last_10_transactions();
	void set_last_10_transactions(transaction *);
	userid account;
	protected:
		unsigned id;
		unsigned long salary;
		transaction last_10_transactions[10];
		Time shift_start;
		Time shift_end;
};

class doctor : public employee{
	public:
	doctor(str, int, Date, address, phone, unsigned long, Time, Time, int, int, int, str, str);

	private:
		int speciality[2];				// Doctor's specialization
		int patient[10];				// Patients currently under care, can take only 10 at once
};

class nurse : public employee{
	public:
	nurse(str, int, Date, address, phone, unsigned long, Time, Time, int, str, str);

	private:
		int patient[5];
};

class receptionist : public employee
{
	public:
		receptionist(str, int, Date, address, phone, unsigned long, Time, Time, int, str, str);
//		doctor assign_doctor(patient);
};

class pharmacy{
	public:
		static void sale(int, int, int);
		static void purchase(int, int);
};

class hospital{
	public:
		static float get_bal();
		static transaction deduct_money(float, char*, Date, Time);
		static transaction add_money(float, char*, Date, Time);
		static transaction* get_transaction();
		static void read_balance();

		static patient get_patient_by_id(int);
		static patient write_patient(patient);
		static void charge_patient(int, transaction);

		static void pay_salary(employee, Date, Time);
//		static void pay_all_salaries();
	private:
		hospital();
		static double balance;

};

//////////////////////////////////
/// Function definitions for class employee

employee::employee(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : person(inp1, inp2, inp3, inp4, inp5), account(inp10, inp11)
{
	salary = inp6;
	shift_start = inp7;
	shift_end = inp8;
	id = inp9;

	transaction null;
	for(int i = 0; i < 10; ++i)
		 last_10_transactions[i] = null;
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

transaction * employee::get_last_10_transactions()
{
	return last_10_transactions;
}

void employee::set_last_10_transactions(transaction * t)
{
	for(int i = 0; i < 10; ++i)
		last_10_transactions[i] = t[i];
}
/////////////////////////////////////////
//// Doctor, Nurse and Receptionist class member defs

doctor::doctor(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, int inp10, int inp11, str inp12, str inp13) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp9, inp12, inp13)
{
	speciality[0] = inp10;
	speciality[1] = inp11;

	for(int i = 0; i < 10; i++){
		patient[i] = 0;
	}
}

nurse::nurse(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp9, inp10, inp11)
{
	for(int i = 0; i < 5; i++){
		patient[i] = 0;
	}
}

receptionist::receptionist(str inp1, int inp2, Date inp3, address inp4, phone inp5, unsigned long inp6, Time inp7, Time inp8, int inp9, str inp10, str inp11) : employee(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp9, inp10, inp11)
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
	sprintf(temp, "patient/%d/base.dat", a.id);
	sprintf(temp2, "patient/%d", a.id);
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
	sprintf(temp, "patient/%d/trans.dat", temp_pat.id);
	ofstream patient_file ( temp , ios::out | ios::binary | ios::app );
	patient_file.write( (char*) &trans , sizeof(transaction) );
	patient_file.close();

	hospital::write_patient(temp_pat);
}

void hospital::discharge_patient(int pat_id){
	patient temp = hospital::get_patient_by_id(pat_id);

	if(temp.id == 0){
		// pass an error ---------------------------------------------------
	}

	
	header.clear();

	str temp_string;
	sprintf(temp_string, "Discharging patient #%d :: %s", temp.id, temp.name);
	header << ui::centeralign << temp_string;

	str temp_path;
	sprintf(temp_path, "patient/%d/trans.dat", temp.id);
	ofstream patient_file ( temp_path , ios::out | ios::binary | ios::app );

	long bal;
	
	int i = 1;

	while(!patient_file.eof()){
		patient_file.read( (char*) &trans , sizeof(transaction) );
		body.clear();
		if(trans.amount != 0){
			str temp_string2;
			sprintf(temp_string2, "$%d", trans.amount);
			body << ui::leftalign << i << ". " << trans.reason << ui::rightalign << temp_string2;
		}
		bal += trans.amount;
	}

	i++;
	
	int stay_length = get_date_difference(temp.admission_date, temp.discharge_date)
	float stay_charge = 200 * (stay_length);

	str temp_string3;
	sprintf(temp_string2\3, "$%d", stay_charge);
	body << ui::leftalign << i << ". " << "Stay for " << stay_length << " days" << ui::rightalign << temp_string3;


	body << ui::endl << ui::rightalign << "Final amount : $" << bal;

	if(patient_file.fail()){
		// pass an error ---------------------------------------------------
	}

	temp.discharged = 1;
	temp.discharge_date = system::get_date();
}

void hospital::pay_salary(employee x, Date d1, Time t1)
{
	 unsigned long inp1 = x.get_salary();
	 char inp2[100] = "Salary paid to ";
	 strcat(inp2, x.get_name());
	 transaction t = hospital::deduct_money(inp1, inp2, d1, t1);
	 for(int i = 0; i < 10; ++i)
	 {
		  if( ( *(x.get_last_10_transactions() + i) ).amount == 0 &&
			  !strcmp( ( *(x.get_last_10_transactions() + i) ).reason, "NA") &&
			  ( *(x.get_last_10_transactions() + i) )._date.day == 0 &&
			  ( *(x.get_last_10_transactions() + i) )._date.month == 0 &&
			  ( *(x.get_last_10_transactions() + i) )._date.year == 0 &&
			  ( *(x.get_last_10_transactions() + i) )._time.hour == 0 &&
			  ( *(x.get_last_10_transactions() + i) )._time.minute == 0 &&
			  ( *(x.get_last_10_transactions() + i) )._time.second == 0		)
		  {
				*(x.get_last_10_transactions() + i) = t;
				return;
		  }
	 }
	 for(i = 0; i < 10; ++i)
	 {
		  *(x.get_last_10_transactions() + i) = *(x.get_last_10_transactions() + i + 1);
	 }
	 *(x.get_last_10_transactions() + i) = t;
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

int count_leap_years(Date d)
{
	int years = d.year;

	if (d.month <= 2){
		years--;		// checking whether to count the current year
	}

	return (years / 4) - (years / 100) + (years / 400);
}

int get_date_difference(Date dt1, Date dt2)
{

	long int n1 = dt1.year*365 + dt1.day;

	for (int i=0; i<dt1.month - 1; i++){
		n1 += monthDays[i];
	}
	n1 += count_leap_years(dt1);
		
	long int n2 = dt2.year*365 + dt2.day;
	
	for (int i=0; i<dt2.month - 1; i++){
			n2 += monthDays[i];
	}
	n2 += count_leap_years(dt2);

	return (n2 - n1);
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
double hospital::balance = 1000.0;
void main()
{

}