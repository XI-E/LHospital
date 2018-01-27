/*!
 \file HOSP.HPP
 \brief Contains prototypes of the hospital management functions
*/

#ifndef HOSP
#define HOSP

#include "base.hpp"
#include "patient.hpp"

const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int stay_charge = 50;  // $50 per day

//!Class containing all the basic hospital management functions used in the program
class hospital
{
	public:
		//!@{Hospital finances management functions
		static float get_balance();		//!<Getter function
		
		//!Deducts the input amount from hospital::balance
		/*!
		\param amt The amount to be deducted from hospital::balance
		\param reason The reason for deduction of money
		\param dt The date of deduction of money
		\param tm The time of deduction of money
		\return A transaction type variable containing details about the amount deduction
		*/
		static transaction deduct_money(float amt, char* reason, Date dt, Time tm);
		
		//!Adds the input amount from hospital::balance
		/*!
		\param amt The amount to be added from hospital::balance
		\param reason The reason for addition of money
		\param dt The date of addition of money
		\param tm The time of addiction of money
		\return A transaction type variable containing details about the amount addition
		*/
		static transaction add_money(float, char*, Date, Time);
		
		//!Returns the last 10 transactions of the hospital
		/*!
		Reads the last 10 records from a file transactions.dat
		\return An array of type transaction containing those 10 records
		*/
		static transaction* get_transaction();
		//!}@
		
		//!@{Patient management functions
		
		//!Gets a patient object from file corresponding to an input id
		/*!
		Reads a patient object from a file base.dat that is present in 
		a folder with name as the id of the patient. All such folders are 
		present in a folder named PATIENT
		\param id The id of the patient to be read
		\return The patient object read from file
		*/
		static patient get_patient_by_id(long id);
		
		//!Writes a patient object to file
		/*!
		Makes a new folder(if it doesn't exist) in PATIENT, whose name is the 
		id of the patient object that is to be written, and writes the patient
		object to a file BASE.DAT inside that folder
		\param a The patient object that is to be written to file
		*/
		static void write_patient(patient a);
		
		//!Charges a patient for any service, treatment etc. that the patient availed from the hospital
		/*!
		Gets the patient object from file using get_patient_by_id(), and adds a 
		transaction type variable to a file TRANS.DAT that is present in the folder
		of the patient(i.e the folder having name as the id of the patient)
		\param pat_id ID of the patient
		\param trans The transaction type variable containing details of the transaction that is charged to the patient
		*/
		static void charge_patient(int pat_id, transaction trans);
		
		//!Discharges a patient from the hospital
		/*!
		Basically calls the patient::discharge() function, sets the patient's discharge
		date to the current system date, and writes the patient back to file
		\param temp The object of the patient who is to be discharged
		*/
		static void discharge_patient(patient temp);
		static float calc_bill(int); //!>Calculates the cost of the patient's stay in the hospital
		//!}@

		//!@{Functions for medicine records management
		
		//!Gets a medicine from file corresponding to an input code
		/*!
		Reads a medicine type variable from a file STOCK/MED.DAT that has code equal to that
		input to the function
		\param inp_code The code of the required medicine
		\return The medicine corresponding to inp_code
		*/
		static medicine get_med_by_code(int inp_code);
		
		//!Writes a medicine to file
		/*!
		Writes a medicine type variable to the file STOCK/MED.DAT
		(actually I don't understand what exactly this function does)
		*/
		static void write_med(medicine);
		//!}@

		//!@{Employee management functions
		
		//!Gets an object of an employee(or its derivative) from file corresponding to an input id
		/*!
		Reads an object from a file base.dat that is present in 
		a folder with name as the id of the employee. All such folders are 
		present in a folder named EMPLOYEE, or EMPLOYEE/DOCTOR, or
		EMPLOYEE/NURSE, or EMPLOYEE/RECEPTIONIST
		This function basically converts the id input to it into the 
		employee type, sets the string of the path to the folder containing
		the employee object file, and reads the object from the file to a 
		buffer pointed to by a void pointer
		A void pointer is used in this function to handle the different data types 
		i.e. employee, doctor, nurse, receptionist that can be input into the
		function as the target parameter.
		\param id The id of the employee to be read
		\param target void pointer pointing to the buffer that stores the object
		\return 1 if the function executed without errors, 0 otherwise
		*/
		static int get_employee_by_id(unsigned long id, void * target);
		
		//!Writes an object of an employee(or its derivative) to file
		/*!
		This function converts the id of the object pointed to by the pointer
		a into the employee type, sets the string of the path to the folder that should 
		contain the employee object file,and then makes a new folder(if it doesn't exist)
		in EMPLOYEE, or EMPLOYEE/DOCTOR, or EMPLOYEE/NURSE, or EMPLOYEE/RECEPTIONIST
		(depending upon the type of the object pointed by the input void pointer), whose
		name is the id of the employee object that is to be written, and writes the 
		employee object to a file BASE.DAT inside that folder.
		A void pointer is used in this function to handle the different data types 
		i.e. employee, doctor, nurse, receptionist that can be input into the
		function as the parameter a.
		\param a Void pointer pointing to the object that is to be written to file
		\return 1 if the function executed without errors, 0 otherwise
		*/
		static int write_employee(void * a);
		
		//!Pays salary to an employee having a particular id
		/*!
		Obtains the employee's object from file using get_employee_by_id(),
		gets the salary of the employee, deducts the salary from hospital::balance 
		using deduct_money(), and then writes the transaction details of this payment
		to a file TRANS.DAT present in the folder of the employee (i.e the folder 
		having name as the id of the employee), that is present in folder EMPLOYEE, 
		or EMPLOYEE/DOCTOR, or EMPLOYEE/NURSE, or EMPLOYEE/RECEPTIONIST, depending
		on the employee type
		\param id ID of the employee to whom salary is to be paid
		\param d1 Date of payment of salary
		\param t1 Time of payment of salary
		\return 1 if the function executed without errors, 0 otherwise
		*/
		static int pay_salary(unsigned long id, Date d1, Time t1);
		
		//!Pays salary to all employees whose files are on the disk
		/*!
		Loops the execution of pay_salary(); the maximum no. of times the loop should
		run is determined by max_id, an unsigned long variable stored in a file
		EMPLOYEE/MAX_ID.DAT
		\return 1 if the function executed without errors, 0 otherwise
		*/
		static int pay_all_salaries();
		//!}@

		//!@{Internal implementation functions
		static int get_date_difference(Date, Date); //!>Calculates the no. of days between 2 dates
		static int count_leap_years(Date); //!>Calculates the no. of leap years between a certain date and the year 0 AD
		static int date_validity(const char * inp_date); //!>\return date_validity(str_to_date(inp_date))
		static int date_validity(Date); //!>\return 1 if the date supplied to the function is a valid date, 0 otherwise
		static int time_validity(const char * inp_time); //!>\return time_validity(str_to_time(inp_time))
		static int time_validity(Time); //!>\return 1 if the time supplied to the function is a valid time, 0 otherwise
		static Date str_to_date(const char *); //!>converts a string to a Date type variable
		static Time str_to_time(const char *); //!>converts a string to a Time type variable
		static int str_to_sex(char *); //!>converts a string to an enum sex type variable
		//!}@

	private:
		hospital();
		//! Reads an object from a file on disk and stores it in a buffer
		/*!
		Basically a specialization of the fstream::read() function
		\param ID ID of the object that is to be read(for error logging purposes only)
		\param dest The path string to the file from which the object is to be read
		\param size Size in bytes of the object that is to be read
		\param temp Pointer to the buffer at which the read object is to be stored
		*/
		static int read_from(unsigned long ID, char * dest, int size, char *temp);
		static double balance; //!>net balance of the hospital
};

//!Huh??!!
class pharmacy{
	public:
		static void sale(int, int, int);
		static void purchase(int, int);
};

#endif