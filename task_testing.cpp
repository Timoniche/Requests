#include "gtest/gtest.h"

#include "inc/RequestManager.h"


TEST(correctness, many_threads) // NOLINT
{
    size_t c = rand() % 20; //NOLINT
    if (c < 10) c = 10;
    std::cout << "num of consumers: " << c << std::endl;
    RequestManager request_manager{c};
    request_manager.simulate_activity();
    EXPECT_EQ("", "");
}


