#include <array>
#include <utility>
#include <type_traits>
#include <gtest/gtest.h>
#include "apply_ref_from_to.h"

#ifdef WITH_STATIC_ASSERT
namespace {
template <typename T>
constexpr bool static_assert_tests(const T it, const T end);

template <typename T>
constexpr bool static_assert_tests_implem(const T it, const T end)
{
    /* this is pure bullshit, constexpr environment, but can use neither:
     * . it->first as condition for static_assert
     * . it->second as string for static_assert (actully, not even a basic constexpr const* const char works...)
     */
    //static_assert((it->first == true), "apply_ref_from_t failure");
    return static_assert_tests(it + 1, end);
}

template <typename T>
constexpr bool static_assert_tests(const T it, const T end)
{
    return it == end || static_assert_tests_implem(it, end);
}
}
#endif	// WITH_STATIC_ASSERT

TEST(apply_ref_from_to_tTest, Foo) {
    struct From {};
    struct To {};
    using namespace broken_algo;

    constexpr auto tests = {
        std::make_pair(std::is_same<apply_ref_from_to_t<From&, To>, To& >(), "From& -> To&")
        , std::make_pair(std::is_same<apply_ref_from_to_t<From&, To>, To& >(), "From&& -> To&&")
        , std::make_pair(std::is_same<apply_ref_from_to_t<From&, To>, To& >(), "const From& -> const To&")
        , std::make_pair(std::is_same<apply_ref_from_to_t<From&, To>, To& >(), "const From&& -> const To&&")
    };

#ifdef WITH_STATIC_ASSERT
    {
        constexpr auto begin = tests.begin();
        constexpr auto end = tests.end();
        constexpr auto dummy = static_assert_tests(begin, end);

        {
            constexpr auto condition = (begin + 0)->first;
            static_assert(condition, "apply_ref_from_to_t failure");
        }
        {
            constexpr auto condition = (begin + 1)->first;
            static_assert(condition, "apply_ref_from_to_t failure");
        }
        {
            constexpr auto condition = (begin + 2)->first;
            static_assert(condition, "apply_ref_from_to_t failure");
        }
        {
            constexpr auto condition = (begin + 3)->first;
            static_assert(condition, "apply_ref_from_to_t failure");
        }
    }
#endif	// WITH_STATIC_ASSERT
    for (auto& test : tests)
    {
        ASSERT_TRUE((test.first)) << test.second;
    }
}
