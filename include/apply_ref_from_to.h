#ifndef APPLY_REF_FROM_TO_H_
# define APPLY_REF_FROM_TO_H_

namespace broken_algo
{

template <typename From, typename To>
struct apply_ref_from_to;

template <typename From, typename To>
struct apply_ref_from_to<From&, To>
{
    typedef To& type;
};

template <typename From, typename To>
struct apply_ref_from_to<const From&&, To>
{
    typedef const To&& type;
};

template <typename From, typename To>
struct apply_ref_from_to<const From&, To>
{
    typedef const To& type;
};

template <typename From, typename To>
struct apply_ref_from_to<From&&, To>
{
    typedef To&& type;
};

template <typename From, typename To>
using apply_ref_from_to_t = typename apply_ref_from_to<From, To>::type;

}

#endif // !APPLY_REF_FROM_TO_H_
