#include <fstream.h>
#include "base.hpp"
#include "iface.hpp"
#include "hosp.hpp"
#include "emp.hpp"

////////////////////////////////////////////
//////// Function definitions for interface

void interface::stock_management(){
	coord c(ui::scr_width / 3, ui::scr_height / 3);
	box menu (c, ui::scr_width / 3, ui::scr_height / 2.2);

	int ch = 0;

	menu << "1. Sale"
			<< ui::endl << "2. Purchase"
			<< ui::endl << "3. Stock check"
			<< ui::endl << "4. Go to main menu"
			<< ui::endl << ui::endl << "Choice : ";
	menu.setdefault(1);
	menu.settcolor_input(YELLOW);
	validate_menu::set_menu_limits(1, 4);
	menu >> validate_menu::input >> ch;

	menu << ui::endl;
	menu.setexit_button("Submit");

	menu.loop();
	menu.hide();

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

int interface::back_func::set_backbit()
{
	backbit = 1;
	return 1;
}

int interface::back_func::backbit = 0;

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
	char text[300];
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

interface::interface(){}

box interface::window;