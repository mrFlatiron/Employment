#include "table_dynamic_search.h"
#include "common/utils.h"
#include <QRegularExpression>
#include <QAbstractItemModel>

table_dynamic_search::table_dynamic_search ()
  : m_model (nullptr)
{

}

table_dynamic_search::~table_dynamic_search ()
{

}

void table_dynamic_search::append_to_search_str (const QString &str)
{
  if (str.length () == 0)
    return;

  if (m_parsed_str.empty ())
    {
      set_search_str (str);
      return;
    }

  std::pair<QStringList, bool /*need last replace*/> words_w_flag = get_new_search_words (str);
  apply_new_words_to_accepted (words_w_flag.first);

  append_with_last_replace (words_w_flag.first, words_w_flag.second);
  m_search_str.append (str);
}

table_dynamic_search::table_dynamic_search (const QAbstractItemModel *model)
{
  set_model (model);
}

void table_dynamic_search::set_model (const QAbstractItemModel *model)
{
  m_model = model;
}

void table_dynamic_search::set_search_str (const QString &str)
{
  m_search_str = str;
  m_parsed_str = str.split (QRegularExpression ("\\s+"), QString::SkipEmptyParts);
  m_accepted_rows.clear ();

  if (!m_model)
    return;

  int row_count = m_model->rowCount ();
  if (m_model->columnCount () == 0)
    return;

  QString row_data;
  for (int i = 0; i < row_count; i++)
    {
      row_data = get_row_str (i);
      bool accepted = accept_str (row_data, m_parsed_str);
      if (accepted)
        m_accepted_rows.insert (QPersistentModelIndex (m_model->index (i, 0)));
    }
}

void table_dynamic_search::clear_search_str ()
{
  m_parsed_str.clear ();
  m_search_str.clear ();
  m_accepted_rows.clear ();
}

bool table_dynamic_search::is_affecting () const
{
  if (m_parsed_str.empty ())
    return false;
  return true;
}

std::vector<int> table_dynamic_search::accepted_rows_vec () const
{
  std::vector<int> retval;
  for (auto index : m_accepted_rows)
    retval.push_back (index.row ());
  return retval;
}

QSet<QPersistentModelIndex> table_dynamic_search::accepted_rows_set () const
{
  return m_accepted_rows;
}

QString table_dynamic_search::get_row_str (const int row) const
{
  int col_count = m_model->columnCount ();
  QString res;
  for (int c = 0; c < col_count; c++)
    {
      QModelIndex index = m_model->index (row, c);
      if (m_model->flags (index) & Qt::ItemIsEnabled)
        {
          res.append (QString (" %1").arg (m_model->data (index).toString ()));
        }
    }
  return res;
}

bool table_dynamic_search::accept_str (const QString &row_data, const QStringList &search_words)
{
  for (QString str : search_words)
    {
      if (!row_data.contains (str, Qt::CaseInsensitive))
        return false;
    }
  return true;
}

void table_dynamic_search::append_with_last_replace (const QStringList &new_search_words, const bool is_replace_needed)
{
  if (is_replace_needed)
    {
      auto it = m_parsed_str.end ();
      it--;
      m_parsed_str.erase (it);
    }

  m_parsed_str.append (new_search_words);
}

void table_dynamic_search::apply_new_words_to_accepted (const QStringList &new_search_words)
{
  auto iter = m_accepted_rows.begin ();
  while (iter != m_accepted_rows.end ())
    {
      int row = iter->row ();
      bool accepted = accept_str (get_row_str (row), new_search_words);
      if (!accepted)
        iter = m_accepted_rows.erase (iter);
      else
        iter++;
    }
}

std::pair<QStringList, bool> table_dynamic_search::get_new_search_words (const QString &str) const
{
  bool need_last_parsed_str_word_change = false;
  QStringList new_search_words = str.split (QRegularExpression ("\\s+"), QString::SkipEmptyParts);
  if (!
      (
        m_parsed_str.empty ()
        || new_search_words.empty ()
        || m_search_str[m_search_str.length () - 1].isSpace ()
        || str[0].isSpace ()
        )
      )
    {
      QString last_old = m_parsed_str.last ();
      last_old.append (new_search_words[0]);
      new_search_words[0] = last_old;
      need_last_parsed_str_word_change = true;
    }
  return {new_search_words, need_last_parsed_str_word_change};
}
