#ifndef EMPLOYEE_MONTHLY_H
#define EMPLOYEE_MONTHLY_H

#include "employee_base.h"


class employee_monthly : public employee_base
{
protected:
  int m_salary; //in cents
public:
  employee_monthly ();
  virtual ~employee_monthly ();

  virtual compensation compensation_type () const override;

  int salary () const;

  void set_salary (const int i);
};

#endif // EMPLOYEE_MONTHLY_H
