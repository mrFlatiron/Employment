#ifndef LINE_EDIT_W_HINT_H
#define LINE_EDIT_W_HINT_H

#include <QLineEdit>

class line_edit_w_hint : public QObject
{
  Q_OBJECT
private:
  QLineEdit m_le;
  QString m_last_text;
public:
  enum class hint
  {
    append,
    clear,
    other,
    COUNT
  };

public:
  line_edit_w_hint (QWidget *parent = nullptr);
  ~line_edit_w_hint ();

  QLineEdit *as_qlineedit ();
  const QLineEdit *as_qlineedit () const;

  QString text () const;
signals:
  /* IMPORTANT
   * If type == hint::append, then hint_str is an "appendix"
   * If type == hint::clear, then hinst_str is a whole new string of type ^\s*$
   * If type == hint::other, then other is a whole new string
   */
  void text_changed (line_edit_w_hint::hint type, const QString &hint_str);
private slots:
  void update_text (const QString &new_text);
};

#endif // LINE_EDIT_W_HINT_H
