#ifndef ALGORITHM_H_
# define ALGORITHM_H_

# include <algorithm>
# include <type_traits>
# include "breaker.h"

namespace broken_algo
{

template <class InputIterator, class Function>
auto for_each(InputIterator first, InputIterator last, Function fn)
-> std::enable_if_t<!is_breaker<decltype(fn(*first))>::value, Function>
{
    return std::for_each(first, last, fn);
}

template <class InputIterator, class Function>
auto for_each(InputIterator first, InputIterator last, Function fn)
-> std::enable_if_t<is_breaker<decltype(fn(*first))>::value, Function>
{
    while (first!=last) {
        auto ret = fn (*first);
      if (!ret) break;
      ++first;
    }
    return fn;
}

template<class InputIterator, class UnaryPredicate>
auto any_of(InputIterator first, InputIterator last, UnaryPredicate pred)
-> std::enable_if_t<!is_breaker<decltype(pred(*first))>::value, bool>
{
    return std::any_of(first, last, pred);
}

template<class InputIterator, class UnaryPredicate>
auto any_of(InputIterator first, InputIterator last, UnaryPredicate pred)
-> std::enable_if_t<is_breaker<decltype(pred(*first))>::value, bool>
{
  while (first!=last) {
    auto ret = pred(*first);
    if (!ret) break;
    if (*ret) return true;
    ++first;
  }
  return false;
}
}

#endif // ALGORITHM_H_
