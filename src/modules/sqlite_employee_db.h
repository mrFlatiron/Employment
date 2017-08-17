#ifndef SQLITE_EMPLOYEE_DB_H
#define SQLITE_EMPLOYEE_DB_H

#include "sqlite_table_configurer.h"
#include "kernel/employee/employee_list.h"
#include "kernel/employee/employee_utils.h"



class sqlite_employee_db : private sqlite_table_configurer<employee::field>
{
private:
  using base = sqlite_table_configurer<employee::field>;
public:
  sqlite_employee_db ();
  ~sqlite_employee_db ();

  using base::set_db_name;

  void get_list_from_db (employee_list &emp_list);
  void save_list (const employee_list &list);
};

#endif // SQLITE_EMPLOYEE_DB_H
