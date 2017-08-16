#include "employee_any.h"

employee_any::employee_any ()
{
  m_id = -1;
}

employee_any::~employee_any ()
{

}

employee_monthly employee_any::to_monthly () const
{
  employee_monthly emp;
  emp.set_id (m_id);
  emp.set_first_name (m_first_name);
  emp.set_last_name (m_last_name);
  emp.set_salary (m_salary);
  emp.set_ssn (m_ssn);
  return emp;
}

employee_hourly employee_any::to_hourly () const
{
  employee_hourly emp;
  emp.set_id (m_id);
  emp.set_first_name (m_first_name);
  emp.set_last_name (m_last_name);
  emp.set_wage (m_salary);
  emp.set_ssn (m_ssn);
  emp.set_hours_done (m_hours_done);
  return emp;
}

employee_salesman employee_any::to_salesman () const
{
  employee_salesman emp;
  emp.set_id (m_id);
  emp.set_first_name (m_first_name);
  emp.set_last_name (m_last_name);
  emp.set_salary (m_salary);
  emp.set_ssn (m_ssn);
  emp.set_bonus_state (m_has_bonus);
  emp.set_realized_outcome (m_realized_outcome);
  emp.set_bonus_coef (m_bonus_coef);
  return emp;
}

void employee_any::set_id (const job_id id)
{
  m_id = id;
}

void employee_any::set_fname (const QString &name)
{
  m_first_name = name;
}

void employee_any::set_lname (const QString &name)
{
  m_last_name = name;
}

void employee_any::set_ssn (const QString &str)
{
  m_ssn = ssn_handle (str);
}

void employee_any::set_salary (const int val)
{
  m_salary = val;
}

void employee_any::set_hours_done (const int val)
{
  m_hours_done = val;
}

void employee_any::set_has_bonus (const bool val)
{
  m_has_bonus = val;
}

void employee_any::set_bonus_coef (const double val)
{
  m_bonus_coef = val;
}

void employee_any::set_realized_outcome (const int val)
{
  m_realized_outcome = val;
}


