#include "line_edit_w_hint.h"
#include <QRegularExpression>

line_edit_w_hint::line_edit_w_hint (QWidget *parent)
  : QObject (parent),
    m_le ()
{
  connect (&m_le, SIGNAL (textChanged (const QString &)), this, SLOT (update_text (const QString &)));
}

line_edit_w_hint::~line_edit_w_hint ()
{

}

QLineEdit *line_edit_w_hint::as_qlineedit ()
{
  return &m_le;
}

const QLineEdit *line_edit_w_hint::as_qlineedit() const
{
  return &m_le;
}

QString line_edit_w_hint::text () const
{
  return m_last_text;
}

void line_edit_w_hint::update_text (const QString &new_text)
{
  if (QRegularExpression ("^\\s*$").match (new_text).hasMatch ())
    {
      m_last_text = new_text;
      text_changed (hint::clear, new_text);
      return;
    }

  if (new_text.indexOf (m_last_text, 0) == 0)
    {
      QString hint_str = new_text.right (new_text.size () - m_last_text.size ());
      m_last_text = new_text;
      text_changed (hint::append, hint_str);
      return;
    }

  m_last_text = new_text;
  text_changed (hint::other, m_last_text);
}
