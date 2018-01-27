/*!
 \file PATIENT.HPP
 \brief Contains the patient class definition
*/

#ifndef PATIENT
#define PATIENT

#include "base.hpp"

class patient : public person
{
	protected:
		long id;
		disease dis;		//patient's afflictions
		str allergies[2];	//patient's known allergies
		int med[50][2];		//patient's purchased meds & quantities
		str guardian_name;
		str emergency_contact;
		phone emer_contact_no;
		insurance insur_info;
		Date admission_date;
		unsigned long bill_amt;
		int discharged;
		Date discharge_date;
	public:
		patient(str, int, Date, address, phone, disease, str, str, phone, insurance, Date = system::get_date());	//if date_of_admission is the current system date, last argument is not needed
		patient();	// Default constructor
		//'get's
		long get_id();
		disease get_dis();
		char* get_guardian_name();
		char* get_emergency_contact();
		char* get_emer_contact_no();
		insurance get_insur_info();
		int get_admission_date(int);
		unsigned long get_bill_amt();
		int get_med(int, int);
		int get_discharge_date(int);
		transaction get_transaction(int);
		transaction get_transaction();

		//updating functions
		void set_dis(disease);
		void set_guardian_name(char*);
		void set_emergency_contact(char*);
		void set_emer_contact_no(char*);
		void set_insur_info(insurance);
		void set_admission_date(Date);
		void set_bill_amt(unsigned long);
		void set_med(int, int, int);
		void set_discharge_date(Date);
		void discharge();
};

#endif
