#include "sqlite_employee_db.h"
#include "kernel/employee/employee_any.h"
#include <QDebug>

sqlite_employee_db::sqlite_employee_db ()
{
  base::set_table_name ("employee");
}

sqlite_employee_db::~sqlite_employee_db ()
{

}

void sqlite_employee_db::get_list_from_db (employee_list &list)
{
  base::set_connection_name ("load");

  if (!base::connect ())
    {
      DEBUG_PAUSE ("Why is that?");
      return;
    }

  QSqlDatabase &db = base::db_handle ();
  QSqlQuery selected (db);
  if (!selected.prepare (base::simple_query (sqlite_simple_query::select)))
    {
      return;
    }

  if (!selected.exec ())
    {
      return;
    }
  while (selected.next ())
    {
      compensation type = enum_cast<compensation> (selected.value (enum_cast<int> (employee::field::compensation)).toInt ());
      employee_any emp;

      for (employee::field f : enum_range<employee::field> ())
        {
          int num = enum_cast<int> (f);

          QVariant val = selected.value (num);
          switch (f)
            {
            case employee::field::id:
              emp.set_id (val.toInt ());
              break;
            case employee::field::compensation:
              break;
            case employee::field::first_name:
              emp.set_fname (val.toString ());
              break;
            case employee::field::last_name:
              emp.set_lname (val.toString ());
              break;
            case employee::field::ssn:
              emp.set_ssn (val.toString ());
              break;
            case employee::field::salary:
              emp.set_salary (val.toInt ());
              break;
            case employee::field::hours_done:
              emp.set_hours_done (val.toInt ());
              break;
            case employee::field::has_bonus:
              emp.set_has_bonus (val.toBool ());
              break;
            case employee::field::bonus_percent:
              emp.set_bonus_coef (val.toDouble () / 100);
              break;
            case employee::field::realized_outcome:
              emp.set_realized_outcome (val.toInt ());
              break;
            case employee::field::COUNT:
              DEBUG_PAUSE ("Shouldn't happen");
              return;
            }
        }
      job_id check;
      switch (type)
        {
        case compensation::monthly:
          check = list.add_employee (emp.to_monthly (), true);
          break;
        case compensation::hourly:
          check = list.add_employee (emp.to_hourly (), true);
          break;
        case compensation::salesman:
          check = list.add_employee (emp.to_salesman (), true);
          break;
        case compensation::COUNT:
          DEBUG_PAUSE ("Shouldn't happen");
          return;
        }
      if (!check)
        DEBUG_PAUSE ("Why is that?");
    }
  db.close ();
  return;
}

void sqlite_employee_db::save_list (const employee_list &list)
{
  base::set_connection_name ("save");

  if (!base::db_handle ().open ())
    {
      if (!base::connect ())
        {
          DEBUG_PAUSE ("Why is that?");
          return;
        }
    }

  QSqlDatabase &db = base::db_handle ();

  QSqlQuery pragma (db);
  QSqlQuery drop (db);
  QSqlQuery create (db);


  if (!pragma.prepare (base::simple_query (sqlite_simple_query::sync_pragma_off))
      || !pragma.exec ())
    {
      DEBUG_PAUSE ("Why is that?");
//      return;
    }
  if (!drop.prepare (base::simple_query (sqlite_simple_query::drop))
      || !drop.exec ())
    {
      DEBUG_PAUSE ("Why is that?");
//      return;
    }
  if (!create.prepare (base::simple_query (sqlite_simple_query::create))
      || !create.exec ())
    {
      DEBUG_PAUSE ("Why is that?");
//      return;
    }

  QSqlQuery insert_stmt (db);
  insert_stmt.prepare (base::simple_query (sqlite_simple_query::insert));

  std::vector<job_id> all_ids = list.all_ids ();

  for (job_id id : all_ids)
    {
      const employee_base *emp = list.get (id);
      for (employee::field f : enum_range<employee::field> ())
        {
          switch (f)
            {
            case employee::field::id:
              insert_stmt.addBindValue (emp->id ());
              break;
            case employee::field::compensation:
              insert_stmt.addBindValue (enum_cast<int> (emp->compensation_type ()));
              break;
            case employee::field::first_name:
              insert_stmt.addBindValue (emp->first_name ());
              break;
            case employee::field::last_name:
              insert_stmt.addBindValue (emp->last_name ());
              break;
            case employee::field::ssn:
              insert_stmt.addBindValue (emp->ssn ().simple ());
              break;
            case employee::field::salary:
              switch (emp->compensation_type ())
                {
                case compensation::monthly:
                case compensation::salesman:
                  insert_stmt.addBindValue (static_cast<const employee_monthly *> (emp)->salary ());
                  break;
                case compensation::hourly:
                  insert_stmt.addBindValue (static_cast<const employee_hourly *> (emp)->wage ());
                  break;
                case compensation::COUNT:
                  DEBUG_PAUSE ("Shouldn't happen");
                  break;
                }
              break;
            case employee::field::hours_done:
              switch (emp->compensation_type ())
                {
                case compensation::monthly:
                case compensation::salesman:
                  insert_stmt.addBindValue (0);
                  break;
                case compensation::hourly:
                  insert_stmt.addBindValue (static_cast<const employee_hourly *> (emp)->hours_done ());
                  break;
                case compensation::COUNT:
                  DEBUG_PAUSE ("Shouldn't happen");
                  break;
                }
              break;
            case employee::field::has_bonus:
              switch (emp->compensation_type ())
                {
                case compensation::monthly:
                case compensation::hourly:
                  insert_stmt.addBindValue (0);
                  break;
                case compensation::salesman:
                  insert_stmt.addBindValue (static_cast<const employee_salesman *> (emp)->has_bonus ());
                  break;
                case compensation::COUNT:
                  DEBUG_PAUSE ("Shouldn't happen");
                  break;
                }
              break;
            case employee::field::bonus_percent:
              switch (emp->compensation_type ())
                {
                case compensation::monthly:
                case compensation::hourly:
                  insert_stmt.addBindValue (0);
                  break;
                case compensation::salesman:
                  insert_stmt.addBindValue (static_cast<const employee_salesman *> (emp)->bonus_coef () * 100);
                  break;
                case compensation::COUNT:
                  DEBUG_PAUSE ("Shouldn't happen");
                  break;
                }
              break;
            case employee::field::realized_outcome:
              switch (emp->compensation_type ())
                {
                case compensation::monthly:
                case compensation::hourly:
                  insert_stmt.addBindValue (0);
                  break;
                case compensation::salesman:
                  insert_stmt.addBindValue (static_cast<const employee_salesman *> (emp)->realized_outcome ());
                  break;
                case compensation::COUNT:
                  DEBUG_PAUSE ("Shouldn't happen");
                  break;
                }
              break;
            case employee::field::COUNT:
              DEBUG_PAUSE ("Shouldn't happen");
              break;
            }
        }
      if (!insert_stmt.exec ())
        {
          DEBUG_PAUSE ("Why is that?");
          continue;
        }
    }
}
