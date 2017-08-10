#ifndef EMPLOYEE_LIST_ABSTRACT_H
#define EMPLOYEE_LIST_ABSTRACT_H

#include <QObject>

#include "employee_base.h"

class employee_list_abstract : public QObject
{
  Q_OBJECT
protected:
  struct transaction_info
  {
    bool size_changed = false;
    bool in_progress = false;
    job_id min = -1;
    job_id max = -1;
    enum_bitset<employee::field> or_fields;
    void end ()
    {
      size_changed = false;
      in_progress = false;
      min = -1;
      max = -1;
    }
    void begin ()
    {
      size_changed = false;
      in_progress = true;
      min = -1;
      max = -1;
    }
  };

public:
  virtual employee_base       *get (const job_id i)                                = 0;
  virtual const employee_base *get (const job_id i) const                          = 0;
  virtual bool                 remove_by_id (const job_id id)                      = 0;
  virtual int                  size () const                                       = 0;
  virtual void                 begin_transaction ()                                  {}
  virtual void                 end_transaction ()                                    {}
signals:
  void data_changed (job_id min, job_id max, const enum_bitset<employee::field> &or_fields);
  void size_changed ();
public slots:
  virtual void on_employee_changed (job_id, const enum_bitset<employee::field> &fields) = 0;
};

#endif // EMPLOYEE_LIST_ABSTRACT_H
