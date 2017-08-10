#ifndef EMPLOYEE_LIST_GENERIC_H
#define EMPLOYEE_LIST_GENERIC_H

#include <unordered_map>
#include <type_traits>

#include "kernel/debug_utils.h"
#include "employee_list_abstract.h"
#include "containers/enum_bitset.h"

template<typename T, typename = typename std::enable_if<std::is_base_of<employee_base, T>::value, T>::type>
class employee_list_generic : public employee_list_abstract
{
private:
  int m_size;
  std::unordered_map<job_id, T> m_emps;
  transaction_info m_transaction;
public:
  employee_list_generic ()
  {
    m_size = 0;
  }

  ~employee_list_generic ()
  {

  }

  employee_base *get (const job_id i) override
  {
    auto it = m_emps.find (i);
    if (it != m_emps.end ())
      return &(it->second);
    return nullptr;
  }

  const employee_base *get (const job_id i) const override
  {
    auto it = m_emps.find (i);
    if (it != m_emps.end ())
      return &(it->second);
    return nullptr;
  }

  T *get_spec (const job_id i)
  {
    auto it = m_emps.find (i);
    if (it != m_emps.end ())
      return &(it->second);
    return nullptr;
  }

  const T *get_spec (const job_id i) const
  {
    auto it = m_emps.find (i);
    if (it != m_emps.end ())
      return &(it->second);
    return nullptr;
  }

  void try_size_changed ()
  {
    if (m_transaction.in_progress)
      m_transaction.size_changed = true;
    else
      size_changed ();
  }

  void try_data_changed (const job_id id,  const enum_bitset<employee::field> &fields)
  {
    if (m_transaction.in_progress)
      {
        if (m_transaction.min > id || m_transaction.min == -1)
          m_transaction.min = id;
        if (m_transaction.max < id || m_transaction.max == -1)
          m_transaction.max = id;
        m_transaction.or_fields |= fields;
      }
    else
      data_changed (id, id, fields);
  }

  bool remove_by_id (const job_id id) override
  {
    auto it = m_emps.find (id);
    if (it != m_emps.end ())
      {
        m_emps.erase (it);
        m_size--;
        try_size_changed ();
        return true;
      }
    try_size_changed ();
    return false;
  }

//  //return number of failed removals
//  int remove_by_id_batch (const std::vector<job_id> &ids) override
//  {
//    int count = 0;
//    for (job_id id : ids)
//      {
//        auto it = m_emps.find (id);
//        if (it != m_emps.end ())
//          {
//            m_emps.erase (it);
//            m_size--;
//          }
//        else
//          count++;
//      }
//    try_size_changed ();
//    return count;
//  }

  int size () const override
  {
    return m_size;
  }

  void add_employee (T &&emp, const job_id id)
  {
    emp.set_id (id);

    m_emps.insert (std::make_pair (id, std::move (emp)));

    connect (&(m_emps.at (id)),
             SIGNAL (data_changed (job_id,
                                   const enum_bitset<employee::field> &)),
             this,
             SLOT (on_employee_changed (job_id, const enum_bitset<employee::field> &)));

    m_size++;

    try_size_changed ();
  }

  void begin_transaction () override
  {
    if (m_transaction.in_progress)
      {
        DEBUG_PAUSE ("Already in transaction");
        return;
      }
    m_transaction.begin ();
  }

  void end_transaction () override
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



public slots:
  void on_employee_changed (job_id id, const enum_bitset<employee::field> &field) override
  {
    if (id >= 0)
      try_data_changed (id, field);
  }
};

#endif // EMPLOYEE_LIST_GENERIC_H
