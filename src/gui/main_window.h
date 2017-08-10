#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>
#include <memory>

class QLineEdit;
class QPushButton;
class QTableView;
class QLabel;
class QSortFilterProxyModel;

class add_emp_window;

class employee_list_model;

class employee_list;

class main_window : public QDialog
{
  Q_OBJECT

  std::unique_ptr<employee_list_model> m_table_model;

  QTableView *m_table;

  QLineEdit *m_search_le;
  QPushButton *m_remove_pb;
  QPushButton *m_add_pb;
  QLabel *m_status_label;
  QSortFilterProxyModel *m_proxy_model;

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
};

#endif // MAIN_WINDOW_H
