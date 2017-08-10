#ifndef EMPLOYEE_UTILS_H
#define EMPLOYEE_UTILS_H

#include <QString>

enum class add_emp_field
{
  first_name,
  last_name,
  ssn,
  salary, // wage
  bonus_percent,
  COUNT
};

namespace employee
{
  enum class field
  {
    compensation,
    first_name,
    last_name,
    ssn,
    salary,
    hours_done,
    has_bonus,
    bonus_percent,
    realized_outcome,
    TOTAL
  };

  bool is_field_valid (field f, const QString &str);

  QString error_string (field f);

  field enum_convert (add_emp_field aef);

  constexpr employee::field enum_end (employee::field) {return employee::field::TOTAL;}
}



#endif // EMPLOYEE_UTILS_H
