#ifndef EMPLOYEE_ANY_H
#define EMPLOYEE_ANY_H

#include "employee_hourly.h"
#include "employee_monthly.h"
#include "employee_salesman.h"

class employee_any
{
private:
  job_id m_id;
  QString m_first_name;
  QString m_last_name;
  ssn_handle m_ssn;
  int m_salary;
  int m_hours_done;
  bool m_has_bonus;
  double m_bonus_coef;
  int m_realized_outcome;
public:
  employee_any ();
  ~employee_any ();

  employee_monthly  to_monthly () const;
  employee_hourly   to_hourly () const;
  employee_salesman to_salesman () const;

  void set_id (const job_id id);
  void set_fname (const QString &name);
  void set_lname (const QString &name);
  void set_ssn (const QString &str);
  void set_salary (const int val);
  void set_hours_done (const int val);
  void set_has_bonus (const bool val);
  void set_bonus_coef (const double val);
  void set_realized_outcome (const int val);
};

#endif // EMPLOYEE_ANY_H
