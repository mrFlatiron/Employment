#ifndef SEARCH_PROXY_MODEL_H
#define SEARCH_PROXY_MODEL_H

#include <QSortFilterProxyModel>
#include "gui/table_dynamic_search.h"

class search_proxy_model : public QSortFilterProxyModel
{
private:
  table_dynamic_search m_search_engine;

public:
  search_proxy_model ();
  ~search_proxy_model ();

  search_proxy_model (QAbstractItemModel *model);
  void set_model (QAbstractItemModel *model);

  void append_to_search_str (const QString &str);

  void set_search_str (const QString &str);
  void clear_search_str ();

  QVariant data (const QModelIndex &index, int role) const override;
  bool lessThan (const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // SEARCH_PROXY_MODEL_H
