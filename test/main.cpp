#include <iostream>
#include <vector>
#include "algorithm.h"

using namespace broken_algo;

int main()
{
    std::vector<int> l = {1, 2, 3, 4, 5};
    broken_algo::for_each(l.begin(), l.end(),
             [](auto elem) -> breaker_t<void> {
        do
        {
            if (elem == 3)
            {
                break;
            }
            std::cout << elem << std::endl;
            return no_breaker;
        } while (0);
        return breaker;
    }
    );

    broken_algo::any_of(l.begin(), l.end(),
             [](auto elem) -> breaker_t<bool> {
        do
        {
            if (elem == 4)
            {
                break;
            }
            std::cout << elem << std::endl;
            return false;
        } while (0);
        return breaker;
    }
    );


    breaker_t<bool> b(false);
    breaker_t<bool> b2 = b;
    b2 = b;
}
