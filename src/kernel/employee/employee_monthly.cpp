#include "employee_monthly.h"

employee_monthly::employee_monthly ()
  : employee_base ()
{

}

employee_monthly::~employee_monthly ()
{

}

compensation employee_monthly::compensation_type () const
{
  return compensation::monthly;
}

int employee_monthly::salary() const
{
  return m_salary;
}

void employee_monthly::set_salary(const int i)
{
  m_salary = i;
  try_data_changed (employee::field::salary);
}
