/*!
 \file iface.hpp
 \brief Contains prototypes of the functions managing the interface of the program
*/

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "ui/ui.hpp"

//!Class containing all the functions that make up the interface of the program
class interface{
	public:
		static void init(); //!>The main interface function of the program; this is the functions that runs throughout the program
		static int login_screen();//!>Login screen interface
		static int menu();//!>The first main menu interface(For administrator employee only)
		static void patient_management(); //!>Patient management interface(for receptionist employees only)
		static void employee_management(); //!>Employee management interface(for administrator employees only)
		static void stock_management(); //!>Stock management interface(for administrator employees only)
		
		static void employee_screen(unsigned long); //!>The interface for non-administrator employees

		static void error(char*); //!>Prints an error message at the footer of interface::window
		static void clear_error(); //!>Clears the error message at the footer of interface::window
		
		static int log_this(char *); //!>Logs a message string into a file log.txt

	protected:
		interface(); //!>Objects of this class shouldn't be created
		/*!
		For creating a validation function to use in menus
		to validate the choice input of the menu option to be accessed
		*/
		class validate_menu
		{
				static int lowest_choice, greatest_choice; //!>The lower and upper limit of the choices of a menu
				validate_menu(); //!>Objects of this class shouldn't be created
			public:
				static int input(const char *); //!>The validation function that will be passed as an argument to box::operator>>()
				static void set_menu_limits(int, int); //!>Setter; sets lowest_choice and greatest_choice
		};
		/*!Creates a back_func that can be passed as an argument to box::setback_func()*/
		class back_func
		{
				back_func(); //!>Objects of this class shouldn't be created
			public:
				static int backbit; //!>1, if shift + bkspc is pressed, 0 otherwise
				static int set_backbit(); //!>Setter, passed as an argument to box::setback_func()
		};
		static box window; //!>The main outer window box
};

//!Class containing all the functions that make up the interface of Employee management
class emp_mgmt : public interface
{
	public:	
		static void view_emp(); //!>Interface of View Employee
		static int view_emp(unsigned long); //!>Creates the interface that shows the details of an employee with a particular ID
		static void add_emp(); //!>Interface of Add Employee
		static void remove_emp(); //!>Interface of Remove Employee
		static void edit_emp(); //!>Interface of Edit Employee
		static void pay_emp(); //!>Interface of Pay Employee
		static void pay_all(); //!>Interface of Pay All Employees
	private:
		emp_mgmt(); //!>Objects of this class shouldn't be created
};

#endif /* INTERFACE_HPP */