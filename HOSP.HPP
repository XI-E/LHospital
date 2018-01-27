#ifndef HOSP
#define HOSP

#include "base.hpp"
#include "patient.hpp"

const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int stay_charge = 50;  // $50 per day

class hospital
{
	public:
		static float get_bal();
		static transaction deduct_money(float, char*, Date, Time);
		static transaction add_money(float, char*, Date, Time);
		static transaction* get_transaction();

		static patient get_patient_by_id(long);
		static void write_patient(patient);
		static void charge_patient(int, transaction);
		static void discharge_patient(patient);
		static float calc_bill(int);

		static medicine get_med_by_code(int);
		static void write_med(medicine);

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
		static int str_to_sex(char *);

	private:
		hospital();
		static int read_from(unsigned long, char *, int, char *);
		static double balance;
};

class pharmacy{
	public:
		static void sale(int, int, int);
		static void purchase(int, int);
};

#endif