#include <fstream.h>
#include "base.hpp"
#include "iface.hpp"
#include "hosp.hpp"
#include "emp.hpp"

////////////////////////////////////////////
//////// Function definitions for interface

void interface::init(){
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
				interface::doctor_screen();
				break;
			case NURSE:
				interface::nurse_screen();
				break;
			case RECEPTIONIST:
				interface::receptionist_screen();
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
			return 0;
		}
	}
	fin.close();
	str temp;
	for(unsigned long id = 1; id <= max_id; ++id)
	{
		employee x;
		if(!hospital::get_employee_by_id(id, &x))
		{
			str errmsg;
			sprintf(errmsg, "Error in reading file of id %lu", id);
			interface::error(errmsg);
			return 0;
		}
		if(!strcmp(x.account.get_username(), uid) && x.account.login(pwd))
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
	box menu (c, ui::scr_width / 3, ui::scr_height / 2.2);

	int ch;
	menu << "1. Patient management"
			<< ui::endl << "2. Employee management"
			<< ui::endl << "3. Stock management"
			<< ui::endl << ui::endl << "Choice : ";
	menu.setdefault(1);
	menu.settcolor_input(YELLOW);
	validate_menu::set_menu_limits(1, 3);
	menu >> validate_menu::input >> ch;

	menu << ui::endl;
	menu.setexit_button("Submit");

	menu.loop();
	menu.hide();

	return ch;
}

void interface::patient_management(){
	int ch = 0;

	while(ch < 1 || ch > 3){
		coord c(ui::scr_width / 3, ui::scr_height / 3);
		box menu (c, ui::scr_width / 3, ui::scr_height / 2.2);

		menu << "1. Patient admission"
				<< ui::endl << "2. Patient discharge"
				<< ui::endl << "3. Edit patient details"
				<< ui::endl << ui::endl << "Choice : ";
		menu.setdefault(1);
		menu.settcolor_input(YELLOW);
		menu >> ch;

		menu << ui::endl;
		menu.setexit_button("Submit");

		menu.loop();
		menu.hide();
	}

	switch(ch){
		case 1:
		{
			coord c(ui::scr_width / 2, ui::scr_height / 3);
			box form (c, ui::scr_width / 2, ui::scr_height / 1.5);
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
			
			form << ui::endl << "/tState : ";
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

		}

		case 2:
		{
			int login_success = 0;

			patient temp_patient;

			while(!login_success){
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
					login_success++;
					interface::clear_error();
				}
				else{
					interface::error("Invalid Patient ID!!");
					continue;
				}
			}

			coord c(ui::scr_width / 3, ui::scr_height / 3);
			box bill (c, ui::scr_width / 3, ui::scr_height / 2);

			int stay_len = hospital::get_date_difference(
														system::get_date(),
														Date(
															temp_patient.get_admission_date(DAY),
															temp_patient.get_admission_date(MONTH),
															temp_patient.get_admission_date(YEAR)
														)
							);

			bill << ui::endl << "Bill for " << temp_patient.get_name()
					<< ui::endl << "1. Stay for "
					<< stay_len	<< " days"	<< ui::endl;

			float total_bill;
			bill.settcolor(GREEN);
			bill << "/t$" << ( total_bill += hospital::calc_bill(stay_len) );

			for(int i = 0; i < 50; i++){
					transaction temp_trans = temp_patient.get_transaction(i);

					if( temp_trans.amount == 0 ){
						break;
					}

					bill << i+2 << ". " << temp_trans.reason << ui::endl;
					bill.settcolor(GREEN);
					bill << "/t$" << temp_trans.amount << ui::endl;
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

		}

		case 3:
		{
			int choice = 0, login_success = 0;

			patient temp_patient;

			while(!login_success){
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
					login_success++;
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
					edit_screen << ui::endl << "Expiry date (MM/DD/YYYY):";
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
	}

}

void interface::employee_management()
{
	/*
	1. View employee data
	2. Add new employee
	3. Remove existing employee
	4. Edit employee data
	5. Pay salary to individual employee
	6. Pay salary to all employees
	7. Back
	*/
	const int menu_corner_top_left_y = 5;
	coord c(ui::scr_width * 0.2, menu_corner_top_left_y);
	int ch;
	box menu(c, ui::scr_width * 0.6, ui::scr_height - 6 );
	menu.settcolor(GREEN);
	menu << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	menu.settcolor(ui::tcolor);
	menu << "1. View employee data" << ui::endl
		 << "2. Add new employee" << ui::endl
		 << "3. Remove existing employee" <<ui::endl
		 << "4. Edit employee data" <<ui::endl
		 << "5. Pay salary to individual employee" <<ui::endl
		 << "6. Pay salary to all employees" <<ui::endl
		 << "7. Back" <<ui::endl
		 << ui::endl <<"Enter your choice: ";
	menu.settcolor_input(YELLOW);
	validate_menu::set_menu_limits(1, 7);
	menu >> validate_menu::input >> ch;
	menu << ui::endl;
	menu.setexit_button("Submit");
	menu.loop();
	menu.hide();
	box menu2(c, ui::scr_width * 0.6, ui::scr_height - 6 );
	menu2.clear();
	menu2.settcolor(GREEN);
	menu2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	menu2.settcolor(WHITE);
	int menu2_height;
	switch (ch)
	{
		case 1:
		{
			menu2_height = 10;
			menu2.setheight(menu2_height);
			menu2 << "View employee data" << ui::endl;
			menu2.settcolor(ui::tcolor);
			menu2 << "Enter employee's id: ";
			unsigned long id;
			void * temp;
			menu2.settcolor_input(YELLOW);
			menu2 >> id;
			menu2 << ui::endl;
			menu2.setexit_button("Submit");
			menu2.loop();
			menu2.hide();
			if(!hospital::get_employee_by_id(id, temp))
			{
				interface::error("Error while reading from file!");
			}
			else
			{
				employee *e = (employee *) temp;
				box menu3( menu2.getcorner_top_left(), menu2.getwidth(), menu2.getheight() );
				menu3.setheight( menu.getheight() );
				menu3.settcolor(GREEN);
				menu3 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
				menu3.settcolor(WHITE);
				menu3 << "Employee Details: " << ui::endl;
				menu3.settcolor(ui::tcolor);
				menu3 << "ID: " << e->get_id() << ui::endl;
				menu3 << "Name: " << e->get_name() << ui::endl;
				menu3 << "Age: " << e->get_age() << ui::endl;
				menu3 << "Sex: " << (sex)e->get_sex() << ui::endl;
				menu3 << "Date of Birth: " << e->get_dob() << ui::endl;
				menu3 << "Address: " << e->get_address() << ui::endl;
				menu3 << "Phone no.: " << e->get_phone() << ui::endl;
				menu3 << "Salary: " << e->get_salary() << ui::endl;
				menu3 << "Shift timings: Starts - " << e->get_shift(START) << ui::endl;
				menu3 << "-------------: Ends - " << e->get_shift(END) <<ui::endl;
				switch( id_to_emp::convert( e->get_id() ) )
				{
					case INVALID:	//Test this case, menu3.hide() not working properly
					{
						menu3.clear();
						int menu3_height = 9;
						menu3.setheight(menu3_height);
						menu3.settcolor(GREEN);
						menu3 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
						menu3.settcolor(WHITE);
						menu3 << "Employee Details: " << ui::endl;
						menu3.settcolor(RED);
						menu3 << "Invalid ID!!" << id_to_emp::convert( e->get_id() );
						menu3.settcolor(ui::tcolor);
						menu3.setexit_button("Back");
						menu3.loop();
						menu3.hide();
						break;
					}
					case OTHERS:
					case RECEPTIONIST:	//there are no extra data members in class receptionist
					{
						menu3.setexit_button("Back");
						menu3.loop();  //	menu3.clear();	int w = window.getwidth(), m = menu3.getwidth(); menu3<<w<<' '<<m;	getch();
						menu3.hide();
						break;
					}
					case DOCTOR:
					{
						doctor *d = (doctor *)temp;
						menu3.hide();
						menu3.setcorner_top_left( coord( 1, menu3.getcorner_top_left().y ) );
						menu3.display();
						menu3.f << ( ui::top | ui::left ) << (char)204
								<< ( ui::bottom | ui::left ) << (char)204;
						menu3.f.display();
						box sidemenu( menu3.getcorner_top_left() + coord( menu3.getwidth() - 1, 0 ), ( ui::scr_width - menu3.getwidth() + 1 ),  menu3.getheight() );
						sidemenu.f << ( ui::top | ui::left ) << (char)203
								   << ( ui::bottom | ui::left ) << (char)202
								   << ( ui::top | ui::right ) << (char)185
								   << ( ui::bottom | ui::right ) << (char)185;
						sidemenu.f.display();
						sidemenu << "Speciality(s)" << ui::endl;
						for(int i = 0; i < 2 && d->get_speciality()[i] <= GEN; ++i)
						{
							sidemenu << i + 1 << ". " << (body_parts)d->get_speciality()[i] << ui::endl;
						}
						if(!i)
						{
							sidemenu << "None" << ui::endl;
						}
						sidemenu << "Patients currently under care:" << ui::endl;
						for(i = 0; d->get_patients()[i] && i < 10; ++i)
						{
							sidemenu << i + 1 << ". " << hospital::get_patient_by_id( d->get_patients()[i] ).get_name() << ui::endl;
						}
						if(!i)
						{
							sidemenu << "None" << ui::endl;
						}
						menu3.loop();
						menu3.hide();
						sidemenu.hide();
						window.f.display();
						break;
					}
					case NURSE:
					{
						nurse *n = (nurse *)temp;
						menu3.hide();
						menu3.setcorner_top_left( coord( 1, menu3.getcorner_top_left().y ) );
						menu3.display();
						menu3.f << ( ui::top | ui::left ) << (char)204
								<< ( ui::bottom | ui::left ) << (char)204;
						menu3.f.display();
						box sidemenu( menu3.getcorner_top_left() + coord( menu3.getwidth() - 1, 0 ), ( ui::scr_width - menu3.getwidth() + 1 ),  menu3.getheight() );
						sidemenu.f << ( ui::top | ui::left ) << (char)203
								   << ( ui::bottom | ui::left ) << (char)202
								   << ( ui::top | ui::right ) << (char)185
								   << ( ui::bottom | ui::right ) << (char)185;
						sidemenu.f.display();
						
						break;
					}
				}
			}
			break;
		}
		case 2:
		{
			menu2 << "Add new employee" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 3:
		{
			menu2 << "Remove existing employee" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 4:
		{
			menu2 << "Edit employee data" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 5:
		{
			menu2 << "Pay salary to individual employee" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 6:
		{
			menu2 << "Pay salary to all employees" << ui::endl;
			menu2.settcolor(ui::tcolor);
			break;
		}
		case 7:
		{
			menu2 << "This program will exit now. You wanna go back, you're gonna have to start the program again." << ui::endl;
			break;
		}
	}
}

void interface::stock_management(){
	coord c(ui::scr_width / 3, ui::scr_height / 3);
	box menu (c, ui::scr_width / 3, ui::scr_height / 2.2);

	int ch = 0;

	while (ch < 1 || ch > 3){
		menu << "1. Sale"
				<< ui::endl << "2. Purchase"
				<< ui::endl << "3. Stock check"
				<< ui::endl << ui::endl << "Choice : ";
		menu.setdefault(1);
		menu.settcolor_input(YELLOW);
		menu >> ch;

		menu << ui::endl;
		menu.setexit_button("Submit");

		menu.loop();
		menu.hide();

		interface::error("Invalid choice!!");
	}

	interface::clear_error();

	switch(ch){
		case 1:
		{

			medicine temp;
			temp.code = 0;

			while(temp.code == 0){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box sale_menu (c, ui::scr_width / 3, ui::scr_height / 3);
				sale_menu.settcolor_input(YELLOW);
				sale_menu << ui::centeralign << "Medicine Sale";
				sale_menu << "Code : ";
				sale_menu.setdefault(42);
				sale_menu >> temp.code;
				sale_menu << ui::endl;
				sale_menu.setexit_button("Submit");
				sale_menu.loop();
				sale_menu.hide();

				temp = hospital::get_med_by_code(temp.code);
			}
			
			int quantity = -2;
			patient temp_patient;
			long pat_id;

			while(quantity < 0 || quantity > 100){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box sale_menu (c, ui::scr_width / 3, ui::scr_height / 2);
				sale_menu.settcolor_input(YELLOW);
				sale_menu << ui::centeralign << "Medicine Sale";
				sale_menu << "Name : " << temp.name
							<< ui::endl << "Price : $" << temp.price
							<< ui::endl << ui::endl
							<< "Patient ID : ";
				sale_menu.setdefault(786);
				sale_menu >> pat_id;
				sale_menu << ui::endl << "Quantity : ";
				sale_menu.setdefault(1);
				sale_menu >> quantity;
				sale_menu.setexit_button("Submit");
				sale_menu.loop();
				sale_menu.hide();

				temp_patient = hospital::get_patient_by_id(pat_id);
				if(temp_patient.get_id() == 0){
					quantity = -1;
					interface::error("Invalid patient ID!!");
					continue;
				}
				interface::error("Invalid quantity!!");
			}

			interface::clear_error();

			temp.stock -= quantity;

			for(int i = 0; i < 50; i++){
				if(temp_patient.get_med(i, 0) == temp.code ||
						temp_patient.get_med(i,0) == 0){
							temp_patient.set_med(i, temp.code, temp_patient.get_med(i, 1) + quantity);
						}
			}

			hospital::write_patient(temp_patient);		
			//hospital::write_med(temp);

			break;
		}

		case 2:
		{
			medicine temp;
			temp.code = 0;

			while(temp.code == 0){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box purchase_menu (c, ui::scr_width / 3, ui::scr_height / 3);
				purchase_menu.settcolor_input(YELLOW);
				purchase_menu << ui::centeralign << "Medicine Purchase";
				purchase_menu << "Code : ";
				purchase_menu.setdefault(42);
				purchase_menu >> temp.code;
				purchase_menu << ui::endl;
				purchase_menu.setexit_button("Submit");
				purchase_menu.loop();
				purchase_menu.hide();

				temp = hospital::get_med_by_code(temp.code);
			}

			int quantity;

			while(quantity < 0 || quantity > 5000){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box purchase_menu (c, ui::scr_width / 3, ui::scr_height / 2);
				purchase_menu.settcolor_input(YELLOW);
				purchase_menu << ui::centeralign << "Medicine Sale";
				purchase_menu << "Name : " << temp.name
							<< ui::endl << "Price : $" << temp.price
							<< ui::endl << ui::endl << "Quantity : ";
				purchase_menu.setdefault(1);
				purchase_menu >> quantity;
				purchase_menu.setexit_button("Submit");
				purchase_menu.loop();
				purchase_menu.hide();

				interface::error("Invalid quantity!!");
			}

			interface::clear_error();

			temp.stock += quantity;
			hospital::deduct_money(temp.price * quantity, "Medicine purchase", system::get_date(), system::get_time());
			//hospital::write_med(temp);
		}
		
		case 3:
		{
			medicine temp;
			temp.code = 0;

			while(temp.code == 0){
				coord c(ui::scr_width / 3, ui::scr_height / 3);
				box stock_menu (c, ui::scr_width / 3, ui::scr_height / 3);
				stock_menu.settcolor_input(YELLOW);
				stock_menu << ui::centeralign << "Medicine Sale";
				stock_menu << "Code : ";
				stock_menu.setdefault(42);
				stock_menu >> temp.code;
				stock_menu << ui::endl;
				stock_menu.setexit_button("Submit");
				stock_menu.loop();
				stock_menu.hide();

				temp = hospital::get_med_by_code(temp.code);
			}

			coord c(ui::scr_width / 3, ui::scr_height / 3);
			box stock_menu (c, ui::scr_width / 3, ui::scr_height / 2);
			stock_menu.settcolor_input(YELLOW);
			stock_menu << ui::centeralign << "Medicine Details";
			stock_menu << "Name : " << temp.name
						<< ui::endl << "Price : $" << temp.price
						<< ui::endl << "Dosage : " << temp.dosage << " ml"
						<< ui::endl << "Quantity in stock : " << temp.stock
						<< ui::endl;
			stock_menu.setexit_button("Okay");
			stock_menu.loop();
			stock_menu.hide();
		}
	}

}

void interface::doctor_screen()
{
	coord c(1, 4);
	box profile(c, (ui::scr_width * 3 / 5), ui::scr_height - 5);
	box menu(( c + coord((ui::scr_width * 3 / 5) - 1, 0)), (ui::scr_width * 2 / 5) + 1, ui::scr_height - 5);
	profile.f << ( ui::top | ui::left ) << (char)204
			  << ( ui::bottom | ui::left ) << (char)204
			  << ( ui::top | ui::right ) << (char)203
			  << ( ui::bottom | ui::right ) << (char)202;
	profile.f.display();
	menu.f << ( ui::top | ui::left ) << (char)203
		   << ( ui::bottom | ui::left ) << (char)202
		   << ( ui::top | ui::right ) << (char)185
		   << ( ui::bottom | ui::right ) << (char)185;
	menu.f.display();
	profile.settcolor(GREEN);
	profile << ui::centeralign << "Personal Details" << ui::endl;
	profile.settcolor(ui::tcolor);
	profile << "Name: ";
	profile.setexit_button("yay");	profile.loop();menu << "yay";
	menu.hide();
	profile.hide();
	window.f.display();
}
void interface::nurse_screen()
{
	coord c(1, 4);
	box profile(c, (ui::scr_width * 3 / 5), ui::scr_height - 5);
	box menu(( c + coord((ui::scr_width * 3 / 5) - 1, 0)), (ui::scr_width * 2 / 5) + 1, ui::scr_height - 5);
	profile.f << ( ui::top | ui::left ) << (char)204
			  << ( ui::bottom | ui::left ) << (char)204
			  << ( ui::top | ui::right ) << (char)203
			  << ( ui::bottom | ui::right ) << (char)202;
	profile.f.display();
	menu.f << ( ui::top | ui::left ) << (char)203
		   << ( ui::bottom | ui::left ) << (char)202
		   << ( ui::top | ui::right ) << (char)185
		   << ( ui::bottom | ui::right ) << (char)185;
	menu.f.display();
	profile.settcolor(GREEN);
	profile << ui::centeralign << "Personal Details" << ui::endl;
	profile.settcolor(ui::tcolor);
	profile << "Name: ";
	profile.setexit_button("yay");	profile.loop();menu << "yay";
	menu.hide();
	profile.hide();
	window.f.display();
}
void interface::receptionist_screen()
{
	coord c(1, 4);
	box profile(c, (ui::scr_width * 3 / 5), ui::scr_height - 5);
	box menu(( c + coord((ui::scr_width * 3 / 5) - 1, 0)), (ui::scr_width * 2 / 5) + 1, ui::scr_height - 5);
	profile.f << ( ui::top | ui::left ) << (char)204
			  << ( ui::bottom | ui::left ) << (char)204
			  << ( ui::top | ui::right ) << (char)203
			  << ( ui::bottom | ui::right ) << (char)202;
	profile.f.display();
	menu.f << ( ui::top | ui::left ) << (char)203
		   << ( ui::bottom | ui::left ) << (char)202
		   << ( ui::top | ui::right ) << (char)185
		   << ( ui::bottom | ui::right ) << (char)185;
	menu.f.display();
	profile.settcolor(GREEN);
	profile << ui::centeralign << "Personal Details" << ui::endl;
	profile.settcolor(ui::tcolor);
	profile << "Name: ";
	profile.setexit_button("yay");	profile.loop();menu << "yay";
	menu.hide();
	profile.hide();
	window.f.display();
}

int interface::validate_menu::input(const char * ch)
{
	char *endptr;
	int a = (int) strtol(ch, &endptr, 10);
	if(!validation::vint(ch) || a < lowest_choice || a > greatest_choice)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void interface::validate_menu::set_menu_limits(int a, int b)
{
	lowest_choice = a;
	greatest_choice = b;
}

int interface::validate_menu::lowest_choice = 0;
int interface::validate_menu::greatest_choice = 0;

void interface::error(char* err){
	window.clear_footer();
	window.setfooter_tcolor(RED);
	window << box::setfooter << ui::centeralign
		<< err;
}

void interface::clear_error(){
	window.clear_footer();
	window.setfooter_tcolor(GREEN);
	window << box::setfooter << ui::centeralign
		<< "Everything looks OK";
}

int interface::log_this(char * message)
{
	Date dnow = system::get_date();
	Time tnow = system::get_time();
	str text;
	sprintf(text, "$ [%u-%u-%u %u:%u:%u +0530]: ", dnow.day, dnow.month, dnow.year, tnow.hour, tnow.minute, tnow.second);
	strcat(text, message);
	ofstream fout;
	fout.open("log.txt", ios::out | ios::app);
	if(!fout)
		return 0;
	fout << text << endl;
	if(fout.fail())
		return 0;
	fout.close();
	return 1;
}

box interface::window;