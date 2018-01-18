#ifndef EMP
#define EMP

#include "base.hpp"

enum emp_type {INVALID, OTHERS, DOCTOR, NURSE, RECEPTIONIST};

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

class id_to_emp
{
		unsigned long id;
		int employee_type;
	public:
		id_to_emp(unsigned long, int);
		id_to_emp();
		static int convert(unsigned long);
};

#endif