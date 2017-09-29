#ifndef PATIENT
#define PATIENT

#include "base.hpp"

class patient : public person
{
    protected:
		disease dis;
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

		//updating functions
		disease setdis(disease);
		char* setguardian_name(char*);
		char* setemergency_contact(char*);
		char* setemer_contact_no(char*);
		insurance setinsur_info(insurance);
		Date setadmission_date(Date);
		unsigned long setbill_amt(unsigned long);
};

#endif
