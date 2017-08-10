#ifndef ENUM_MISC_H
#define ENUM_MISC_H

#include "enum_range.h"
#include <cstring>

template<typename Enum, typename = use_if_enum<Enum>, typename = decltype (enum_to_string (Enum ()))>
Enum string_to_enum (const char *string)
{
  for (Enum e : enum_range<Enum> ())
    {
      if (!strcmp (enum_to_string (e), string))
        return e;
    }
  return enum_end (Enum ()); //implement enum_end (Enum) or Enum::COUNT;
}

template<typename Enum, typename = use_if_enum<Enum>>
constexpr int enum_size (Enum) {return static_cast<int> (enum_end (Enum ())) - static_cast<int> (enum_begin (Enum ()));}

template<typename T, typename Enum, typename = typename std::enable_if<std::is_integral<T>::value, T>::type, typename = use_if_enum<Enum>>
constexpr T enum_cast (Enum e) {return static_cast<T> (e) - static_cast<T> (enum_begin (Enum ()));}

template<typename Enum, typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type, typename = use_if_enum<Enum>>
constexpr Enum enum_cast (const T t, ...) {return static_cast<Enum> (t + static_cast<T> (enum_begin (Enum ())));}

#endif // ENUM_MISC_H
