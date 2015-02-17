#ifndef BROKEN_ALGO__BREAKER_H_
# define BROKEN_ALGO__BREAKER_H_

# include <functional>
# include <type_traits>
# include <array>
# include <cassert>
# include "fake_instance.h"
# include "apply_ref_from_to.h"
# include "mono_allocator.h"

namespace broken_algo
{

template <typename T, template <typename...> class Allocator>
struct breaker_t;

template <typename T>
struct is_breaker : std::false_type
{};

template <typename T, template <typename...> class Allocator>
struct is_breaker<breaker_t<T, Allocator> > : std::true_type
{};

template <typename breaker>
struct breaker_underlying_type;

template <typename T, template <typename...> class Allocator>
struct breaker_underlying_type<breaker_t<T, Allocator> >
{
    typedef T type;
};

template <typename breaker>
using breaker_underlying_type_t = typename breaker_underlying_type<breaker>::type;

template <typename T, template <typename...> class Allocator = mono_allocator>
struct breaker_t
{
    template <typename, template <typename...> class>
    friend class breaker_t;

    constexpr breaker_t() : t_(nullptr)
    {}

    template <typename U,
              std::enable_if_t<
                  std::is_same<
                      std::decay_t<U>, T
                      >::value
                  >* = nullptr
              >
    breaker_t(U&& cpy)
        : t_(make_unique(std::forward<U>(cpy)))
    {
    }

    template <typename U,
              std::enable_if_t<
                  std::is_same<
                      std::decay_t<U>, T
                      >::value
                  >* = nullptr
              >
    breaker_t &operator=(U&& cpy)
    {
        if (t_)
        {
            *t_ = std::forward<U>(cpy);
        }
        else
        {
            t_ = make_unique(std::forward<U>(cpy));
        }
        return *this;
    }

    template <typename U,
              std::enable_if_t<
                  is_breaker<std::decay_t<U> >::value
                  && std::is_same<breaker_underlying_type_t<std::decay_t<U> >, T>::value
                  >* = nullptr
              >
    breaker_t(U&& rhs)
    {
        if (rhs.t_)
        {
            t_ = make_unique(static_cast<apply_ref_from_to_t<U, T> >(*rhs.t_));
        }
    }

    template <typename U,
              std::enable_if_t<
                  is_breaker<std::decay_t<U> >::value
                  && std::is_same<breaker_underlying_type_t<std::decay_t<U> >, T>::value
                  >* = nullptr
              >
    breaker_t &operator=(U&& rhs)
    {
        if (t_ != rhs.t_)
        {
            if (t_)
            {
                if (!rhs.t_)
                {
                    t_.reset();
                }
                else
                {
                    *t_ = static_cast<apply_ref_from_to_t<U, T> >(*rhs.t_);
                }
            }
            else
            {
                t_ = make_unique(static_cast<apply_ref_from_to_t<U, T> >(*rhs.t_));
            }
        }
        return *this;
    }

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
    Allocator<T> allocator_;
    std::function<void(T*)> deleter_{[this](T* ptr)
    {
            this->allocator_.deallocate(ptr, 1);
    }};
    template <typename ...Args>
    auto make_unique(Args&&... args)
    {
        return decltype(t_)(new (this->allocator_.allocate(1)) T(std::forward<Args>(args)...), deleter_);
    }
    std::unique_ptr<T, decltype(deleter_) > t_;
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

constexpr breaker_t<br> breaker;
constexpr breaker_t<no_br> no_breaker;

}

#endif // !BROKEN_ALGO__BREAKER_H_
