#ifndef EMPLOYEE_HOURLY_H
#define EMPLOYEE_HOURLY_H

#include "employee_base.h"

class employee_hourly : public employee_base
{
protected:
  int m_wage; //in cents
  int m_hours_done;
public:
  employee_hourly ();
  virtual ~employee_hourly ();

  virtual compensation compensation_type () const override;

  int wage () const;
  int hours_done () const;

  void set_wage (const int wage);
  void set_hours_done (const int done);
  void inc_hours_done (const int inc = 1);
};

#endif // EMPLOYEE_HOURLY_H
