float hospital::get_balance(){
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

patient hospital::get_patient_by_id(long id){
	patient a;

	str temp;

	sprintf(temp, "patient/%lu/base.dat", id);

	int i = hospital::read_from(id, (char *) &a, sizeof(patient), temp);

	if(!i){
		interface::error("File read error!!");
		getch();
	}

	return a;

}

void hospital::write_patient(patient a){
	str temp, temp2;
	sprintf(temp, "patient/%lu/base.dat", a.get_id());
	sprintf(temp2, "patient/%lu", a.get_id());
	mkdir("patient");
	mkdir(temp2);
	ofstream patient_file ( temp , ios::out | ios::binary );

	if(patient_file){
		patient_file.write( (char*) &a , sizeof(patient) );
	}
	else{
		interface::error("Patient file access failure!!");
	}
	if(patient_file.fail()){
		interface::error("Patient file write failure!!");
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

void hospital::discharge_patient(patient temp){
	temp.discharge();
	temp.set_discharge_date( system::get_date() );
	hospital::write_patient(temp);
}

float hospital::calc_bill(int stay){
	return stay * ::stay_charge;
}

medicine hospital::get_med_by_code(int inp_code){
	fstream meds ("stock/med.dat", ios::in | ios::binary);

	medicine temp;
	
	if(inp_code < 1 || inp_code > 100){
		temp.code = 0;
		temp.price = 0;
		temp.dosage = 0;
		temp.stock = 0;
		strcpy(temp.name, "Shell Medicine");

		interface::error("Invalid medicine code!!");

		return temp;
	}

	for(int i = 0; i<100; i++){
		meds.read((char*) &temp, sizeof(medicine));
		if(temp.code == inp_code){
			break;
		}
	}

	return temp;
}

void hospital::write_med(medicine inp_med){
	fstream med_file ("stock/medicine.dat", ios::in | ios::out | ios::binary);
	med_file.seekg(0);

	int success = 0;
	
	while (!success){
		medicine a;
		med_file.read( (char*) &a, sizeof(medicine) );
		if(a.code==inp_med.code){
			med_file.seekg( med_file.tellg() - sizeof(medicine) );
			med_file.write( (char*) &a, sizeof(medicine) );
			success++;
		}
	}

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

int hospital::count_leap_years(Date d)
{
	int years = d.year;

	if (d.month <= 2){
		years--;		// checking whether to count the current year
	}

	return (years / 4) - (years / 100) + (years / 400);
}

int hospital::date_validity(const char * inp_date){
	return hospital::date_validity(hospital::str_to_date(inp_date));
}

int hospital::date_validity(Date inp_date){
	 if(inp_date.year % 4 == 0 && inp_date.month == 2 &&
		inp_date.day == 29){
			return 1;
	}
	if (
		inp_date.month > 12 || 
		inp_date.day > monthDays[inp_date.month - 1])
	{
		return 0;
	}
	else{
		return 1;
	}
}

Date hospital::str_to_date(const char * inp_date){
	int counter = 0;
	int count = 0;
	int input[3];
	input[0] = input[1] = input[2] = 0;
	while(counter < 3){
		char ch[12];
		ch[0] = '/';
		for(int i = 1; i < 7; i++){
			ch[i] = inp_date[count];
			count++;
			if(ch[i] == '/' || ch[i] == '\\' || ch[i] == 0 || ch[i] == '-'){
				if(ch[i] == 0 && count < 11){
					interface::error("Invalid date!");
					return Date (99, 99, 9999);
				}
				ch[i] = '/';
				int temp = i-1, temp2 = 0;
				while(ch[temp] != '/'){
					input[counter] += ( pow(10, temp2) * ((int)ch[temp] - (int)'0') );
					temp--;
					temp2++;
				}
				counter++;
			}
		}
	}

	return Date(input[0], input[1], input[2]);
}

