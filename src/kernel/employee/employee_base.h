#ifndef EMPLOYEE_BASE_H
#define EMPLOYEE_BASE_H

#include <QString>
#include <QObject>
#include "kernel/ssn_handle.h"
#include "kernel/employee/employee_utils.h"
#include "containers/enum_bitset.h"

using job_id = int;

enum class compensation
{
  monthly,
  hourly,
  salesman,
  COUNT
};

const char *enum_to_string (compensation c);



class employee_base : public QObject
{
  Q_OBJECT

protected:
  struct transaction_info
  {
    bool in_progress = false;
    enum_bitset<employee::field> changed;
    void begin () { in_progress = true; changed.reset ();  }
    void end ()   { in_progress = false; changed.reset (); }
  };

  transaction_info m_transaction;

  job_id m_job_id;
  QString m_first_name;
  QString m_last_name;
  ssn_handle m_ssn;
public:
  employee_base ();
  virtual ~employee_base ();

  employee_base (const employee_base &rhs);

  //  employee_base (const QString &full_name);
  //  employee_base (const QString &full_name, const ssn_handle &ssn);
  //  employee_base (const QString &first_name, const QString &last_name);
  employee_base (const QString &first_name, const QString &last_name, const ssn_handle &ssn);

  QString      full_name ()         const;
  QString      first_name ()        const;
  QString      last_name ()         const;
  ssn_handle   ssn ()               const;
  job_id       id ()                const;

  virtual void begin_transaction ();
  virtual void end_transaction ();
  virtual void try_data_changed (employee::field changed_field);

  void set_first_name (const QString &first_name);
  void set_last_name (const QString &last_name);
  void set_ssn (const ssn_handle &ssn);
  void set_id (const job_id id);

  virtual compensation compensation_type () const = 0;
signals:
  void data_changed (job_id, const enum_bitset<employee::field> &bits);

};

#endif // EMPLOYEE_BASE_H
