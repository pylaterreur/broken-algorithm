#include <gtest/gtest.h>
#include "algorithm.h"
#include "breaker.h"

static constexpr auto l = {1, 2, 3, 4, 5};

TEST(AlgorithmTest, StdForEach) {
    int result = 0;
    const int expected_result = 12345;

    broken_algo::for_each(l.begin(), l.end(),
             [&result](auto elem) -> void {
        result = result * 10 + elem;
    }
    );
    ASSERT_EQ(result, expected_result);
}

TEST(AlgorithmTest, BreakForEach) {
    int result = 0;
    const int expected_result = 123;
    using namespace broken_algo;

    broken_algo::for_each(l.begin(), l.end(),
             [&result](auto elem) -> breaker_t<void> {
        do
        {
            if (elem == 4)
            {
                break;
            }
            result = result * 10 + elem;
            return no_breaker;
        } while (0);
        return breaker;
    }
    );
    ASSERT_EQ(result, expected_result);
}

TEST(AlgorithmTest, StdAnyOf) {
    bool result = broken_algo::any_of(l.begin(), l.end(),
             [](auto elem) -> bool {
        return elem == 4;
    }
    );
    ASSERT_TRUE(result);
}

TEST(AlgorithmTest, BreakAnyOf) {
    using namespace broken_algo;

    auto lambda = [](auto f){
        return broken_algo::any_of(l.begin(), l.end(), f);
    };
    ASSERT_FALSE(lambda([](auto elem) -> breaker_t<bool> {
        do
        {
        if (elem == 3)
        {
            break;
        }
        return elem == 4;
        }
        while (0);
        return breaker;
    }));
    ASSERT_TRUE(lambda([](auto elem) -> breaker_t<bool> {
        return elem == 4;
    }));
    ASSERT_FALSE(lambda([](auto elem) -> breaker_t<bool> {
        return elem == 6;
    }));
}
