#ifndef TABLE_DYNAMIC_SEARCH_H
#define TABLE_DYNAMIC_SEARCH_H

#include <map>

#include <QString>
#include <QStringList>
#include <QObject>
#include <QSet>

class QAbstractItemModel;
class QPersistentModelIndex;

class table_dynamic_search
{
private:
  const QAbstractItemModel *m_model;
  QSet<QPersistentModelIndex> m_accepted_rows;
  QString m_search_str;
  QStringList m_parsed_str;
public:
  table_dynamic_search ();
  ~table_dynamic_search ();

  table_dynamic_search (const QAbstractItemModel *model);

  void set_model (const QAbstractItemModel *model);

  void append_to_search_str (const QString &str); //optimized

  void set_search_str (const QString &str);
  void clear_search_str ();

  bool is_affecting () const;

  std::vector<int> accepted_rows_vec () const;
  QSet<QPersistentModelIndex> accepted_rows_set () const;
private:
  QString get_row_str (const int row) const;
  bool accept_str (const QString &row_data, const QStringList &search_words);
  void append_with_last_replace (const QStringList &new_search_words, const bool is_replace_needed);
  void apply_new_words_to_accepted (const QStringList &new_search_words);
  std::pair<QStringList, bool> get_new_search_words (const QString &str) const;
};

#endif // TABLE_DYNAMIC_SEARCH_H
