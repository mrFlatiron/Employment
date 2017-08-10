#ifndef ADD_EMP_WINDOW_H
#define ADD_EMP_WINDOW_H

#include <QDialog>
#include "containers/enum_vector.h"
#include "kernel/employee/employee_hourly.h"
#include "kernel/employee/employee_monthly.h"
#include "kernel/employee/employee_salesman.h"
#include "kernel/employee/employee_list.h"
#include "kernel/employee/employee_utils.h"

class QComboBox;
class QLineEdit;
class QLabel;
class QGridLayout;
class QVBoxLayout;
class QPushButton;


const char *enum_to_string (add_emp_field le);


class add_emp_window : public QDialog
{
  Q_OBJECT

private:
  QComboBox *m_type_cb;
  QLabel *m_type_label;
  enum_vector<add_emp_field, QLineEdit *> m_line_edits;
  enum_vector<add_emp_field, QLabel *> m_labels;
  QGridLayout *m_grid_layout;
  QVBoxLayout *m_vert_layout;
  QLabel *m_error_label;
  int m_row_count;
  QPushButton *m_ok_pb;
  QPushButton *m_clear_pb;

  const employee_list *m_emp_list;
public:
  add_emp_window (const employee_list *list, QWidget *parent = nullptr);
  ~add_emp_window ();

  QSize sizeHint () const;

  compensation employee_type () const;
  employee_monthly get_monthly_employee () const;
  employee_hourly get_hourly_employee () const;
  employee_salesman get_salesman_employee () const;

signals:
  void add_completed ();
private:
  void create_widgets ();
  void set_layout ();
  void make_connections ();

  void remove_salesman_fields ();
  void add_salesman_fields ();

 void set_base_fields (employee_base &emp) const;

  std::pair<bool, QString> validate_field (add_emp_field field) const;
  void set_error_msg (const QString &msg);
  void clear_error ();
private slots:
  void clear_all ();
  void validate ();
  void refresh_layout ();
};

#endif // ADD_EMP_WINDOW_H
