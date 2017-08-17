#include "gui/main_window.h"
#include <QApplication>
#include <memory>

#include "common/enum_range.h"
#include "common/enum_misc.h"

#include "kernel/employee/employee_list.h"
#include "modules/sqlite_employee_db.h"


int main (int argc, char *argv[])
{
  sqlite_employee_db db;
  db.set_db_name ("employies");

  employee_list global_employee_list;
  db.get_list_from_db (global_employee_list);

  QApplication a(argc, argv);
  main_window w (&global_employee_list);
  w.exec ();

  db.save_list (global_employee_list);
  return 0;
}
