#ifndef PATIENT
#define PATIENT
class patient
{
		str name;
		address addr;
		date dob;
		str guardian_name;
		int age;
		char sex;	//x-female | y-male | z-others
		char contact_no[K];
		str emergency_contact;
		char emer_contact_no[K];
		insurance insur_info;
		///////////////////////////////////////
		///Symptoms reported by patient////
		str symp[4];
		str allergies[2];
		///////////////////////////////////////
		date d_of_admission;
	public:


}

#endif