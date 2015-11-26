#include <gtest/gtest.h>

int add(int value1, int value2) {
    return (value1 + value2);
}

TEST (AddTest, PositiveNos) {
    EXPECT_EQ (16, add(8, 8));
}