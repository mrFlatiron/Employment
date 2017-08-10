#include "employee_salesman.h"

employee_salesman::employee_salesman ()
 : employee_monthly ()
{
  m_bonus_condition = false;
}

employee_salesman::~employee_salesman ()
{

}

compensation employee_salesman::compensation_type () const
{
  return compensation::salesman;
}

bool employee_salesman::has_bonus () const
{
  return m_bonus_condition;
}

double employee_salesman::bonus_coef() const
{
  return m_bonus_coef;
}

int employee_salesman::realized_outcome() const
{
  return m_realized_outcome;
}

void employee_salesman::set_bonus_state (const bool state)
{
  m_bonus_condition = state;
  try_data_changed (employee::field::has_bonus);
}

void employee_salesman::set_bonus_coef(const double coef)
{
  m_bonus_coef = coef;
  try_data_changed (employee::field::bonus_percent);
}

void employee_salesman::set_realized_outcome (const int outcome)
{
  m_realized_outcome  = outcome;
  try_data_changed (employee::field::realized_outcome);
}

void employee_salesman::inc_realized_outcome (const int inc)
{
  m_realized_outcome += inc;
  try_data_changed (employee::field::realized_outcome);
}

void employee_salesman::set_realized_outcome_zero ()
{
  m_realized_outcome = 0;
  try_data_changed (employee::field::realized_outcome);
}

