#include <gtest/gtest.h>
#include "breaker.h"

template <typename T>
struct my_breaker_default_storage : broken_algo::breaker_default_storage<T>
{};

TEST(BreakerTest, StorageAgnostism) {
    using namespace broken_algo;

    breaker_t<bool, my_breaker_default_storage> b(false);
    breaker_t<bool> b2 = b;
    b2 = b;
}
