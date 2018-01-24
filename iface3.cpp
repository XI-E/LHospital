#include <fstream.h>
#include "base.hpp"
#include "iface.hpp"
#include "hosp.hpp"
#include "emp.hpp"

void interface::employee_management()
{
	const int menu_corner_top_left_y = 5;
	coord c(ui::scr_width * 0.2, menu_corner_top_left_y);
	int ch;
	while(1)
	{
		interface::clear_error();
		box menu(c, ui::scr_width * 0.6, ui::scr_height - 6 );
		menu.settcolor(GREEN);
		menu << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
		menu.settcolor(ui::tcolor);
		menu << "1. View employee data" << ui::endl
			 << "2. Add new employee" << ui::endl
			 << "3. Remove existing employee" << ui::endl
			 << "4. Edit employee data" << ui::endl
			 << "5. Pay salary to individual employee" << ui::endl
			 << "6. Pay salary to all employees" << ui::endl
			 << "7. Back" << ui::endl
			 << ui::endl << "Enter your choice: ";
		menu.settcolor_input(YELLOW);
		validate_menu::set_menu_limits(1, 7);
		menu >> validate_menu::input >> ch;
		menu << ui::endl;
		menu.setexit_button("Submit");
		menu.loop();
		menu.hide();
		emp_mgmt::menu2 = box( c, ui::scr_width * 0.6, ui::scr_height - 6);
		emp_mgmt::menu2.display();
		emp_mgmt::menu2.settcolor(GREEN);
		emp_mgmt::menu2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
		emp_mgmt::menu2.settcolor(WHITE);
		switch (ch)
		{
			case 1:
			{
				emp_mgmt::view_emp();
				break;
			}
			case 2:
			{
				emp_mgmt::add_emp();
				break;
			}
			case 3:
			{
				emp_mgmt::remove_emp();
				break;
			}
			case 4:
			{
				emp_mgmt::edit_emp();
				break;
			}
			case 5:
			{
				emp_mgmt::pay_emp();
				break;
			}
			case 6:
			{
				emp_mgmt::pay_all();
				break;
			}
			case 7:
			{
				emp_mgmt::menu2.hide();
				return;
			}
		}
	}
}

void interface::employee_screen(unsigned long id)
{
	void * temp = malloc( sizeof(doctor) );	//as doctor has the greatest size among employee, doctor, nurse and receptionist classes
	if(temp == NULL)
	{
		interface::log_this("interface::employee_screen() : Not enough memory to allocate buffer void * temp = malloc( sizeof(doctor) )");
		interface::error("Out of memory!! Check log");
		getch();
		return;
	}
	if(!hospital::get_employee_by_id(id, temp))
	{
		interface::error("ID not found or error while reading from file!");
		getch();
		return;
	}
	employee *e = (employee *) temp;
	const int menu_corner_top_left_y = 5;
	coord c(ui::scr_width * 0.2, menu_corner_top_left_y);
	int ch;
	str heading = "Welcome, ";
	strcat( heading, e->get_name() );
	strcat(heading, "!");
	while(1)
	{
		interface::clear_error();
		box menu(c, ui::scr_width * 0.6, ui::scr_height - 6 );
		menu.settcolor(GREEN);
		menu << ui::centeralign << heading << ui::endl << ui::endl;
		menu.settcolor(ui::tcolor);
		menu << "1. View profile" << ui::endl
			 << "2. Edit profile" << ui::endl
			 << "3. View last 10 transactions" << ui::endl
			 << "4. Exit" << ui::endl
			 << ui::endl << "Enter your choice: ";
		menu.settcolor_input(YELLOW);
		validate_menu::set_menu_limits(1, 4);
		menu >> validate_menu::input >> ch;
		menu << ui::endl;
		menu.setexit_button("Submit");
		menu.loop();
		menu.hide();
		switch(ch)
		{
			case 1: 
			{
				emp_mgmt::menu2 = box( c, ui::scr_width * 0.6, ui::scr_height - 6);
				emp_mgmt::menu2.hide();
				if(! emp_mgmt::view_emp(id) )
				{
					interface::error("Failed to display profile!");
					getch();
				}
				break;
			}
			case 2:
			{
				break;
			}
			case 3:
			{
				transaction t[10], null;
				for(int i = 0; i < 10; ++i)
				{
					t[i] = e->get_last_10_transactions()[i];
				}
				if( t == NULL )
				{
					interface::error("Error while reading or writing to file! Check log");
					getch();
					break;
				}
				
				
				break;
			}
			case 4:
			{
				return;
			}
		}
	}
}

emp_mgmt::emp_mgmt()
{}

int emp_mgmt::menu2_height = 1;

box emp_mgmt::menu2;

void emp_mgmt::view_emp()
{
	menu2_height = 10;
	menu2.setheight(menu2_height);
	menu2 << "View employee data" << ui::endl;
	menu2.settcolor(ui::tcolor);
	menu2 << "Enter employee's id: ";
	unsigned long id;
	menu2.settcolor_input(YELLOW);
	menu2 >> id;
	menu2 << ui::endl;
	menu2.setexit_button("Submit");
	menu2.loop();
	menu2.hide();
	view_emp(id);
}

int emp_mgmt::view_emp(unsigned long id)
{
	void * temp = malloc( sizeof(doctor) );	//as doctor has the greatest size among employee, doctor, nurse and receptionist classes
	if(temp == NULL)
	{
		interface::log_this("emp_mgmt::view_emp(int) : Not enough memory to allocate buffer void * temp = malloc( sizeof(doctor) )");
		interface::error("Out of memory!! Check log");
		getch();
		return 0;
	}
	if(!hospital::get_employee_by_id(id, temp))
	{
		interface::error("ID not found or error while reading from file!");
		getch();
		return 0;
	}
	employee *e = (employee *) temp;
	box menu3( menu2.getcorner_top_left(), menu2.getwidth(), ui::scr_height - 6 );
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
			sidemenu.setexit_button("Back");
			sidemenu.loop();
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
			sidemenu << "Patients currently under care:" << ui::endl;
			for(int i = 0; n->get_patients()[i] && i < 5; ++i)
			{
				sidemenu << i + 1 << ". " << hospital::get_patient_by_id( n->get_patients()[i] ).get_name() << ui::endl;
			}
			if(!i)
			{
				sidemenu << "None" << ui::endl;
			}
			sidemenu.setexit_button("Back");
			sidemenu.loop();
			menu3.hide();
			sidemenu.hide();
			window.f.display();
			break;
		}
	}
	return 1;
}

void emp_mgmt::add_emp()
{
	menu2_height = 17;
	menu2.setheight(menu2_height);
	menu2 << "Add new employee" << ui::endl;
	menu2.settcolor(ui::tcolor);
	menu2 << "Step 1: Select employee type" << ui::endl << ui::endl
		  << "1. Doctor" << ui::endl
		  << "2. Nurse" << ui::endl
		  << "3. Receptionist" << ui::endl
		  << "4. Others" << ui::endl << ui::endl
		  << "Enter your choice: ";
	int ch;
	validate_menu::set_menu_limits(1, 4);
	menu2.settcolor_input(YELLOW);
	menu2 >> validate_menu::input >> ch;
	menu2 << ui::endl;
	menu2.setexit_button("Submit");
	menu2.loop();
	menu2.hide();
	box menu3( menu2.getcorner_top_left(), menu2.getwidth(), menu2.getheight() );
	menu3.settcolor(GREEN);
	menu3 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	menu3.settcolor(WHITE);
	menu3 << "Add new employee" << ui::endl;
	menu3.settcolor(ui::tcolor);
	menu3 << "Step 2: Add employee details" << ui::endl << ui::endl;
	str name, dob_str, adr_hno, adr_street, adr_city, adr_dist, adr_state, shift_start_str, shift_end_str, uid, pwd;
	unsigned sex_choice;
	Date dob;
	address adr;
	phone phn_no;
	unsigned long salary;
	Time shift_start, shift_end;
	menu3.settcolor_input(YELLOW);
	menu3 << "Name: ";
	menu3 >> name;
	menu3 << "Sex: 1. Male | 2. Female | 3. Transsexual" << ui::endl
		  << "---- Enter your choice: ";
	validate_menu::set_menu_limits(1, 3);
	menu3 >> validate_menu::input >> (int)sex_choice;
	menu3 << "Date of Birth(DD/MM/YYYY): ";
	menu3 >> hospital::date_validity >> dob_str;
	menu3 << "Address: " << ui::endl;
	menu3 << (char)26 << "House no.: ";
	menu3 >> adr_hno;
	menu3 << (char)26 << "Street: ";
	menu3 >> adr_street;
	menu3 << (char)26 << "City: ";
	menu3 >> adr_city;
	menu3 << (char)26 << "District: ";
	menu3 >> adr_dist;
	menu3 << (char)26 << "State: ";
	menu3 >> adr_state;
	menu3 << "Phone no.: ";
	menu3 >> phn_no;
	menu3 << "Salary: ";
	menu3 >> salary;
	menu3 << "Shift timings: Starts - (HH:MM:SS)";
	menu3 >> hospital::time_validity >> shift_start_str;
	menu3 << "-------------: Ends - (HH:MM:SS)";
	menu3 >> hospital::time_validity >> shift_end_str;
	menu3.setexit_button("Submit");
	menu3.loop();
	--sex_choice;
	dob = hospital::str_to_date(dob_str);
	adr = address(adr_hno, adr_street, adr_city, adr_dist, adr_state);
	shift_start = hospital::str_to_time(shift_start_str);
	shift_end = hospital::str_to_time(shift_end_str);
	menu3.hide();
	if(ch != 4)
	{
		box menu4(menu3.getcorner_top_left(), menu3.getwidth(), menu3.getheight());
		menu4.settcolor(GREEN);
		menu4 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
		menu4.settcolor(WHITE);
		menu4 << "Add new employee" << ui::endl;
		menu4.settcolor(ui::tcolor);
		menu4.settcolor_input(YELLOW);
		menu4 << "Step 3: Add login details" << ui::endl << ui::endl;
		menu4 << "User ID: ";
		menu4 >> uid;
		menu4 << "Password: ";
		menu4 >> box::setpassword >> pwd;
		menu4 << ui::endl;
		menu4.setexit_button("Submit");
		menu4.loop();
		menu4.hide();
	}
	void * temp = NULL;
	unsigned long id;
	switch (ch)
	{
		case 1:
		{
			coord c(1, 4);
			box menu5(c, (ui::scr_width / 2), ui::scr_height - 5);
			box inp_box(( c + coord((ui::scr_width / 2) - 1, 0)), (ui::scr_width / 2) + 1, ui::scr_height - 5);
			menu5.f << ( ui::top | ui::left ) << (char)204
					  << ( ui::bottom | ui::left ) << (char)204
					  << ( ui::top | ui::right ) << (char)203
					  << ( ui::bottom | ui::right ) << (char)202;
			menu5.f.display();
			inp_box.f << ( ui::top | ui::left ) << (char)203
				   << ( ui::bottom | ui::left ) << (char)202
				   << ( ui::top | ui::right ) << (char)185
				   << ( ui::bottom | ui::right ) << (char)185;
			inp_box.f.display();
			menu5 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
			menu5.settcolor(WHITE);
			menu5 << "Add new employee" << ui::endl;
			menu5.settcolor(ui::tcolor);
			menu5 << "Step 4: Add doctor details" << ui::endl << ui::endl;
			int speciality[2];
			menu5 << "Specialization of doctor (max 2)" << ui::endl
				  << "Choose from the following list: " << ui::endl;
			for(int i = 0; i <= GEN; ++i)
			{
				if(i <= 8)
				{
					menu5 << i << ". " << (body_parts)i << ui::endl;
				}
				else
				{
					inp_box << i << ". " << (body_parts)i << ui::endl;
				}
			}
			inp_box.settcolor_input(YELLOW);
			inp_box << "Enter the number corresponding to the required entry in the 2 fields below" << ui::endl;
			validate_menu::set_menu_limits(BRAIN, GEN);
			inp_box << (char)26;	inp_box >> validate_menu::input >> speciality[0];
			inp_box << (char)26;	inp_box >> validate_menu::input >> speciality[1];
			inp_box << ui::endl;
			inp_box.setexit_button("Submit");
			inp_box.loop();
			menu5.hide();
			inp_box.hide();
			window.f.display();
			doctor x(name, sex_choice, dob, adr, phn_no, salary, shift_start, shift_end, speciality[0], speciality[1], uid, pwd);
			temp = &x;
			id = x.get_id();
			break;
		}
		case 2:
		{
			nurse x(name, sex_choice, dob, adr, phn_no, salary, shift_start, shift_end, uid, pwd);
			temp = &x;
			id = x.get_id();
			break;
		}
		case 3:
		{
			receptionist x(name, sex_choice, dob, adr, phn_no, salary, shift_start, shift_end, uid, pwd);
			temp = &x;
			id = x.get_id();
			break;
		}
		case 4:
		{
			employee x(name, sex_choice, dob, adr, phn_no, salary, shift_start, shift_end);
			temp = &x;
			id = x.get_id();
			break;
		}
	}
	const int notice_height = 12;
	box notice(menu3.getcorner_top_left(), menu3.getwidth(), notice_height);
	notice.settcolor(GREEN);
	notice << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	if(!hospital::write_employee(temp))
	{
		notice.settcolor(RED);
		notice << "Employee addition unsuccessful!!";
		notice.setexit_button("Exit");
		notice.loop();
		notice.hide();
		return;
	}
	notice << "Employee added successfully!!" << ui::endl;
	notice.settcolor(WHITE);
	notice << "Hit the button below to display the details you entered: " << ui::endl;
	notice.settcolor(ui::tcolor);
	notice << ui::endl;
	notice.setexit_button("View employee...");
	notice.loop();
	notice.hide();
	view_emp(id);
}

void emp_mgmt::remove_emp()
{
	menu2_height = 10;
	menu2.setheight(menu2_height);
	menu2 << "Remove existing employee" << ui::endl;
	menu2.settcolor(ui::tcolor);
	menu2 << "Enter employee's id: ";
	unsigned long id;
	menu2.settcolor_input(YELLOW);
	menu2 >> id;
	menu2 << ui::endl;
	menu2.setexit_button("Submit");
	menu2.loop();
	menu2.hide();
	const int notice_height = 14;
	box notice(menu2.getcorner_top_left(), menu2.getwidth(), notice_height);
	notice.settcolor(GREEN);
	notice << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice.settcolor(WHITE);
	notice << "Hit the button below to display the details of the employee you want to remove: " << ui::endl;
	notice.settcolor(ui::tcolor);
	notice << ui::endl;
	notice.setexit_button("View employee...");
	notice.loop();
	notice.hide();
	if( !view_emp(id) )
	{
		return;
	}
	box notice2( notice.getcorner_top_left(), notice.getwidth(), notice.getheight() );
	notice2.settcolor(GREEN);
	notice2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice2.settcolor(WHITE);
	notice2 << "Are you sure you want to remove this employee?(y/n): " << ui::endl;
	char ch;
	notice2.settcolor_input(YELLOW);
	notice2 >> ch;
	notice2.settcolor(ui::tcolor);
	notice2 << ui::endl;
	notice2.setexit_button("Submit");
	notice2.loop();
	notice2.hide();
	if(ch == 'n' || ch == 'N')
	{
		return;
	}
	box notice3( notice.getcorner_top_left(), notice.getwidth(), notice.getheight() );
	notice3.settcolor(GREEN);
	notice3 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice3.settcolor(RED);
	str path;
	switch(id_to_emp::convert(id))
	{
		case INVALID:
			interface::log_this("emp_mgmt::remove_emp() : No file with zero id exists\nFunction aborted");
			notice3 << "Invalid ID supplied!! Check log" << ui::endl;
			notice3.setexit_button("Back");
			notice3.loop();
			notice3.hide();
			return;
		case OTHERS:
			sprintf(path, "employee/%lu", id);
			break;
		case DOCTOR:
			mkdir("employee/doctor");
			sprintf(path, "employee/doctor/%lu", id);
			break;
		case NURSE:
			mkdir("employee/nurse");
			sprintf(path, "employee/nurse/%lu", id);
			break;
		case RECEPTIONIST:
			mkdir("employee/receptionist");
			sprintf(path, "employee/receptionist/%lu", id);
			break;
	}
	int remove_status;
	str file;
	strcpy(file, path);
	strcat(file, "/base.dat");
	if( remove(file) == -1)
	{
		str log_str;
		sprintf(log_str, "emp_mgmt::remove_emp() : Failed to delete base.dat file of id %lu\nFunction aborted", id);
		interface::log_this(log_str);
		notice3 << "Failed to delete file of employee!!" << ui::endl;
		notice3.setexit_button("Back");
		notice3.loop();
		notice3.hide();
		return;
	}
	if( rmdir(path) == -1)
	{
		str log_str;
		sprintf(log_str, "emp_mgmt::remove_emp() : Failed to delete folder of id %lu", id);
		interface::log_this(log_str);
	}
	notice3.settcolor(GREEN);
	notice3 << "Employee deletion successful!!" << ui::endl;
	notice3.setexit_button("Back");
	notice3.loop();
	notice3.hide();
}

void emp_mgmt::edit_emp()
{
	menu2_height = 10;
	menu2.setheight(menu2_height);
	menu2 << "Edit employee data" << ui::endl;
	menu2.settcolor(ui::tcolor);
	menu2 << "Step 1: Enter employee's id: ";
	unsigned long id;
	menu2.settcolor_input(YELLOW);
	menu2 >> id;
	menu2 << ui::endl;
	menu2.setexit_button("Submit");
	menu2.loop();
	menu2.hide();
	void * temp = malloc( sizeof(doctor) );	//as doctor has the greatest size among employee, doctor, nurse and receptionist classes
	if(temp == NULL)
	{
		interface::log_this("emp_mgmt::edit_emp() : Not enough memory to allocate buffer void * temp = malloc( sizeof(doctor) )");
		interface::error("Out of memory!! Check log");
		getch();
		return;
	}
	if(!hospital::get_employee_by_id(id, temp))
	{
		interface::error("ID not found or error while reading from file!");
		getch();
		return;
	}
	const int notice_height = 14;
	box notice(menu2.getcorner_top_left(), menu2.getwidth(), notice_height);
	notice.settcolor(GREEN);
	notice << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice.settcolor(WHITE);
	notice << "Details of the employee will now be shown with the existing data filled. "
		   << "Change the data fields that you require to change, and leave the other data fields as they are. "
		   << "When you are finished, press Submit to submit the new details." << ui::endl;
	notice.settcolor(ui::tcolor);
	notice << ui::endl;
	notice.setexit_button("View employee...");
	notice.loop();
	notice.hide();
	const int menu3_height = 18;
	box menu3( menu2.getcorner_top_left(), menu2.getwidth(), menu3_height );
	menu3.settcolor(GREEN);
	menu3 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	menu3.settcolor(WHITE);
	menu3 << "Edit employee data" << ui::endl;
	menu3.settcolor(ui::tcolor);
	menu3 << "Step 2: Edit employee details" << ui::endl << ui::endl;
	str name, dob_str, adr_hno, adr_street, adr_city, adr_dist, adr_state, shift_start_str, shift_end_str, uid, pwd;
	unsigned sex_choice;
	Date dob;
	address adr;
	phone phn_no;
	unsigned long salary;
	Time shift_start, shift_end;
	employee *e = (employee *) temp;
	menu3.settcolor_input(YELLOW);
	menu3 << "Name: ";
	menu3.setdefault( e->get_name() );
	menu3 >> name;
	menu3 << "Sex: 1. Male | 2. Female | 3. Transsexual" << ui::endl
		  << "---- Enter your choice: ";
	validate_menu::set_menu_limits(1, 3);
	menu3.setdefault( e->get_sex() + 1 );
	menu3 >> validate_menu::input >> (int)sex_choice;
	menu3 << "Date of Birth(DD/MM/YYYY): ";
	str default_dob_str;
	sprintf(default_dob_str, "%u/%u/%u", e->get_dob().day, e->get_dob().month, e->get_dob().year);
	menu3.setdefault( default_dob_str );
	menu3 >> hospital::date_validity >> dob_str;
	menu3 << "Address: " << ui::endl;
	menu3 << (char)26 << "House no.: ";
	menu3.setdefault( e->get_address().house_no );
	menu3 >> adr_hno;
	menu3 << (char)26 << "Street: ";
	menu3.setdefault( e->get_address().street );
	menu3 >> adr_street;
	menu3 << (char)26 << "City: ";
	menu3.setdefault( e->get_address().city );
	menu3 >> adr_city;
	menu3 << (char)26 << "District: ";
	menu3.setdefault( e->get_address().district );
	menu3 >> adr_dist;
	menu3 << (char)26 << "State: ";
	menu3.setdefault( e->get_address().state );
	menu3 >> adr_state;
	menu3 << "Phone no.: ";	
	menu3.setdefault( e->get_phone() );
	menu3 >> phn_no;
	menu3 << "Salary: ";
	menu3.setdefault( e->get_salary() );
	menu3 >> salary;
	menu3 << "Shift timings: Starts - (HH:MM:SS)";
	str default_shift_str;
	sprintf(default_shift_str, "%u:%u:%u", e->get_shift(START).hour, e->get_shift(START).minute, e->get_shift(START).second );
	menu3.setdefault( default_shift_str );
	menu3 >> hospital::time_validity >> shift_start_str;
	menu3 << "-------------: Ends - (HH:MM:SS)";
	sprintf(default_shift_str, "%u:%u:%u", e->get_shift(END).hour, e->get_shift(END).minute, e->get_shift(END).second );
	menu3.setdefault( default_shift_str );
	menu3 >> hospital::time_validity >> shift_end_str;
	menu3.setexit_button("Submit");
	menu3.loop();
	--sex_choice;
	dob = hospital::str_to_date(dob_str);
	adr = address(adr_hno, adr_street, adr_city, adr_dist, adr_state);
	shift_start = hospital::str_to_time(shift_start_str);
	shift_end = hospital::str_to_time(shift_end_str);
	menu3.hide();
	e->set_name(name);
	e->set_sex(sex_choice);
	e->set_dob(dob);
	e->set_address(adr);
	e->set_phone(phn_no);
	e->set_salary(salary);
	e->set_shift(START, shift_start);
	e->set_shift(END, shift_end);
	if(id_to_emp::convert(id) == DOCTOR)
	{
		coord c(1, 4);
		doctor *d = (doctor *)temp;
		box menu4(c, (ui::scr_width / 2), ui::scr_height - 5);
		box inp_box(( c + coord((ui::scr_width / 2) - 1, 0)), (ui::scr_width / 2) + 1, ui::scr_height - 5);
		menu4.f << ( ui::top | ui::left ) << (char)204
				  << ( ui::bottom | ui::left ) << (char)204
				  << ( ui::top | ui::right ) << (char)203
				  << ( ui::bottom | ui::right ) << (char)202;
		menu4.f.display();
		inp_box.f << ( ui::top | ui::left ) << (char)203
			   << ( ui::bottom | ui::left ) << (char)202
			   << ( ui::top | ui::right ) << (char)185
			   << ( ui::bottom | ui::right ) << (char)185;
		inp_box.f.display();
		menu4 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
		menu4.settcolor(WHITE);
		menu4 << "Edit employee data" << ui::endl;
		menu4.settcolor(ui::tcolor);
		menu4 << "Step 3: Edit doctor details" << ui::endl << ui::endl;
		int speciality[2];
		menu4 << "Specialization of doctor (max 2)" << ui::endl
			  << "Choose from the following list: " << ui::endl;
		for(int i = 0; i <= GEN; ++i)
		{
			if(i <= 8)
			{
				menu4 << i << ". " << (body_parts)i << ui::endl;
			}
			else
			{
				inp_box << i << ". " << (body_parts)i << ui::endl;
			}
		}
		inp_box.settcolor_input(YELLOW);
		inp_box << "Enter the number corresponding to the required entry in the 2 fields below" << ui::endl;
		validate_menu::set_menu_limits(BRAIN, GEN);
		inp_box << (char)26;	inp_box.setdefault(d->get_speciality()[0]);	inp_box >> validate_menu::input >> speciality[0];
		inp_box << (char)26;	inp_box.setdefault(d->get_speciality()[1]);	inp_box >> validate_menu::input >> speciality[1];
		inp_box << ui::endl;
		inp_box.setexit_button("Submit");
		inp_box.loop();
		menu4.hide();
		inp_box.hide();
		window.f.display();
		d->set_speciality(speciality);
	}
	const int notice2_height = 12;
	box notice2(menu3.getcorner_top_left(), menu3.getwidth(), notice2_height);
	notice2.settcolor(GREEN);
	notice2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	if(!hospital::write_employee(temp))
	{
		notice2.settcolor(RED);
		notice2 << "Employee edit unsuccessful!!";
		notice2.setexit_button("Exit");
		notice2.loop();
		notice2.hide();
		return;
	}
	notice2 << "Employee edited successfully!!" << ui::endl;
	notice2.settcolor(WHITE);
	notice2 << "Hit the button below to display the details you entered: " << ui::endl;
	notice2.settcolor(ui::tcolor);
	notice2 << ui::endl;
	notice2.setexit_button("View employee...");
	notice2.loop();
	notice2.hide();
	view_emp(id);
}

void emp_mgmt::pay_emp()
{
	menu2_height = 10;
	menu2.setheight(menu2_height);
	menu2 << "Pay salary to individual employee" << ui::endl;
	menu2.settcolor(ui::tcolor);
	menu2 << "Enter employee's id: ";
	unsigned long id;
	menu2.settcolor_input(YELLOW);
	menu2 >> id;
	menu2 << ui::endl;
	menu2.setexit_button("Submit");
	menu2.loop();
	menu2.hide();
	const int notice_height = 14;
	box notice(menu2.getcorner_top_left(), menu2.getwidth(), notice_height);
	notice.settcolor(GREEN);
	notice << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice.settcolor(WHITE);
	notice << "Hit the button below to display the details of the employee you want to pay salary to: " << ui::endl;
	notice.settcolor(ui::tcolor);
	notice << ui::endl;
	notice.setexit_button("View employee...");
	notice.loop();
	notice.hide();
	if( !view_emp(id) )
	{
		return;
	}
	box notice2( notice.getcorner_top_left(), notice.getwidth(), notice.getheight() );
	notice2.settcolor(GREEN);
	notice2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice2.settcolor(WHITE);
	notice2 << "Are you sure you want to pay salary to this employee?(y/n): " << ui::endl;
	char ch;
	notice2.settcolor_input(YELLOW);
	notice2 >> ch;
	notice2.settcolor(ui::tcolor);
	notice2 << ui::endl;
	notice2.setexit_button("Submit");
	notice2.loop();
	notice2.hide();
	if(ch == 'n' || ch == 'N')
	{
		return;
	}
	box notice3( notice.getcorner_top_left(), notice.getwidth(), notice.getheight() );
	notice3.settcolor(GREEN);
	notice3 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice3.settcolor(RED);
	if( !hospital::pay_salary(id, system::get_date(), system::get_time()) )
	{
		notice3 << "Failed to pay salary to the employee! Check log";
		notice3.setexit_button("Back");
		notice3.loop();
		notice3.hide();
		return;
	}
	notice3.settcolor(GREEN);
	notice3 << "Pay salary successful!!" << ui::endl;
	notice3.setexit_button("Back");
	notice3.loop();
	notice3.hide();
}

void emp_mgmt::pay_all()
{
	char ch;
	menu2_height = 11;
	menu2.setheight(menu2_height);
	menu2 << "Pay salary to all employees" << ui::endl;
	menu2.settcolor(ui::tcolor);
	menu2 << "Are you sure you want to pay salary to all employees?(y/n): ";
	menu2.settcolor_input(YELLOW);
	menu2 >> ch;
	menu2 << ui::endl;
	menu2.setexit_button("Submit");
	menu2.loop();
	menu2.hide();
	if(ch == 'n' || ch == 'N')
	{
		return;
	}
	const int notice_height = 10;
	box notice( menu2.getcorner_top_left(), menu2.getwidth(), notice_height );	notice.hide();
	box notice2( notice.getcorner_top_left(), notice.getwidth(), notice.getheight() );
	notice2.settcolor(GREEN);
	notice2 << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice2.hide();	notice.display();
	notice.settcolor(GREEN);
	notice << ui::centeralign << "Employee Management" << ui::endl << ui::endl;
	notice.settcolor(ui::tcolor);
	notice << "Pay all salaries in progress..." << ui::endl;
	if( !hospital::pay_all_salaries() )
	{
		notice.hide();
		notice2.settcolor(RED);
		notice2 << "Failed to pay salary to all employees! Check log";
		notice2.setexit_button("Back");
		notice2.loop();
		notice2.hide();
		return;
	}
	notice.hide();	notice2.display();
	notice2 << "Pay all salaries successful!!" << ui::endl;
	notice2.setexit_button("Back");
	notice2.loop();
	notice2.hide();
}