#include <fstream.h>
#include "base.hpp"
#include "iface.hpp"
#include "hosp.hpp"
#include "emp.hpp"

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
