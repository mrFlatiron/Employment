#include "employee_list_model.h"
#include "kernel/employee/employee_list.h"
#include "common/utils.h"
#include "kernel/employee/employee_utils.h"
#include "common/enum_misc.h"
#include <QFont>

employee_list_model::employee_list_model (employee_list *list, QObject *parent)
  : QAbstractTableModel (parent),
    m_emp_list (list)
{
  connect (m_emp_list, SIGNAL (size_changed ()), this, SLOT (refresh_indices ()));
  refresh_indices ();
}

employee_list_model::~employee_list_model ()
{

}

int employee_list_model::rowCount (const QModelIndex &parent) const
{
  (void)parent;

  return isize (m_row_to_index);
}

int employee_list_model::columnCount (const QModelIndex &parent) const
{
  (void)parent;
  return enum_size (employee::field ());
}

QVariant employee_list_model::data (const QModelIndex &index, int role) const
{
  if (!index.isValid ())
    return QVariant ();

  if (role == Qt::TextAlignmentRole )
      return Qt::AlignCenter;

  job_id id = m_row_to_index[index.row ()];

  employee_base *emp = m_emp_list->get (id);

  if (!emp)
    {
      DEBUG_PAUSE ("Shouldn't happen");
      return QVariant ();
    }

  int col = index.column ();
  employee::field field = enum_cast<employee::field> (col);

  compensation type = emp->compensation_type ();

  if (role == Qt::FontRole)
    {
      QFont font;
      font.setPointSizeF (13);
      return font;
    }

  switch (field)
    {
    case employee::field::compensation:
      if (role == Qt::DisplayRole)
        return enum_to_string (emp->compensation_type ());
      break;
    case employee::field::first_name:
      if (role == Qt::DisplayRole || role == Qt::EditRole)
        return emp->first_name ();
      break;
    case employee::field::last_name:
      if (role == Qt::DisplayRole || role == Qt::EditRole)
        return emp->last_name ();
      break;
    case employee::field::ssn:
      if (role == Qt::DisplayRole || role == Qt::EditRole)
        return emp->ssn ().pretty ();
      break;
    case employee::field::salary:
      if (role == Qt::DisplayRole || role == Qt::EditRole)
        switch (type)
          {
          case compensation::hourly:
            return static_cast<employee_hourly *> (emp)->wage ();
          case compensation::monthly:
            return static_cast<employee_monthly *> (emp)->salary ();
          case compensation::salesman:
            return static_cast<employee_salesman *> (emp)->salary ();
          case compensation::COUNT:
            DEBUG_PAUSE ("Shouldn't happen");
            return QVariant ();
          }
      break;
    case employee::field::hours_done:
      if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
          if (type != compensation::hourly)
            return QVariant ();
          return static_cast<employee_hourly *> (emp)->hours_done ();
        }
      break;
    case employee::field::has_bonus:
      if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
          if (type != compensation::salesman)
            return QVariant ();
          return static_cast<employee_salesman *> (emp)->has_bonus ();
        }
      break;
    case employee::field::bonus_percent:
      if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
          if (type != compensation::salesman)
            return QVariant ();
          return static_cast<employee_salesman *> (emp)->bonus_coef () * 100;
        }
      break;
    case employee::field::realized_outcome:
      if (role == Qt::DisplayRole || role == Qt::EditRole)
       {
          if (type != compensation::salesman)
            return QVariant ();
          return static_cast<employee_salesman *> (emp)->realized_outcome ();
        }
      break;
    case employee::field::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return QVariant ();
    }
  return QVariant ();
}

//type
//fname
//lname
//ssn
//salary/wage
//hours done
//bonus condition
//bonus percent
//salesman outcome
QVariant employee_list_model::headerData (int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Vertical)
    return QVariant ();
  if (role != Qt::DisplayRole)
    return QVariant ();

  employee::field field = enum_cast<employee::field> (section);

  switch (field)
    {
    case employee::field::compensation:
      return "Compensation";
    case employee::field::first_name:
      return "First name";
    case employee::field::last_name:
      return "Last name";
    case employee::field::ssn:
      return "SSN";
    case employee::field::salary:
      return "Salary/Wage";
    case employee::field::hours_done:
      return "Hours done";
    case employee::field::has_bonus:
      return "Has bonus";
    case employee::field::bonus_percent:
      return "Bonus percent";
    case employee::field::realized_outcome:
      return "Realized outcome";
    case employee::field::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return QVariant ();
    }
  return QVariant ();
}

bool employee_list_model::setData (const QModelIndex &index, const QVariant &value, int role)
{
  if (role != Qt::EditRole)
    return false;

  job_id id = m_row_to_index[index.row ()];
  int col = index.column ();
  employee::field field = static_cast<employee::field> (col);
  employee_base *emp_base = m_emp_list->get (id);
  compensation type = emp_base->compensation_type ();

  switch (field)
    {
    case employee::field::compensation:
      DEBUG_PAUSE ("Shouldn't happen");
      return false;
    case employee::field::first_name:
      if (!employee::is_field_valid (field, value.toString ()))
        {
          m_last_error = employee::error_string (field);
          error_occured ();
          return false;
        }
      else
        {
          emp_base->set_first_name (value.toString ());
          dataChanged (index, index);
          return true;
        }
    case employee::field::last_name:
      if (!employee::is_field_valid (field, value.toString ()))
        {
          m_last_error = employee::error_string (field);
          error_occured ();
          return false;
        }
      else
        {
          emp_base->set_last_name (value.toString ());
          dataChanged (index, index);
          return true;
        }
    case employee::field::ssn:
      {
        ssn_handle ssn (value.toString ());
        if (!ssn.is_valid ())
          {
            m_last_error = employee::error_string (field);
            error_occured ();
            return false;
          }
        else
          {
            if (!m_emp_list->is_ssn_unique (ssn))
              {
                m_last_error = QString ("This Social Security Number is already in the database");
                error_occured ();
                return false;
              }
            emp_base->set_ssn (ssn);
            dataChanged (index, index);
            return true;
          }
      }
    case employee::field::salary:
      {
        if (!employee::is_field_valid (field, value.toString ()))
          {
            m_last_error = employee::error_string (field);
            error_occured ();
            return false;
          }
        switch (type)
          {
          case compensation::monthly:
          case compensation::salesman:
            static_cast<employee_monthly *> (emp_base)->set_salary (value.toInt ());
            break;
          case compensation::hourly:
            static_cast<employee_hourly *> (emp_base)->set_wage (value.toInt ());
            break;
          case compensation::COUNT:
            DEBUG_PAUSE ("Shouldn't happen");
            break;
          }
        dataChanged (index, index);
        return true;
      }
    case employee::field::hours_done:
      if (!employee::is_field_valid (field, value.toString ()))
        {
          m_last_error = employee::error_string (field);
          error_occured ();
          return false;
        }
      static_cast<employee_hourly *> (emp_base)->set_hours_done (value.toInt ());
      dataChanged (index, index);
      return true;
    case employee::field::has_bonus:
      if (!employee::is_field_valid (field, value.toString ()))
        {
          m_last_error = employee::error_string (field);
          error_occured ();
          return false;
        }
      static_cast<employee_salesman *> (emp_base)->set_bonus_state (value.toBool ());
      dataChanged (index, index);
      return true;
    case employee::field::bonus_percent:
      if (!employee::is_field_valid (field, value.toString ()))
        {
          m_last_error = employee::error_string (field);
          error_occured ();
          return false;
        }
      static_cast<employee_salesman *> (emp_base)->set_bonus_coef (value.toDouble () / 100);
      dataChanged (index, index);
      return true;
    case employee::field::realized_outcome:
      if (!employee::is_field_valid (field, value.toString ()))
        {
          m_last_error = employee::error_string (field);
          error_occured ();;
          return false;
        }
      static_cast<employee_salesman *> (emp_base)->set_realized_outcome (value.toInt ());
      dataChanged (index, index);
      return true;
    case employee::field::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return false;
    }
  return false;
}

Qt::ItemFlags employee_list_model::flags (const QModelIndex &index) const
{
  Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  employee_base *emp = m_emp_list->get (m_row_to_index[index.row ()]);
  if (!emp)
    {
//      DEBUG_PAUSE ("Shouldn't happen");
      return flags;
    }

  compensation type = emp->compensation_type ();

  employee::field field = enum_cast<employee::field> (index.column ());

  switch (field)
    {
    case employee::field::compensation:
      break;
    case employee::field::first_name:
    case employee::field::last_name:
    case employee::field::ssn:
    case employee::field::salary:
      flags |= Qt::ItemIsEditable;
      break;
    case employee::field::hours_done:
      if (type == compensation::hourly)
        flags |= Qt::ItemIsEditable;
      break;
    case employee::field::has_bonus:
    case employee::field::bonus_percent:
    case employee::field::realized_outcome:
      if (type == compensation::salesman)
        flags |= Qt::ItemIsEditable;
      break;
    case employee::field::COUNT:
      DEBUG_PAUSE ("Shouldn't happen");
      return Qt::NoItemFlags;
    }
  return flags;
}

employee_list *employee_list_model::emp_list ()
{
  return m_emp_list;
}

bool employee_list_model::remove_by_row (const int row)
{
  job_id id = m_row_to_index[row];
  return m_emp_list->remove_by_id (id);
}

QString employee_list_model::last_error () const
{
  return m_last_error;
}

std::vector<job_id> employee_list_model::get_indices ()
{
  return m_row_to_index = m_emp_list->all_ids ();
}

void employee_list_model::refresh_indices ()
{
  m_row_to_index = get_indices ();

  auto &indices = m_row_to_index;

  std::map<job_id, int> index_to_row;

  int to_adjust = 0;

  int size = isize (indices);

  for (int i = 0; i < size; i++)
    index_to_row.insert (std::make_pair (indices[i], i));

  std::map<job_id, int>::iterator old_it = m_index_to_row.begin ();
  std::map<job_id, int>::iterator new_it = index_to_row.begin ();

  if (m_index_to_row.size () == 0
      && index_to_row.size () == 0)
    return;

  if (m_index_to_row.size () == 0)
    {
      beginInsertRows (QModelIndex (), index_to_row.begin ()->second, u_last (index_to_row)->second);
      m_index_to_row = index_to_row;
      endInsertRows ();
      return;
    }

  if (index_to_row.size () == 0)
    {
      beginRemoveRows (QModelIndex (), m_index_to_row.begin ()->second, u_last (m_index_to_row)->second);
      m_index_to_row = index_to_row;
      endRemoveRows ();
      return;
    }

  while (old_it != m_index_to_row.end () && new_it != index_to_row.end ())
    {
      if (old_it->first == new_it->first)
        {
          old_it->second = new_it->second;
          old_it++;
          new_it++;
          continue;
        }

      if (old_it->first < new_it->first)
        {
          auto first_old_it = old_it;
          auto last_old_it = old_it;

          while (old_it->first < new_it->first && old_it != m_index_to_row.end ())
            old_it++;

          last_old_it = old_it;
          last_old_it--;
          beginRemoveRows (QModelIndex (), first_old_it->second + to_adjust,
                           last_old_it->second + to_adjust);
          to_adjust -= last_old_it->second - first_old_it->second + 1;
          last_old_it++;
          m_index_to_row.erase (first_old_it, last_old_it);
          endRemoveRows ();
          continue;
        }

      if (old_it->first > new_it->first)
        {
          auto first_new_it = new_it;
          auto last_new_it = new_it;

          while (new_it->first < old_it->first && new_it != index_to_row.end ())
            new_it++;

          last_new_it = new_it;
          last_new_it--;
          beginInsertRows (QModelIndex (), first_new_it->second + to_adjust,
                           last_new_it->second + to_adjust);
          to_adjust += last_new_it->second - first_new_it->second + 1;
          last_new_it++;
          m_index_to_row.insert (first_new_it, last_new_it);
          endInsertRows ();
          continue;
        }
    }
  if (old_it != m_index_to_row.end ())
    {
      auto first_old_it = old_it;
      auto last_old_it = m_index_to_row.end ();

      beginRemoveRows (QModelIndex (), first_old_it->second + to_adjust,
                       u_last (m_index_to_row)->second + to_adjust);
      m_index_to_row.erase (first_old_it, last_old_it);
      endRemoveRows ();
      return;
    }
  if (new_it != index_to_row.end ())
    {
      auto first_new_it = new_it;
      auto last_new_it = index_to_row.end ();

      beginInsertRows (QModelIndex (), first_new_it->second + to_adjust,
                       u_last (index_to_row)->second + to_adjust);
      m_index_to_row.insert (first_new_it, last_new_it);
      endInsertRows ();
      return;
    }
}
