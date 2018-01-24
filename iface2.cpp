#include <fstream.h>
#include "base.hpp"
#include "iface.hpp"
#include "hosp.hpp"
#include "emp.hpp"

void interface::init(){
	emp_mgmt::menu2.hide();
	window.hide();
	window.display();
	window.settcolor(WHITE);
	window << ui::centeralign << "LHOSPITAL";
	window.settcolor(ui::tcolor);
	window.setfooter_tcolor(GREEN);

	Date current_date = system::get_date();
	Time current_time = system::get_time();

	str curr_date, curr_time;
	sprintf(curr_date, "%d/%d/%d", current_date.day, current_date.month, current_date.year);
	sprintf(curr_time, "%d:%d", current_time.hour, current_time.minute);

	window << box::setheader << curr_date << box::setheader << ui::rightalign 
			<< curr_time << box::setfooter << ui::centeralign
		    << "Everything looks OK";
	int id;
	do
	{
		id = interface::login_screen();
		if(id && id_to_emp::convert(id) != OTHERS || id == 1)	//so that general employees (except administrator) do
		{														// not accidentally login(as they have been assigned
			interface::clear_error();							// username and password as "", "")
			break;
		}
	}while(1);
	if(id == 1)	//if user logging in is administrator
	{
		int choice = 0;

		while(1){
			choice = interface::menu();

			switch(choice){
				case 1:
					interface::patient_management();
					break;
				case 2:
					interface::employee_management();
					break;
				case 3:
					interface::stock_management();
					break;
				case 4:
					return;
			}
		}
	}
	else
	{
		switch(id_to_emp::convert(id))
		{
			case INVALID:
				interface::error("You have an invalid id generated. Create a new account");
				break;
			case DOCTOR:
			case NURSE:
			case RECEPTIONIST:
				interface::employee_screen(id);
				break;
		}
	}
}

int interface::login_screen()
{
	const int login_screen_height = 9;
	coord c(ui::scr_width / 3, ui::scr_height / 3);
	box login_box (c, ui::scr_width / 3, login_screen_height);

	str uid, pwd;

	login_box.settcolor_input(YELLOW);
	login_box << "User ID : ";
	login_box >> uid;
	login_box << ui::endl << "Password : ";
	login_box >> box::setpassword >> pwd;
	login_box << ui::endl;
	login_box.setexit_button("Login");
	login_box.loop();
	login_box.hide();
	unsigned long max_id;
	ifstream fin;
	fin.open("employee/max_id.dat", ios::binary);
	if(!fin)
		max_id = 1;
	else
	{
		fin.read((char *) &max_id, sizeof(unsigned long));
		if(fin.fail())
		{
			interface::error("ERROR WHILE READING FROM FILE!!! ");
			getch();
			return 0;
		}
	}
	fin.close();
	for(unsigned long id = 1; id <= max_id; ++id)
	{
		void * x = malloc( sizeof(doctor) );
		if(x == NULL)
		{
			interface::log_this("interface::login_screen() : Not enough memory to allocate buffer void * temp = malloc( sizeof(doctor) )");
			interface::error("Out of memory!! Check log");
			getch();
			return 0;
		}
		if(!hospital::get_employee_by_id(id, x))
		{
			char log_msg[300];
			sprintf(log_msg, "interface::login_screen() : Error in reading file of id %lu (hospital::get_employee_by_id(id, x) returned 0), could be due to invalid login details entered", id);
			interface::log_this(log_msg);
		}
		employee * e = (employee *)x;
		if(!strcmp(e->account.get_username(), uid) && e->account.login(pwd))
		{
			interface::clear_error();
			return id;
		}
	}
	interface::error("Invalid login details!!");
	return 0;
}

int interface::menu(){
	coord c(ui::scr_width / 3, ui::scr_height / 3);
	box menu (c, ui::scr_width / 3, ui::scr_height / 2.2 + 1);

	int ch;
	menu << "1. Patient management"
			<< ui::endl << "2. Employee management"
			<< ui::endl << "3. Stock management"
			<< ui::endl << "4. Exit"
			<< ui::endl << ui::endl << "Choice : ";
	menu.settcolor_input(YELLOW);
	validate_menu::set_menu_limits(1, 4);
	menu >> validate_menu::input >> ch;

	menu << ui::endl;
	menu.setexit_button("Submit");

	menu.loop();
	menu.hide();

	return ch;
}

void interface::patient_management(){
	int ch = 0;

	coord c(ui::scr_width / 3, ui::scr_height / 3);
	box menu (c, ui::scr_width / 3, ui::scr_height / 2.2);

	menu << "1. Patient admission"
			<< ui::endl << "2. Patient discharge"
			<< ui::endl << "3. Edit patient details"
			<< ui::endl << "4. Go to main menu"
			<< ui::endl << ui::endl << "Choice : ";
	menu.setdefault(1);
	menu.settcolor_input(YELLOW);
	validate_menu::set_menu_limits(1,4);
	menu >> validate_menu::input >> ch;

		menu << ui::endl;
		menu.setexit_button("Submit");

		menu.loop();
		menu.hide();

	switch(ch){
		case 1:
		{
			coord c(ui::scr_width / 4, ui::scr_height / 4);
			box form (c, ui::scr_width / 2, ui::scr_height / 1.25);
			form.settcolor_input(YELLOW);

			str inp_name, inp_sex_str, inp_dob_str
				, inp_phone, inp_guard_name, inp_emer_contact
				, inp_emer_phone, inp_insur_expiry, inp_admdate_str;
			
			address inp_adr;
			disease inp_dis;
			insurance inp_insur;

			form << "Enter data for the patient :" << ui::endl
					<< ui::endl << "Name : ";
			form >> inp_name;
			
			form << ui::endl << "Sex : ";
			form >> inp_sex_str;
			form << ui::endl << "Key - M/F/T = Male/Female/Trans"
					<< ui::endl << "Date of Birth : ";
			
			form.setdefault("25/12/1991");
			form >> inp_dob_str;
			
			
			form << ui::endl << "Address"
					<< ui::endl << ui::endl
					<< "\tHouse # : ";
			form.setdefault("221B");
			form >> inp_adr.house_no;
			
			form << ui::endl << "\tStreet : ";
			form.setdefault("Baker Street");
			form >> inp_adr.street;
			
			form << ui::endl << "\tDistrict : ";
			form.setdefault("Idk");
			form >> inp_adr.district;
			
			form << ui::endl << "\tState : ";
			form.setdefault("London(?)");
			form >> inp_adr.state;
			
			
			form << ui::endl << ui::endl
					<< "Phone : ";
			form.setdefault("1234567890");
			form >> inp_phone;
			
			
			form << ui::endl << "Disease"
					<< ui::endl << ui::endl
					<< "\tName : ";
			form.setdefault("Melanoma");
			form >> inp_dis.name;
			
			form << ui::endl << "Type : ";
			form.setdefault(0);
			form >> inp_dis.type;
			
			form << ui::endl << "\tType key : " << ui::endl
					<< "\t0 - Brain\t1 - Heart" << ui::endl
					<< "\t2 - Skin\t3 - Lung" << ui::endl
					<< "\t4 - Bone\t5 - Eye" << ui::endl
					<< "\t6 - Throat\t7 - Teeth" << ui::endl
					<< "\t8 - Stomach\t9 - Blood" << ui::endl
					<< "\t10 - General/full body condition"
					<< ui::endl << "\tSymptoms"
					<< ui::endl << "\tSymptom 1 : ";
			
			form >> inp_dis.symptoms[0];
			
			form << ui::endl << "\tSymptom 2 : ";
			form >> inp_dis.symptoms[1];
			
			form << ui::endl << "\tSymptom 3 : ";
			form >> inp_dis.symptoms[2];
			
			form << ui::endl << "\tSymptom 4 : ";
			form >> inp_dis.symptoms[3];
			
			
			form << ui::endl << ui::endl
					<< "Guardian name : ";
			form.setdefault("Dr. John Watson");
			form >> inp_guard_name;
			
			form << ui::endl << "Emergency Contact : ";
			form.setdefault("Irene Adler");
			form >> inp_emer_contact;
			
			form << ui::endl << "Emer. Cont. Phone : ";
			form.setdefault("1234567890");
			form >> inp_emer_phone;
			
			
			form << ui::endl << "Insurance"
					<< ui::endl << ui::endl
					<< "\tProvider : ";
			form.setdefault("LIC");
			form >> inp_insur.provider;
			
			form << ui::endl << "\tAmount ($) : ";
			form.setdefault(30000);
			form >> inp_insur.amount;
			
			form << ui::endl << "\tExpiry";
			form.setdefault("25/12/2022");
			form >> inp_insur_expiry;
			inp_insur.expiry = hospital::str_to_date(inp_insur_expiry);
			
			
			form << ui::endl << ui::endl
					<< "Admission Date : ";
			char dnow[11];
			Date current_date = system::get_date();
			sprintf(dnow, "%d/%d/%d", current_date.day 
									, current_date.month
									, current_date.year);
			form.setdefault(dnow);
			form >> inp_admdate_str;

			form << ui::endl << ui::endl;
			form.setexit_button("Submit");

			form.loop();

			form.hide();

			patient temp_pat = patient(inp_name, hospital::str_to_sex(inp_sex_str)
										, hospital::str_to_date(inp_dob_str), inp_adr
										, inp_phone, inp_dis, inp_guard_name
										, inp_emer_contact, inp_emer_phone
										, inp_insur, hospital::str_to_date(inp_admdate_str));

			hospital::write_patient(temp_pat);

			break;
		}

		case 2:
		{
			patient temp_patient;

			while(1){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box login_box (c, ui::scr_width / 3, ui::scr_height / 2.5);

				long inp_pat_id;

				login_box << ui::endl << "Patient Discharge"
							<< ui::endl << "Enter patient ID : ";
				login_box.setdefault(1);
				login_box >> inp_pat_id;
				
				login_box << ui::endl;
				login_box.setexit_button("Submit");
				
				login_box.loop();

				login_box.hide();

				temp_patient = hospital::get_patient_by_id(inp_pat_id);

				if(temp_patient.get_id() == inp_pat_id){
					break;
					interface::clear_error();
				}
				else{
					interface::error("Invalid Patient ID!!");
					continue;
				}
			}

			coord c(ui::scr_width / 3, ui::scr_height / 3);
			box bill (c, ui::scr_width / 3, ui::scr_height / 2);

			str tt;
			sprintf(tt, "%d/%d/%d", temp_patient.get_admission_date(DAY),
															temp_patient.get_admission_date(MONTH),
															temp_patient.get_admission_date(YEAR));

															interface::log_this(tt);

			int stay_len = abs( hospital::get_date_difference(
														system::get_date(),
														Date(
															temp_patient.get_admission_date(DAY),
															temp_patient.get_admission_date(MONTH),
															temp_patient.get_admission_date(YEAR)
														)
							) );

			bill << ui::endl << "Bill for " << temp_patient.get_name()
					<< ui::endl << "1. Stay for "
					<< stay_len	<< " days"	<< ui::endl;

			float total_bill;
			bill.settcolor(GREEN);
			bill << "$" << ( total_bill += hospital::calc_bill(stay_len) );

			for(int i = 0; i < 50; i++){
					transaction temp_trans = temp_patient.get_transaction(i);

					if( temp_trans.amount == 0 ){
						break;
					}

					bill << i+2 << ". " << temp_trans.reason << ui::endl;
					bill.settcolor(GREEN);
					bill << "\t$" << temp_trans.amount << ui::endl;
					bill.settcolor(ui::tcolor);

					total_bill += temp_trans.amount;
			}

			bill.settcolor(CYAN);
			bill << ui::endl << "Final bill : $" << total_bill;
			bill.settcolor(ui::tcolor);
			bill.setexit_button("Pay Bill");
			bill.loop();
			bill.hide();

			hospital::discharge_patient(temp_patient);

			break;
		}

		case 3:
		{
			int choice = 0;

			patient temp_patient;

			while(1){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box login_box (c, ui::scr_width / 3, ui::scr_height / 2.5);

				long inp_pat_id;

				login_box << ui::endl << "Patient Data Alteration"
							<< ui::endl << "Enter patient ID : ";
				login_box.setdefault(1);
				login_box >> inp_pat_id;
				
				login_box << ui::endl;
				login_box.setexit_button("Submit");
				
				login_box.loop();

				login_box.hide();

				temp_patient = hospital::get_patient_by_id(inp_pat_id);
				
				if(temp_patient.get_id() == inp_pat_id){
					break;
					interface::clear_error();
				}
				else{
					interface::error("Invalid Patient ID!!");
					continue;
				}
			}

			while(choice < 1 || choice > 5){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box menu (c, ui::scr_width / 3, ui::scr_height / 1.5);

				menu << "Choose item to edit:"
						<< ui::endl << "1. Disease/condition"
						<< ui::endl << "2. Guardian name"
						<< ui::endl << "3. Emergency contact"
						<< ui::endl << "4. Emergency contact no."
						<< ui::endl << "5. Insurance information"
						<< ui::endl << ui::endl << "Choice : ";
				menu.setdefault(1);
				menu.settcolor_input(YELLOW);
				menu >> choice;

				menu << ui::endl;
				menu.setexit_button("Submit");

				menu.loop();
				menu.hide();
			}
			switch(choice){
				case 1:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 1.5);

					edit_screen <<  "Enter disease/condition for " << temp_patient.get_name()
								<< ui::endl << "Disease : ";
					disease temp = temp_patient.get_dis();
					edit_screen.setdefault(temp.name);
					edit_screen >> temp.name;
					edit_screen << ui::endl << "Type : ";
					edit_screen.setdefault(temp.type);
					edit_screen >> temp.type;
					edit_screen << ui::endl << "Type key : " << ui::endl
								<< "0 - Brain\t1 - Heart" << ui::endl
								<< "2 - Skin\t3 - Lung" << ui::endl
								<< "4 - Bone\t5 - Eye" << ui::endl
								<< "6 - Throat\t7 - Teeth" << ui::endl
								<< "8 - Stomach\t9 - Blood" << ui::endl
								<< "10 - General/full body condition"
								<< ui::endl << ui::endl
								<< "Symptoms" << ui::endl
								<< "Symptom 1 : ";
					edit_screen.setdefault(temp.symptoms[0]);
					edit_screen >> temp.symptoms[0];
					edit_screen << ui::endl << "Symptom 2 : ";
					edit_screen.setdefault(temp.symptoms[1]);
					edit_screen >> temp.symptoms[1];
					edit_screen << ui::endl << "Symptom 3 : ";
					edit_screen.setdefault(temp.symptoms[2]);
					edit_screen >> temp.symptoms[2];
					edit_screen << ui::endl << "Symptom 4 : ";
					edit_screen.setdefault(temp.symptoms[3]);
					edit_screen >> temp.symptoms[3];

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp_patient.set_dis(temp);
					hospital::write_patient(temp_patient);

					break;
				}

				case 2:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 3);

					edit_screen <<  "Enter name of guardian for " << temp_patient.get_name()
								<< ui::endl << "Guardian Name : ";
					str temp;
					edit_screen.setdefault(temp_patient.get_guardian_name());
					edit_screen >> temp;

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp_patient.set_guardian_name(temp);
					hospital::write_patient(temp_patient);

					break;
				}
				
				case 3:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 3);

					edit_screen <<  "Enter emergency contact no. for " << temp_patient.get_name()
								<< ui::endl << "Contact no. : ";
					str temp;
					edit_screen.setdefault(temp_patient.get_emergency_contact());
					edit_screen >> temp;

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp_patient.set_emergency_contact(temp);
					hospital::write_patient(temp_patient);

					break;
				}
				
				case 4:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 3);

					edit_screen <<  "Enter emergency contact no. for " << temp_patient.get_name()
								<< ui::endl << "Contact no. : ";
					phone temp;
					edit_screen.setdefault(temp_patient.get_emer_contact_no());
					edit_screen >> temp;

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();

					temp_patient.set_emer_contact_no(temp);
					hospital::write_patient(temp_patient);

					break;
				}
				
				case 5:
				{
					coord c(ui::scr_width / 3, ui::scr_height / 3);
					box edit_screen (c, ui::scr_width / 3, ui::scr_height / 3);

					edit_screen <<  "Enter insurance information for " << temp_patient.get_name()
								<< ui::endl << "Provider : ";
					insurance temp = temp_patient.get_insur_info();
					edit_screen.setdefault(temp.provider);
					edit_screen >> temp.provider;
					edit_screen << ui::endl << "Amount (in $) :";
					edit_screen.setdefault(temp.amount);
					edit_screen >> temp.amount;
					edit_screen << ui::endl << "Expiry date (DD/MM/YYYY):";
					char temp_date[11];
					edit_screen >> hospital::date_validity >> temp_date;

					edit_screen << ui::endl << ui::endl;
					edit_screen.setexit_button("Submit");

					edit_screen.loop();

					edit_screen.hide();
					
					temp.expiry = hospital::str_to_date(temp_date);
					temp_patient.set_insur_info(temp);
					hospital::write_patient(temp_patient);

					break;
				}

			}

			break;
		}
		case 4:
		{ 
			break;
		}
	}
}
