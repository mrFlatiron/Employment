#include "main_window.h"
#include "common/utils.h"
#include "employee_list_model.h"
#include "kernel/employee/employee_list.h"
#include "add_emp_window.h"

#include "search_proxy_model.h"

#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QIcon>
#include <QBitmap>

main_window::main_window (employee_list *list, QWidget *parent)
  : QDialog(parent)
{

  put_in (m_table_model, list);

  create_widgets ();
  set_layout ();
  make_connections ();

  m_table->resizeColumnsToContents ();
}

main_window::~main_window()
{

}

QSize main_window::sizeHint () const
{
 return QSize (1024, 780);
}

void main_window::create_widgets ()
{
  m_search_le = new line_edit_w_hint (this);
  m_add_pb = new QPushButton ("Add");
  m_remove_pb = new QPushButton ("Remove", this);
  m_status_label = new QLabel ("Idle", this);
  m_table = new QTableView (this);
  m_table->setSortingEnabled (true);
  m_add_emp_window = new add_emp_window (m_table_model->emp_list (), this);

  m_add_emp_window->hide ();

//  m_std_proxy_model = new QSortFilterProxyModel (this);
//  m_std_proxy_model->setSourceModel (m_table_model.get ());
  m_search_proxy_model = new search_proxy_model (m_table_model.get ());
  m_table->setModel (m_search_proxy_model);
  m_table->setSelectionBehavior (QAbstractItemView::SelectRows);
  m_table->setSelectionMode (QAbstractItemView::ExtendedSelection);
  m_table->setAlternatingRowColors (true);

  m_remove_pb->setDisabled (true);
}

void main_window::set_layout ()
{
  QVBoxLayout *vlo_0 = new QVBoxLayout;
  {
    QHBoxLayout *hlo_1 = new QHBoxLayout;
    {
      QLabel *label = new QLabel (this);
      QPixmap pixmap;
      pixmap.load (":/icons/search.png");
      pixmap = pixmap.scaled (24, 24);
      label->setPixmap (pixmap);
      label->setMask(pixmap.mask());
      hlo_1->addWidget (label);
      hlo_1->addWidget (m_search_le->as_qlineedit ());
    }
    vlo_0->addLayout (hlo_1);
    vlo_0->addWidget (m_table);
    QHBoxLayout *hlo_0 = new QHBoxLayout;
    {
      hlo_0->addWidget (m_status_label);
      hlo_0->addStretch ();
      hlo_0->addWidget (m_remove_pb);
      hlo_0->addWidget (m_add_pb);
    }
    vlo_0->addLayout (hlo_0);

  }
  setLayout (vlo_0);

  setWindowFlags(Qt::Window |
                 Qt::CustomizeWindowHint |
                 Qt::WindowMaximizeButtonHint |
                 Qt::WindowCloseButtonHint);
}

void main_window::make_connections ()
{
  connect (m_add_pb, SIGNAL (clicked ()), this, SLOT (open_add_window ()));
  connect (m_add_emp_window, SIGNAL (add_completed ()), this, SLOT (add_employee ()));
  connect (m_table->selectionModel (), SIGNAL (selectionChanged (const QItemSelection, const QItemSelection)), this, SLOT (set_status_to_selection ()));
  connect (m_remove_pb, SIGNAL (clicked ()), this, SLOT (remove_selected_employies ()));
  connect (m_table_model.get (), SIGNAL (error_occured ()), this, SLOT (on_edit_error ()));
  connect (m_search_le, SIGNAL (text_changed (line_edit_w_hint::hint, const QString &)), this,
           SLOT (on_search_changed (line_edit_w_hint::hint, const QString &)));
}

void main_window::set_status_text (const QString &txt)
{
  m_status_label->setText (txt);
}

void main_window::add_employee ()
{
  compensation type = m_add_emp_window->employee_type ();
  switch (type)
    {
    case compensation::monthly:
      m_table_model->emp_list ()->add_employee (m_add_emp_window->get_monthly_employee ());
      break;
    case compensation::hourly:
      m_table_model->emp_list ()->add_employee (m_add_emp_window->get_hourly_employee ());
      break;
    case compensation::salesman:
      m_table_model->emp_list ()->add_employee (m_add_emp_window->get_salesman_employee ());
      break;
    case compensation::COUNT:
      return;
    }
}

void main_window::set_status_to_selection ()
{
  QModelIndexList rows = m_table->selectionModel ()->selectedRows ();

  if (rows.size () == 0)
    {
      set_status_to_idle ();
      m_remove_pb->setDisabled (true);
      return;
    }

  m_remove_pb->setEnabled (true);
  set_status_text (QString ("Employies selected: %1").arg
                   (QString::number (rows.size ())));
}

void main_window::set_status_to_idle ()
{
  set_status_text ("Idle");
}

void main_window::remove_selected_employies ()
{
  int failed = 0;
  QModelIndexList rows = m_table->selectionModel ()->selectedRows ();
  m_table_model->emp_list ()->begin_transaction ();
  for (QModelIndex index : rows)
    {
      int row = /*m_std_proxy_model->mapToSource (*/m_search_proxy_model->mapToSource (index)/*)*/.row ();
      bool check = m_table_model->remove_by_row (row);
      if (!check)
        failed++;
    }
  m_table_model->emp_list ()->end_transaction ();
  if (failed)
    set_status_text (QString ("Failed to remove %1 employies").arg (QString::number (failed)));
}

void main_window::on_edit_error ()
{
  set_status_text (m_table_model->last_error ());
}

void main_window::on_search_changed (line_edit_w_hint::hint hint, const QString &hint_str)
{
  switch (hint)
    {
    case line_edit_w_hint::hint::append:
      m_search_proxy_model->append_to_search_str (hint_str);
      break;
    case line_edit_w_hint::hint::clear:
      m_search_proxy_model->clear_search_str ();
      break;
    case line_edit_w_hint::hint::other:
      m_search_proxy_model->set_search_str (hint_str);
      break;
    case line_edit_w_hint::hint::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return;
    }
  m_search_proxy_model->sort (0);
}

void main_window::open_add_window ()
{
  if (!m_add_emp_window)



  m_add_emp_window->raise ();
  m_add_emp_window->show ();
}
