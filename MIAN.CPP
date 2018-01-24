#include "iface.hpp"
#include <conio.h>
#include "hosp.hpp"
#include "emp.hpp"

void main()
{
	clrscr();
/*///////////////Administrator object creator///////////
	address yay("", "", "", "", "");
	employee x("Administrator", 3, Date(), yay, "", 0, Time(), Time(), "admin", "password");
	hospital::write_employee(&x);
///////////////////////////////////////////////////////*/

	interface::log_this("Program initiated\n\n");

	interface::init();

	interface::log_this("Program terminated\n\n");
}