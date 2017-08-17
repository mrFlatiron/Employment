#ifndef SQLITE_TABLE_CONFIGURER_H
#define SQLITE_TABLE_CONFIGURER_H

#include "common/enum_range.h"
#include "kernel/debug_utils.h"

#include <QString>
#include <QtSql/QtSql>
#include <QRegularExpression>

#include <cstdio>

enum class sqlite_simple_query
{
  drop,
  create,
  select,
  insert,
  sync_pragma_off,
  COUNT
};

template<typename Enum, typename = use_if_enum<Enum>>
const char *enum_to_sqlite_table_constr (Enum, Enum = Enum ()) {return "";}

template<typename Enum, typename = use_if_enum<Enum>>
const char *enum_to_sqlite_col_constr (Enum, Enum = Enum ()) {return "";}

template<typename Enum, typename = use_if_enum<Enum>>
class sqlite_table_configurer
{
private:
  QString m_table_name;
  QString m_db_name;
  QString m_connection_name;

  QSqlDatabase m_db;
public:
  sqlite_table_configurer () {}
  ~sqlite_table_configurer () {}

  sqlite_table_configurer (const QString &table_name, const QString &db_name, const QString &connection_name) :
    m_table_name (table_name)
  {
    set_connection_name (connection_name);
    set_db_name (db_name);
  }

  void set_table_name (const QString &table_name)           {m_table_name = table_name;}

  void set_db_name (const QString &db_name)
  {
    m_db_name = db_name;
    if (m_db.isValid ())
      m_db.setDatabaseName (m_db_name);
  }

  void set_connection_name (const QString &connection_name)
  {
    m_connection_name = connection_name;
    m_db = QSqlDatabase::addDatabase ("QSQLITE", m_connection_name);
    if (!QRegularExpression ("^\\s*$").match (m_db_name).hasMatch ())
      set_db_name (m_db_name);
  }

  bool connect ()
  {
    if (!m_db.isValid ())
      return false;
    if (m_db.isOpen ())
      return false;

    return m_db.open ();
  }

  QSqlDatabase &db_handle ()             {return m_db;}
  const QSqlDatabase &db_handle () const {return m_db;}

  QString simple_query (sqlite_simple_query query_type) const
  {
    switch (query_type)
      {
      case sqlite_simple_query::drop:
        return QString ("DROP TABLE IF EXISTS %1;").arg (m_table_name);
      case sqlite_simple_query::create:
        {
          QString templ = "CREATE TABLE %1 (%2 %3);";
          QString col_defs;
          for (Enum e : enum_range<Enum> ())
            {
              col_defs += QString ("%1 %2 %3, ").arg (enum_to_string (e),
                                                      enum_to_sqlite_type (e),
                                                      enum_to_sqlite_col_constr (e));
            }
          col_defs = col_defs.left (col_defs.lastIndexOf (','));
          QString table_constraint;
          QString check;
          if (QRegularExpression ("^\\s*$").match (enum_to_sqlite_table_constr (Enum ())).hasMatch ())
            {
              check = templ.arg (m_table_name, col_defs, "");
              fprintf (stderr, "CREATE DUMP: %s\n", check.toUtf8 ().data ());
              return check;
            }
          else
            {
              table_constraint += ',';
              table_constraint += enum_to_sqlite_table_constr (Enum ());
            }
          check = templ.arg (m_table_name, col_defs, table_constraint);
          fprintf (stderr, "CREATE DUMP: %s\n", check.toUtf8 ().data ());
          return check;
        }
      case sqlite_simple_query::select:
        {
          QString templ = "SELECT %1 FROM %2;";
          QString cols;
          for (Enum e : enum_range<Enum> ())
            {
              cols += QString ("%1, ").arg (enum_to_string (e));
            }
          cols = cols.left (cols.lastIndexOf (','));
          QString check = templ.arg (cols, m_table_name);
          fprintf (stderr, "SELECT DUMP: %s\n", check.toUtf8 ().data ());
        return check;
        }
      case sqlite_simple_query::sync_pragma_off:
        return "PRAGMA synchronous = OFF;";
      case sqlite_simple_query::insert:
        {
          QString templ = "INSERT INTO %1 (%2) VALUES (%3);";
          QString cols;
          QString qmarks;
          for (Enum e : enum_range<Enum> ())
            {
              cols += QString ("%1, ").arg (enum_to_string (e));
              qmarks += QString ("?, ");
            }
          cols = cols.left (cols.lastIndexOf (','));
          qmarks = qmarks.left (qmarks.lastIndexOf (','));
          QString check = templ.arg (m_table_name, cols, qmarks);
          fprintf (stderr, "INSERT DUMP: %s\n", check.toUtf8 ().data ());
          return check;
        }
      case sqlite_simple_query::COUNT:
        DEBUG_PAUSE ("Shouldn't happen");
        return "";
      }
  }
};

#endif // SQLITE_TABLE_CONFIGURER_H
