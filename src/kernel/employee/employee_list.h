#ifndef EMPLOYIES_LIST_H
#define EMPLOYIES_LIST_H

#include <vector>
#include <unordered_map>
#include <map>

#include "kernel/debug_utils.h"
#include "employee_list_generic.h"
#include "containers/enum_vector.h"

#include "employee_hourly.h"
#include "employee_monthly.h"
#include "employee_salesman.h"

class employee_list_abstract;

class employee_list : public employee_list_abstract
{
  Q_OBJECT

private:
  transaction_info m_transaction;

  job_id m_greatest_id;

  employee_list_generic <employee_hourly> m_hourly_emp;
  employee_list_generic <employee_monthly> m_monthly_emp;
  employee_list_generic <employee_salesman> m_salesman_emp;

  enum_vector<compensation, employee_list_abstract *> m_all_lists;
  std::map<ssn_handle, job_id> m_ssn_to_id;
  std::unordered_map<job_id, ssn_handle> m_id_to_ssn; //for changing ssn purposes
public:
  employee_list ();
  ~employee_list ();

  employee_base       *get (const job_id i);
  const employee_base *get (const job_id i) const;

  job_id add_employee (employee_hourly &&emp);
  job_id add_employee (employee_monthly &&emp);
  job_id add_employee (employee_salesman &&emp);

  bool remove_by_id (const job_id id);

  employee_list_generic <employee_hourly>        &hourly_employies ();
  const employee_list_generic <employee_hourly>  &hourly_employies () const;

  employee_list_generic<employee_monthly>        &monthly_employies ();
  const employee_list_generic<employee_monthly>  &monthly_employies () const;

  employee_list_generic<employee_salesman>       &salesman_employies ();
  const employee_list_generic<employee_salesman> &salesman_employies () const;

  std::vector<job_id> all_ids () const;
  const std::map<ssn_handle, job_id> &ssn_to_id () const;

  int size () const;

  bool is_ssn_unique (const ssn_handle &ssn) const;

  void begin_transaction () override;
  void end_transaction () override;

public slots:
  void on_employee_changed (job_id id, const enum_bitset<employee::field> &or_fields) override;
  void on_sublist_data_changed (job_id min, job_id max, const enum_bitset<employee::field> &or_fields);
  void on_sublist_size_changed ();
private:
  template<typename T>
  job_id add_employee (T&& emp, employee_list_generic<T> &con);

  void try_size_changed ();
  void try_data_changed (const job_id min, const job_id max, const enum_bitset<employee::field> &or_fields);

};

#endif // EMPLOYIES_LIST_H
