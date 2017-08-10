#include "add_emp_window.h"

#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>

#include "common/enum_range.h"
#include "kernel/debug_utils.h"
#include "kernel/employee/employee_base.h"

const char *enum_to_string (add_emp_field le)
{
  switch (le)
    {
    case add_emp_field::first_name:
      return "First name";
    case add_emp_field::last_name:
      return "Last name";
    case add_emp_field::ssn:
      return "Social Security Number";
    case add_emp_field::salary:
      return "Salary/Wage";
    case add_emp_field::bonus_percent:
      return "Outcome bonus percent";
    case add_emp_field::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return "";
    }
  return "";
}

add_emp_window::add_emp_window (const employee_list *list, QWidget *parent)
  : QDialog (parent),
    m_emp_list (list)
{
  create_widgets ();
  set_layout ();
  make_connections ();

  refresh_layout ();
}

add_emp_window::~add_emp_window ()
{

}

QSize add_emp_window::sizeHint () const
{
  return QSize (400, 200);
}

compensation add_emp_window::employee_type () const
{
  int cur_index = m_type_cb->currentIndex ();
  if (cur_index < 0)
    return compensation::COUNT;

  return static_cast<compensation> (cur_index);
}

employee_monthly add_emp_window::get_monthly_employee () const
{
  employee_monthly emp;
  if (employee_type () != compensation::monthly)
    return emp;

  set_base_fields (emp);
  emp.set_salary (m_line_edits[add_emp_field::salary]->text ().toInt ());

  return emp;
}

employee_hourly add_emp_window::get_hourly_employee () const
{
  employee_hourly emp;
  if (employee_type () != compensation::hourly)
    return emp;

  set_base_fields (emp);
  emp.set_wage (m_line_edits[add_emp_field::salary]->text ().toInt ());
  emp.set_hours_done (0);
  return emp;
}

employee_salesman add_emp_window::get_salesman_employee () const
{
  employee_salesman emp;
  if (employee_type () != compensation::salesman)
    return emp;

  set_base_fields (emp);
  emp.set_salary (m_line_edits[add_emp_field::salary]->text ().toInt ());
  emp.set_bonus_state (false);
  emp.set_realized_outcome (0);
  emp.set_bonus_coef (m_line_edits[add_emp_field::bonus_percent]->text ().toDouble () / 100);
  return emp;
}

void add_emp_window::create_widgets ()
{
  m_type_cb = new QComboBox (this);
  for (auto type : enum_range<compensation> ())
    m_type_cb->addItem (enum_to_string (type));
  m_type_cb->setCurrentIndex (-1);

  m_type_label = new QLabel ("Compensation type:", this);

  m_error_label = new QLabel ("", this);
  m_error_label->hide ();

  for (auto type : enum_range<add_emp_field> ())
    {
      m_line_edits[type] = new QLineEdit (this);
      m_labels[type] = new QLabel (enum_to_string (type), this);
    }

  m_clear_pb = new QPushButton ("Clear", this);
  m_ok_pb = new QPushButton ("Ok", this);

  m_grid_layout = new QGridLayout;
  m_vert_layout = new QVBoxLayout;

  setTabOrder (m_ok_pb, m_clear_pb);
}

void add_emp_window::set_layout ()
{
  //QVBoxLayout
  {
    //QGridLayout
    {
      m_grid_layout->addWidget (m_type_label, 0, 0);
      m_grid_layout->addWidget (m_type_cb, 0, 1);

      int row = 1;

      for (auto field : enum_range<add_emp_field> ())
        {
          if (field == add_emp_field::bonus_percent)
            {
              m_labels[field]->hide ();
              m_line_edits[field]->hide ();
              continue;
            }

          m_grid_layout->addWidget (m_labels[field], row, 0);
          m_grid_layout->addWidget (m_line_edits[field], row, 1);
          row++;
        }

      m_row_count = row;
    }
    m_vert_layout->addLayout (m_grid_layout);
    m_vert_layout->addStretch ();

    QHBoxLayout *hlo_0 = new QHBoxLayout;
    {
      hlo_0->addStretch ();
      hlo_0->addWidget (m_clear_pb);
      hlo_0->addWidget (m_ok_pb);
    }
    m_vert_layout->addLayout (hlo_0);
  }
  setLayout (m_vert_layout);
}

void add_emp_window::make_connections ()
{
  connect (m_type_cb, SIGNAL (currentIndexChanged (int)), this, SLOT (refresh_layout ()));
  connect (m_ok_pb, SIGNAL (clicked ()), this, SLOT (validate ()));
  connect (m_clear_pb, SIGNAL (clicked ()), this, SLOT (clear_all ()));
}

void add_emp_window::remove_salesman_fields ()
{
  m_labels[add_emp_field::bonus_percent]->hide ();
  m_line_edits[add_emp_field::bonus_percent]->hide ();

  if (m_row_count == 6)
    {
      m_grid_layout->removeItem (m_grid_layout->itemAtPosition (m_row_count - 1, 0));
      m_grid_layout->removeItem (m_grid_layout->itemAtPosition (m_row_count - 1, 1));
      m_row_count--;
    }
}

void add_emp_window::add_salesman_fields ()
{
  m_labels[add_emp_field::bonus_percent]->show ();
  m_line_edits[add_emp_field::bonus_percent]->show ();
  if (m_row_count == 5)
    {
      m_grid_layout->addWidget (m_labels[add_emp_field::bonus_percent], m_row_count, 0);
      m_grid_layout->addWidget (m_line_edits[add_emp_field::bonus_percent], m_row_count, 1);
      m_row_count++;
    }
}

void add_emp_window::set_base_fields (employee_base &emp) const
{
  emp.set_first_name (m_line_edits[add_emp_field::first_name]->text ());
  emp.set_last_name (m_line_edits[add_emp_field::last_name]->text ());
  emp.set_ssn (ssn_handle (m_line_edits[add_emp_field::ssn]->text ()));

}

std::pair<bool, QString> add_emp_window::validate_field (add_emp_field field) const
{
  QString str = m_line_edits[field]->text ();
  employee::field general_field = employee::enum_convert (field);

  if (field == add_emp_field::ssn)
    {
      ssn_handle ssn (str);
      if (!employee::is_field_valid (general_field, str))
        return {false, employee::error_string (general_field)};

      if (m_emp_list && !m_emp_list->is_ssn_unique (ssn))
        return {false, "This Social Security Number is already in the database"};
    }

  return {employee::is_field_valid (general_field, str),
          employee::error_string (general_field)};
}

void add_emp_window::set_error_msg (const QString &msg)
{
  m_error_label->setText (msg);
  m_error_label->show ();
  m_vert_layout->addWidget (m_error_label);
}

void add_emp_window::clear_error ()
{
  m_error_label->hide ();
  m_vert_layout->removeWidget (m_error_label);
}

void add_emp_window::clear_all ()
{
  for (auto field : enum_range<add_emp_field> ())
    m_line_edits[field]->clear ();
  clear_error ();
}

void add_emp_window::validate ()
{
  for (auto field : enum_range<add_emp_field> ())
    {
      if (field == add_emp_field::bonus_percent
          && employee_type () != compensation::salesman)
        continue;

      std::pair<bool, QString> res = validate_field (field);
      if (!res.first)
        {
          set_error_msg (res.second);
          return;
        }
    }
  clear_error ();
  add_completed ();
  clear_all ();
}

void add_emp_window::refresh_layout ()
{
  int cur_type_index = m_type_cb->currentIndex ();
  if (cur_type_index < 0)
    {
      remove_salesman_fields ();
      return;
    }

  compensation comp_type = static_cast<compensation> (cur_type_index);

  switch (comp_type)
    {
    case compensation::monthly:
      remove_salesman_fields ();
      m_labels[add_emp_field::salary]->setText ("Salary");
      return;
    case compensation::hourly:
      remove_salesman_fields ();
      m_labels[add_emp_field::salary]->setText ("Wage");
      return;
    case compensation::salesman:
      add_salesman_fields ();
      m_labels[add_emp_field::salary]->setText ("Salary");
      return;
    case compensation::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return;
    }
}
