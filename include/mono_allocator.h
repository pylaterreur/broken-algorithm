#ifndef MONO_ALLOCATOR_H_
# define MONO_ALLOCATOR_H_

# include <cassert>
# include <memory>
# include <type_traits>

#include <iostream>
#include <ios>

namespace broken_algo
{
template <typename T>
struct mono_allocator
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
//    using propagate_on_container_move_assignment = std::false_type;
    template<typename U> struct rebind { typedef mono_allocator<U> other; };
    using is_always_equal = std::false_type;

    mono_allocator() noexcept = default;
    mono_allocator(const mono_allocator& other) noexcept = default;
    template <typename U> mono_allocator(const mono_allocator<U>& ) noexcept {}
    ~mono_allocator() noexcept = default;

    pointer address(reference x) const noexcept { return std::addressof(x); }
    const_pointer address(const_reference x) const noexcept { return std::addressof(x); }

    pointer allocate(size_type n, std::allocator<void>::const_pointer hint = 0) noexcept
    {
        assert(n == 1);
        assert(allocated == false);
#ifndef NDEBUG
        allocated = true;
#endif
        return reinterpret_cast<pointer>(buff_.data());
    }

    void deallocate(pointer p, size_type n) noexcept
    {
        assert(n == 1);
        assert(allocated == true);
        assert(reinterpret_cast<void*>(p) == reinterpret_cast<void*>(buff_.data()));
#ifndef NDEBUG
        allocated = false;
#endif
    }

    size_type max_size() const noexcept { return 1; }

    void construct(pointer p, const_reference val)
    {
        new (reinterpret_cast<void*>(p)) T(val);
    }

    template <class... Args>
    void construct(pointer p, Args&&... args)
    {
        assert(reinterpret_cast<void*>(p) == reinterpret_cast<void*>(buff_.data_));
        new (reinterpret_cast<void*>(p)) T(std::forward<Args>(args)...);
    }

    template <typename U>
    bool operator==(const mono_allocator<U> &rhs) const noexcept
    {
        return false;
    }

    template <typename U>
    bool operator!=(const mono_allocator<U> &rhs) const noexcept
    {
        return !operator==(rhs);
    }
private:
    std::array<char, sizeof(value_type)> buff_;
#ifndef NDEBUG
    bool allocated{false};
#endif
};
}


#endif // !MONO_ALLOCATOR_H_
