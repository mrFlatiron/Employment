#include "employee_base.h"
#include "kernel/debug_utils.h"

employee_base::employee_base ()
{
  set_id (-1);
}

employee_base::~employee_base()
{

}

employee_base::employee_base (const employee_base &rhs)
  : QObject (),
    m_job_id (rhs.m_job_id),
    m_first_name (rhs.m_first_name),
    m_last_name (rhs.m_last_name),
    m_ssn (rhs.m_ssn)
{
}

employee_base::employee_base (const QString &first_name, const QString &last_name, const ssn_handle &ssn)
{
  set_first_name (first_name);
  set_last_name (last_name);
  set_ssn (ssn);
  set_id (-1);
}

QString employee_base::full_name () const
{
  return m_last_name + m_first_name;
}

QString employee_base::first_name () const
{
  return m_first_name;
}

QString employee_base::last_name () const
{
  return m_last_name;
}

ssn_handle employee_base::ssn () const
{
  return m_ssn;
}

job_id employee_base::id () const
{
  return m_job_id;
}

void employee_base::begin_transaction ()
{
  if (m_transaction.in_progress)
    {
      DEBUG_PAUSE ("Already in transaction");
      return;
    }
  m_transaction.begin ();
}

void employee_base::end_transaction ()
{
  if (!m_transaction.in_progress)
    {
      DEBUG_PAUSE ("No transaction in progress");
      return;
    }
  data_changed (id (), m_transaction.changed);
  m_transaction.end ();
}

void employee_base::try_data_changed (employee::field changed_field)
{
  if (m_transaction.in_progress)
    m_transaction.changed.set (changed_field, true);
  else
    {
      enum_bitset<employee::field> bits;
      bits.set (changed_field, true);
      data_changed (id (), bits);
    }
}

void employee_base::set_first_name (const QString &first_name)
{
  m_first_name = first_name;
  try_data_changed (employee::field::first_name);
}

void employee_base::set_last_name (const QString &last_name)
{
  m_last_name = last_name;
  try_data_changed (employee::field::last_name);
}

void employee_base::set_ssn (const ssn_handle &ssn)
{
  m_ssn = ssn;
  try_data_changed (employee::field::ssn);
}

// must be done only once, during add. So no signal is emitted
void employee_base::set_id (const job_id id)
{
  m_job_id = id;
}

const char *enum_to_string (compensation c)
{
  switch (c)
    {
    case compensation::monthly:
      return "Monthly";
    case compensation::hourly:
      return "Hourly";
    case compensation::salesman:
      return "Salesman";
    case compensation::COUNT:
      return "COUNT";
    }
  return "";
}
