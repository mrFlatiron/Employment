#include "ssn_handle.h"
#include <cstring>

#include <QRegExp>

ssn_handle::ssn_handle ()
{
  m_is_valid = false;
}

ssn_handle::~ssn_handle ()
{

}

ssn_handle::ssn_handle (const QString &ssn)
{
  set_from_string (ssn);
}

void ssn_handle::set_from_string (const QString &ssn)
{
  QString ssn_parsed = ssn;
  ssn_parsed.remove ('-');
  ssn_parsed.remove (' ');

  if (QRegExp ("^[0-9]{9}$").indexIn (ssn_parsed) != 0)
    {
      m_is_valid = false;
      return;
    }

  strcpy (m_first, ssn_parsed.left (3).toUtf8 ().data ());
  strcpy (m_second, ssn_parsed.left (5).right (2).toUtf8 ().data ());
  strcpy (m_third, ssn_parsed.right (4).toUtf8 ().data ());

  m_is_valid = true;
}

bool ssn_handle::is_valid () const
{
  return m_is_valid;
}

QString ssn_handle::pretty () const
{
  return QString ("%1-%2-%3").arg (m_first, m_second, m_third);
}

QString ssn_handle::simple () const
{
  return QString ("%1%2%3").arg (m_first, m_second, m_third);
}

bool ssn_handle::operator < (const ssn_handle &rhs) const
{
  if (strcmp (m_first, rhs.m_first) < 0)
    return true;

  if (strcmp (m_first, rhs.m_first) > 0)
    return false;

  if (strcmp (m_second, rhs.m_second) < 0)
    return true;

  if (strcmp (m_second, rhs.m_second) > 0)
    return false;

  if (strcmp (m_third, rhs.m_third) < 0)
    return true;

  if (strcmp (m_third, rhs.m_third) > 0)
    return false;

  return false;
}

