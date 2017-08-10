#ifndef EMPLOYEE_SALESMAN_H
#define EMPLOYEE_SALESMAN_H

#include "employee_monthly.h"

class employee_salesman : public employee_monthly
{
protected:
  bool m_bonus_condition;
  double m_bonus_coef; // <= 1
  int m_realized_outcome;
public:
  employee_salesman ();
  virtual ~employee_salesman ();

  virtual compensation compensation_type () const override;

  bool has_bonus () const;
  double bonus_coef () const;
  int realized_outcome () const;

  void set_bonus_state (const bool state);
  void set_bonus_coef (const double coef);
  void set_realized_outcome (const int outcome);

  void inc_realized_outcome (const int inc);
  void set_realized_outcome_zero ();
};

#endif // EMPLOYEE_SALESMAN_H
