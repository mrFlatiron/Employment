#include "employee_hourly.h"

employee_hourly::employee_hourly () :
  employee_base ()
{

}

employee_hourly::~employee_hourly ()
{

}

compensation employee_hourly::compensation_type () const
{
  return compensation::hourly;
}

int employee_hourly::wage () const
{
  return m_wage;
}

int employee_hourly::hours_done () const
{
  return m_hours_done;
}

void employee_hourly::set_wage (const int wage)
{
  m_wage = wage;
  try_data_changed (employee::field::salary);
}

void employee_hourly::set_hours_done (const int done)
{
  m_hours_done = done;
  try_data_changed (employee::field::hours_done);
}

void employee_hourly::inc_hours_done (const int inc)
{
  m_hours_done += inc;
  try_data_changed (employee::field::hours_done);
}
