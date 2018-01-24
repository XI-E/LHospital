#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "ui/ui.hpp"

class interface{
	public:
		static void init();
		static int login_screen();
		static int menu();
		static void patient_management();
		static void employee_management();
		static void stock_management();
		
		static void employee_screen(unsigned long);

		static void error(char*);
		static void clear_error();
		
		static int log_this(char *);

	protected:
		interface();
		class validate_menu			//for creating a validation function to use in menus
		{							//to validate the choice input of the menu option to be accessed
				static int lowest_choice, greatest_choice;
				validate_menu();
			public:
				static int input(const char *);
				static void set_menu_limits(int, int);
		};
		static box window;
};

class emp_mgmt : public interface
{
	public:	
		static box menu2;
		static void view_emp();
		static int view_emp(unsigned long);
		static void add_emp();
		static void remove_emp();
		static void edit_emp();
		static void pay_emp();
		static void pay_all();
	private:
		emp_mgmt();
		static int menu2_height;
};

#endif /* INTERFACE_HPP */