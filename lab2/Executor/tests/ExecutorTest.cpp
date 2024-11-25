#include <gtest/gtest.h>

#include "func.h"

struct TestData {
    char operation;
    Position exceptedPos;
};

std::vector<TestData> testCases = {{'m', Position{0, 10, W}}, {'r', Position{0, 10, N}}, {'l', Position{0, 10, W}}};

Executor car1, car2(10, 10, W);
Position defaultPos1, *defaultPos2, *car1Pos;
TEST(Executor, functions)
{
    // should return default pose when without init and command
    EXPECT_EQ(defaultPos1, car1.pos);

    // tests Executor::GetInstructions(char key)
    for (const auto& testCases : testCases) {
        car2.GetInstructions(testCases.operation);
        EXPECT_EQ(testCases.exceptedPos, car2.pos);
    }

    // tests Executor::GetInstructions(char key)
    defaultPos2 = new Position;
    car1Pos = car1.GetPositon();
    EXPECT_EQ(*defaultPos2, *car1Pos);
}