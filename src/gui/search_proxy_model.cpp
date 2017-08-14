#include "search_proxy_model.h"
#include "kernel/debug_utils.h"
#include <QColor>

search_proxy_model::search_proxy_model ()
{

}

search_proxy_model::~search_proxy_model ()
{

}

search_proxy_model::search_proxy_model (QAbstractItemModel *model)
{
  set_model (model);
}

void search_proxy_model::set_model (QAbstractItemModel *model)
{
  QSortFilterProxyModel::setSourceModel (model);
  m_search_engine.set_model (model);
}

void search_proxy_model::append_to_search_str (const QString &str)
{
  auto check = m_search_engine.accepted_rows_set ();
  m_search_engine.append_to_search_str (str);
  if (check != m_search_engine.accepted_rows_set ())
    invalidate ();
}

void search_proxy_model::set_search_str (const QString &str)
{
  auto check = m_search_engine.accepted_rows_set ();
  m_search_engine.set_search_str (str);
  if (check != m_search_engine.accepted_rows_set ())
    invalidate ();
}

void search_proxy_model::clear_search_str ()
{
  auto check = m_search_engine.accepted_rows_set ();
  m_search_engine.clear_search_str ();
  if (check != m_search_engine.accepted_rows_set ())
    invalidate ();
}

QVariant search_proxy_model::data (const QModelIndex &index, int role) const
{
  QModelIndex source_index = mapToSource (index);

  if (!source_index.isValid ())
    {
      DEBUG_PAUSE ("Why is that?");
      return QVariant ();
    }

  if (!m_search_engine.is_affecting ())
    return QSortFilterProxyModel::data (index, role);

  auto accepted_rows = m_search_engine.accepted_rows_set ();
  bool is_accepted = accepted_rows.find (QPersistentModelIndex (sourceModel ()->index (source_index.row (), 0))) != accepted_rows.end ();

  if (role == Qt::ForegroundRole)
    {
      if (is_accepted)
        return QColor (Qt::blue);
      else
        return QColor (Qt::gray);
    }

  return QSortFilterProxyModel::data (index, role);
}

bool search_proxy_model::lessThan (const QModelIndex &source_left, const QModelIndex &source_right) const
{
  if (!m_search_engine.is_affecting ())
    return QSortFilterProxyModel::lessThan (source_left, source_right);

  auto accepted_rows = m_search_engine.accepted_rows_set ();

  bool is_accepted_left = accepted_rows.find (QPersistentModelIndex (sourceModel ()->index (source_left.row (), 0))) != accepted_rows.end ();
  bool is_accepted_right = accepted_rows.find (QPersistentModelIndex (sourceModel ()->index (source_right.row (), 0))) != accepted_rows.end ();

  if (is_accepted_left && is_accepted_right)
    return QSortFilterProxyModel::lessThan (source_left, source_right);

  if (is_accepted_left)
    {
      if (sortOrder () == Qt::AscendingOrder)
        return true;
      else
        return false;
    }

  if (is_accepted_right)
    {
      if (sortOrder () == Qt::AscendingOrder)
        return false;
      return true;
    }

  return QSortFilterProxyModel::lessThan (source_left, source_right);
}
