/*!
 \file PATIENT.HPP
 \brief Contains the patient class definition
*/

#ifndef PATIENT
#define PATIENT

#include "base.hpp"

//!Base patient class
class patient : public person
{
	protected:

		long id; //!>Unique identification number of patient
		disease dis; //!>patient's afflictions
		str allergies[2]; //!>patient's known allergies
		int med[50][2]; //!>patient's purchased medicines & quantities
		str guardian_name; //!>Patient's guardian/caretaker
		str emergency_contact; //!>Patient's emergency contact's name
		phone emer_contact_no; //!>Emergency contact phone number
		insurance insur_info; //!>Insurance struct storing patient's insurance info
		Date admission_date; //!>Date patient was admitted
		unsigned long bill_amt; //!>Net amount to be billed to patient
		int discharged; //!>Binary; = 0 Admitted | = 1 Discharged
		Date discharge_date; //!>Date patient was discharged. NULL if still admitted

	public:

		patient(str, int, Date, address, phone, disease, str, str, phone, insurance, Date = system::get_date()); //!>Explicit constructor
		//!>If no date is provided, it is assumed that patient was admitted on the current system date
		patient(); //!>Default constructor
		
		//!@{Getters

		//!Returns unique ID of patient
		long get_id();
		//!Returns a disease type object of patient's affliction
		disease get_dis();
		//!Returns patient's guardian's name
		char* get_guardian_name();
		//!Returns emergency contact's name
		char* get_emergency_contact();
		//!Returns emergency contact's phone no.
		char* get_emer_contact_no();
		//!Returns patient's insurance info as a struct
		insurance get_insur_info();
		/*!Returns admission date parameters
				Uses enum date_type
					0 - DAY
					1 - MONTH
					2 - YEAR*/
		int get_admission_date(int);
		//!Returns current amount billed to patient
		unsigned long get_bill_amt();
		/*!Returns a record of medicines purchased
				get_med(x, y)
					0 <= x < 50	Number of medicine purchase in list
					0 <= y <= 1 | 0 - Return medicine code
								| 1 - Return medicine quantity */
		int get_med(int, int);
		/*!Returns discharge date parameters
				Uses enum date_type
					0 - DAY
					1 - MONTH
					2 - YEAR*/
		int get_discharge_date(int);
		/*!Returns a transaction element by its number from
		the patient's list of transactions*/
		transaction get_transaction(int);
		//!}@

		//!@{Setters
		//!Overwrites the current stored affliction for patient
		void set_dis(disease);
		//!Overwrites the current stored guardian name
		void set_guardian_name(char*);
		//!Overwrites the current stored emergency contact name
		void set_emergency_contact(char*);
		//!Overwrites the current stored emeegency contant phone
		void set_emer_contact_no(char*);
		//!Overwrites the current stored insurance information
		void set_insur_info(insurance);
		//!Overwrites the current stored admission date
		void set_admission_date(Date);
		//!Overwrites the current billed amount
		void set_bill_amt(unsigned long);
		/*!Overwrites a medicine record for patient
				get_med(x, y, z)
					0 <= x < 50	Number of medicine purchase in list
					y - medicine code
					z - medicine quantity */
		void set_med(int, int, int);
		//!Overwrites the current stored discharge date
		void set_discharge_date(Date);
		//!Sets patient's discharge status to 1 / TRUE
		void discharge();
		//!}@
};

#endif
