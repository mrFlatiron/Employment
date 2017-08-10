#include "gui/main_window.h"
#include <QApplication>
#include <memory>

#include "common/enum_range.h"
#include "common/enum_misc.h"

#include "kernel/employee/employee_list.h"



int main (int argc, char *argv[])
{
  employee_list global_employee_list;

  employee_salesman emp_s;
  emp_s.set_first_name ("Slava");
  emp_s.set_last_name ("Nazarenko");
  emp_s.set_ssn (ssn_handle  ("123456789"));
  emp_s.set_salary (250000);
  emp_s.set_bonus_coef (0.15);
  emp_s.set_bonus_state (true);
  emp_s.set_realized_outcome (100000);

  employee_hourly emp_h;
  emp_h.set_first_name ("Darya");
  emp_h.set_last_name ("Brusova");
  emp_h.set_ssn (ssn_handle ("987654321"));
  emp_h.set_wage (203240);
  emp_h.set_hours_done (10);

  employee_hourly emp_h1;
  emp_h1.set_first_name ("Darya");
  emp_h1.set_last_name ("Brusova");
  emp_h1.set_ssn (ssn_handle ("111111111"));
  emp_h1.set_wage (203240);
  emp_h1.set_hours_done (10);

  employee_hourly emp_h2;
  emp_h2.set_first_name ("Darya");
  emp_h2.set_last_name ("Brusova");
  emp_h2.set_ssn (ssn_handle ("222222222"));
  emp_h2.set_wage (203240);
  emp_h2.set_hours_done (10);

  global_employee_list.add_employee (std::move (emp_s));
  global_employee_list.add_employee (std::move (emp_h));
  global_employee_list.add_employee (std::move (emp_h1));
  global_employee_list.add_employee (std::move (emp_h2));

  QApplication a(argc, argv);
  main_window w (&global_employee_list);
  w.exec ();
  return 0;
}
