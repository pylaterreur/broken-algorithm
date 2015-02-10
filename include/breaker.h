#ifndef BROKEN_ALGO__BREAKER_H_
# define BROKEN_ALGO__BREAKER_H_

# include <type_traits>
# include <array>
# include "fake_instance.h"

namespace broken_algo
{

template <typename T, template <typename...> class Storage>
struct breaker_t;

template <typename breaker>
struct breaker_underlying_type;

template <typename T, template <typename...> class Storage>
struct breaker_underlying_type<breaker_t<T, Storage> >
{
    typedef T type;
};

template <typename breaker, class Enable = void>
struct breaker_default_storage
{};

template <typename breaker>
struct breaker_default_storage<breaker, typename std::enable_if<!std::is_void<typename breaker_underlying_type<breaker>::type>::value>::type>
{
protected:
    typename breaker_underlying_type<breaker>::type *internal()
    {
        return const_cast<typename breaker_underlying_type<breaker>::type*>(const_cast<const breaker_default_storage&>(*this).internal());
    }

    const typename breaker_underlying_type<breaker>::type *internal() const
    {
        return reinterpret_cast<const typename breaker_underlying_type<breaker>::type *>(buff_.data());
    }
private:
    std::array<unsigned char, sizeof(typename breaker_underlying_type<breaker>::type)> buff_;
};

template <typename T, template <typename...> class Storage = breaker_default_storage>
struct breaker_t : private Storage<breaker_t<T, Storage> >
{
    constexpr breaker_t() : t_(nullptr)
    {}

    template <typename U,
              typename std::enable_if<
                  std::is_same<
                      typename std::decay<U>::type, T
                      >::value
                  >::type* = nullptr
              >
    breaker_t(U&& cpy)
        : t_{new (this->internal()) T(std::forward<U>(cpy))}
    {}

    // returns false if must break
    template <typename Dumy = int>
    operator bool() const
    {
        return t_ != nullptr;
    }

    const T& operator*() const
    {
        assert(t_ != nullptr);
        return *t_;
    }

    T& operator*()
    {
        return const_cast<T&>(*const_cast<const breaker_t&>(*this));
    }

private:
    T* t_;
};

template <template <typename...> class Storage>
struct breaker_t<void, Storage>
{
    constexpr explicit breaker_t(bool must_not_break) : t_(must_not_break)
    {}

    breaker_t(const breaker_t &) = default;
    breaker_t(breaker_t &&) = default;

    // returns false if must break
    operator bool() const
    {
        return t_;
    }

private:
    bool t_;
};

struct no_br
{};

struct br
{};

template <template <typename...> class Storage>
struct breaker_t<br, Storage>
{
    constexpr breaker_t()
    {}

    breaker_t(const breaker_t&) = default;
    breaker_t(breaker_t&&) = default;

    // returns false if must break
    operator bool() const
    {
        return true;
    }

    template <typename T, template <typename...> class Storage2>
    operator breaker_t<T, Storage2>() const
    {
        return breaker_t<T, Storage2>();
    }

    template <template <typename...> class Storage2>
    operator breaker_t<void, Storage2>() const
    {
        return breaker_t<void, Storage2>(false);
    }
};

template <template <typename...> class Storage>
struct breaker_t<no_br, Storage>
{
    constexpr breaker_t()
    {}

    breaker_t(const breaker_t&) = default;
    breaker_t(breaker_t&&) = default;

    // returns false if must break
    operator bool() const
    {
        return false;
    }

    template <typename T, template <typename...> class Storage2>
    operator breaker_t<T, Storage2>() const
    {
        return T();
    }

    template <template <typename...> class Storage2>
    operator breaker_t<void, Storage2>() const
    {
        return breaker_t<void, Storage2>(true);
    }
};

template <typename T>
struct is_breaker : std::false_type
{};

template <typename T, template <typename...> class Storage>
struct is_breaker<breaker_t<T, Storage> > : std::true_type
{};

constexpr breaker_t<br> breaker;
constexpr breaker_t<no_br> no_breaker;

}

#endif // !BROKEN_ALGO__BREAKER_H_