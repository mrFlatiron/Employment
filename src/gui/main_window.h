#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>
#include <memory>

#include "line_edit_w_hint.h"

class QLineEdit;
class QPushButton;
class QTableView;
class QLabel;
class QSortFilterProxyModel;

class add_emp_window;
class employee_list_model;
class employee_list;
class search_proxy_model;

class main_window : public QDialog
{
  Q_OBJECT

  std::unique_ptr<employee_list_model> m_table_model;

  QTableView *m_table;

  line_edit_w_hint *m_search_le;
  QPushButton *m_remove_pb;
  QPushButton *m_add_pb;
  QLabel *m_status_label;
  QSortFilterProxyModel *m_std_proxy_model;

  search_proxy_model *m_search_proxy_model;

  add_emp_window *m_add_emp_window;


public:
  main_window (employee_list *list, QWidget *parent = 0);
  ~main_window();

  QSize sizeHint () const;
private:
  void create_widgets ();
  void set_layout ();
  void make_connections ();

  void set_status_text (const QString &txt);
private slots:
  void open_add_window ();
  void add_employee ();
  void set_status_to_selection ();
  void set_status_to_idle ();
  void remove_selected_employies ();
  void on_edit_error ();
  void on_search_changed (line_edit_w_hint::hint, const QString &hint_str);
};

#endif // MAIN_WINDOW_H
