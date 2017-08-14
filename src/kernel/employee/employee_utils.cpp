#include "employee_utils.h"
#include <QRegExp>
#include "kernel/ssn_handle.h"
#include "kernel/debug_utils.h"

bool employee::is_field_valid (employee::field f, const QString &str)
{
  switch (f)
    {
    case employee::field::compensation:
      return QRegExp ("^\\s*(Hourly|Salesman|Monthly)\\s*$",
                      Qt::CaseInsensitive).indexIn (str) == 0;
    case employee::field::first_name:
    case employee::field::last_name:
       return QRegExp ("^[a-zA-Z]{1,100}$").indexIn (str) == 0;
    case employee::field::ssn:
      return ssn_handle (str).is_valid ();
    case employee::field::salary:
    case employee::field::hours_done:
    case employee::field::realized_outcome:
      {
        bool check;
        str.toInt (&check);
        return check;
      }
    case employee::field::bonus_percent:
      {
        bool check;
        double d = str.toDouble (&check);
        if (d < 0)
          check = false;
        return check;
      }
    case employee::field::has_bonus:
      {
        return QRegExp ("^\\s*(Yes|No|true|false)\\s*$",
                        Qt::CaseInsensitive).indexIn (str) == 0;
      }
    case employee::field::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return false;
    }
  return false;
}

QString employee::error_string (employee::field f)
{
  switch (f)
    {
    case field::compensation:
      return "Wrong compensation type";
    case field::first_name:
    case field::last_name:
      return "Names should be written in Latin and be at "
                      "least 1 and at most 100 characters long";
    case field::ssn:
      return "Social Security Number is incorrect";
    case field::salary:
      return "Salary/Wage should be int > 0";
    case field::hours_done:
      return "Hours done must be integer";
    case field::has_bonus:
      return "Has bonus field must be yes/no or true/false";
    case field::bonus_percent:
      return "Bonus percent must be a real number";
    case field::realized_outcome:
      return "Realized outcome must be integer";
    case field::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return "";
    }
  return "";
}

employee::field employee::enum_convert (add_emp_field aef)
{
  switch (aef)
    {
    case add_emp_field::first_name:
      return field::first_name;
    case add_emp_field::last_name:
      return field::last_name;
    case add_emp_field::ssn:
      return field::ssn;
    case add_emp_field::salary:
      return field::salary;
    case add_emp_field::bonus_percent:
      return field::bonus_percent;
    case add_emp_field::COUNT:
      return field::COUNT;
    }
  return field::COUNT;
}
