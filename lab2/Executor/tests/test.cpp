#include <gtest/gtest.h>

#include "func.h"
Executor fault;
Pos *pos = new Pos(0, 0, 'N');
TEST(func, test_case1)
{
    EXPECT_EQ(*fault.GetPos(), *pos);
}