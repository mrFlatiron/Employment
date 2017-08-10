#ifndef EMPLOYEE_LIST_MODEL_H
#define EMPLOYEE_LIST_MODEL_H

#include <QAbstractTableModel>
#include <map>
#include <vector>

#include "kernel/employee/employee_base.h"

class employee_list;

class employee_list_model : public QAbstractTableModel
{
  Q_OBJECT
private:
  employee_list *m_emp_list;
  std::map<job_id, int> m_index_to_row;
  std::vector<job_id> m_row_to_index;
  QString m_last_error;
public:
  employee_list_model (employee_list *list, QObject *parent = nullptr);
  ~employee_list_model ();

  int           rowCount (const QModelIndex &parent) const                            override;
  int           columnCount (const QModelIndex &parent) const                         override;
  QVariant      data (const QModelIndex &index, int role) const                       override;
  QVariant      headerData (int section, Qt::Orientation orientation, int role) const override;
  bool          setData (const QModelIndex &index, const QVariant &value, int role)   override;
  Qt::ItemFlags flags (const QModelIndex &index) const                                override;

  employee_list *emp_list ();
  bool remove_by_row (const int row);

  QString last_error () const;
signals:
  void error_occured ();
private:
  std::vector<job_id> get_indices ();
private slots:
  void refresh_indices ();
};

#endif // EMPLOYEE_LIST_MODEL_H
