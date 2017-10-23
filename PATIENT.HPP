#ifndef PATIENT
#define PATIENT

#include "base.hpp"

class patient : public person
{
	protected:
		disease dis;  		//patient's afflictions
		str allergies[2];   //patient's known allergies
		int med[50][2]		//patient's purchased meds & quantities
		str guardian_name;
		str emergency_contact;
		phn emer_contact_no;
		insurance insur_info;
		Date admission_date;
		unsigned long bill_amt;
	public:
		 patient(str, int, Date, adr, phn, disease, str, str, phn, insurance);
		 patient(str, int, Date, adr, phn, disease, str, str, phn, insurance, Date); //if date_of_admission is not the current system date
		//'get's
		disease getdis();
		char* getguardian_name();
		char* getemergency_contact();
		char* getemer_contact_no();
		insurance getinsur_info();
		int getadmission_date(int);
		unsigned long getbill_amt();
		int get_med(int, int);

		//updating functions
		void setdis(disease);
		void setguardian_name(char*);
		void setemergency_contact(char*);
		void setemer_contact_no(char*);
		void setinsur_info(insurance);
		void setadmission_date(Date);
		void setbill_amt(unsigned long);
		void set_med(int, int, int);
};

#endif
