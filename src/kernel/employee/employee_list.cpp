#include "employee_list.h"
#include <cassert>
#include <algorithm>


employee_list::employee_list () :
  m_greatest_id (0)
{
  m_all_lists[compensation::monthly] = &m_monthly_emp;
  m_all_lists[compensation::hourly] = &m_hourly_emp;
  m_all_lists[compensation::salesman] = &m_salesman_emp;

  for (auto type : enum_range<compensation> ())
    {
      connect (m_all_lists[type], SIGNAL (data_changed (job_id, job_id, const enum_bitset<employee::field> &)),
               this, SLOT (on_sublist_data_changed (job_id, job_id, const enum_bitset<employee::field> &)));
      connect (m_all_lists[type], SIGNAL (size_changed ()), this, SLOT (on_sublist_size_changed ()));
    }
}

employee_list::~employee_list ()
{

}

employee_base *employee_list::get (const job_id i)
{
  for (auto type : enum_range<compensation> ())
    {
      employee_base *retval = m_all_lists[type]->get (i);
      if (retval)
        return retval;
    }
  return nullptr;
}

const employee_base *employee_list::get (const job_id i) const
{
  for (auto type : enum_range<compensation> ())
    {
      employee_base *retval = m_all_lists[type]->get (i);
      if (retval)
        return retval;
    }
  return nullptr;
}

job_id employee_list::add_employee (employee_hourly &&emp)
{
  return add_employee (std::move (emp), m_hourly_emp);
}

job_id employee_list::add_employee (employee_monthly &&emp)
{
  return add_employee (std::move (emp), m_monthly_emp);
}

job_id employee_list::add_employee (employee_salesman &&emp)
{
  return add_employee (std::move (emp), m_salesman_emp);
}

bool employee_list::remove_by_id (const job_id id)
{
  for (auto type : enum_range<compensation> ())
    {
      employee_base *to_remove = m_all_lists[type]->get (id);
      if (!to_remove)
        continue;
      m_ssn_to_id.erase (to_remove->ssn ());
      m_id_to_ssn.erase (id);
      m_all_lists[type]->remove_by_id (id);
      return true;
    }
  return false;
}

employee_list_generic<employee_hourly> &employee_list::hourly_employies ()
{
  return m_hourly_emp;
}

const employee_list_generic<employee_hourly> &employee_list::hourly_employies () const
{
  return m_hourly_emp;
}

employee_list_generic<employee_monthly> &employee_list::monthly_employies ()
{
  return m_monthly_emp;
}

const employee_list_generic<employee_monthly> &employee_list::monthly_employies () const
{
  return m_monthly_emp;
}

employee_list_generic<employee_salesman> &employee_list::salesman_employies ()
{
  return m_salesman_emp;
}

const employee_list_generic<employee_salesman> &employee_list::salesman_employies () const
{
  return m_salesman_emp;
}

std::vector<job_id> employee_list::all_ids () const
{
  std::vector<job_id> retval;

  for (auto &p : m_ssn_to_id)
    retval.push_back (p.second);

  std::sort (retval.begin (), retval.end ());

  return retval;
}

const std::map<ssn_handle, job_id> &employee_list::ssn_to_id () const
{
  return m_ssn_to_id;
}

int employee_list::size () const
{
  int sum = 0;
  for (auto type : enum_range<compensation> ())
    sum += m_all_lists[type]->size ();

  return sum;
}

bool employee_list::is_ssn_unique (const ssn_handle &ssn) const
{
  return m_ssn_to_id.find (ssn) == m_ssn_to_id.end () && ssn.is_valid ();
}

void employee_list::begin_transaction ()
{
  if (m_transaction.in_progress)
    {
      DEBUG_PAUSE ("Already in progress");
      return;
    }
  m_transaction.begin ();
}

void employee_list::end_transaction ()
{
  if (!m_transaction.in_progress)
    {
      DEBUG_PAUSE ("No transaction in progress");
      return;
    }
  if (m_transaction.size_changed)
    size_changed ();
  else if (m_transaction.max != -1)
    data_changed (m_transaction.min, m_transaction.max, m_transaction.or_fields);
  m_transaction.end ();
}

void employee_list::on_employee_changed (job_id id, const enum_bitset<employee::field> &or_fields)
{
  //per employee action is done on employee_list_generic level
  (void)or_fields;
  (void)id;
}

void employee_list::on_sublist_data_changed (job_id min, job_id max, const enum_bitset<employee::field> &or_fields)
{
  if (or_fields[employee::field::ssn])
    {
      for (job_id id = min; id <= max; id++)
        {
          employee_base *base = get (id);
          if (!base)
            {
              DEBUG_PAUSE ("How did this happened?");
              continue;
            }
          ssn_handle ssn = base->ssn ();
          if (m_ssn_to_id.find (ssn) != m_ssn_to_id.end ())
            continue;

          ssn_handle ssn_old = m_id_to_ssn.at (id);
          m_ssn_to_id.erase (ssn_old);
          m_id_to_ssn.at (id) = ssn;
          m_ssn_to_id.insert (std::make_pair (ssn, id));
        }
    }

  try_data_changed (min, max, or_fields);
}

void employee_list::on_sublist_size_changed ()
{
  try_size_changed ();
}

void employee_list::try_size_changed ()
{
  if (m_transaction.in_progress)
    {
      m_transaction.size_changed = true;
      return;
    }
  else
    size_changed ();
}

void employee_list::try_data_changed (const job_id min, const job_id max, const enum_bitset<employee::field> &or_fields)
{
  if (m_transaction.in_progress)
    {
      if (m_transaction.max < max || m_transaction.max == -1)
        m_transaction.max = max;
      if (m_transaction.min > min || m_transaction.min == -1)
        m_transaction.min = min;
      m_transaction.or_fields |= or_fields;
      return;
    }
  else
    data_changed (min, max, or_fields);
}

template<typename T>
job_id employee_list::add_employee (T&& emp, employee_list_generic<T> &con)
{
  if (!is_ssn_unique (emp.ssn ()))
    {
      DEBUG_PAUSE ("Non unique or non valid ssn");
      return -1;
    }

  m_greatest_id++;

  m_ssn_to_id.insert (std::make_pair (emp.ssn (), m_greatest_id));
  m_id_to_ssn.insert (std::make_pair (m_greatest_id, emp.ssn ()));

  con.add_employee (std::move (emp), m_greatest_id);

  return m_greatest_id;
}
