#ifndef SSN_HANDLE_H
#define SSN_HANDLE_H

#include <QString>

class ssn_handle
{
private:
  char m_first[5];
  char m_second[3];
  char m_third[6];
  bool m_is_valid;
public:
  ssn_handle ();
  ~ssn_handle ();

  //accepts both pretty and simple format,
  //whitespaces at the beginning and end are discarded
  ssn_handle (const QString &ssn);
  void set_from_string (const QString &ssn);

  bool is_valid () const;
  QString pretty () const; //123-45-6789
  QString simple () const; //123456789

  bool operator < (const ssn_handle &rhs) const;
};

#endif // SSN_HANDLE_H
