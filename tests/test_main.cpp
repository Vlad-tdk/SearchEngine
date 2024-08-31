//
// Created by Vladimir Martemianov on 31.8.24..
//
#include "gtest/gtest.h"

TEST(SampleTestCase, SampleTest) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
