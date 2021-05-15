#include <gtest/gtest.h>
#include "solver.hpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

GTEST_TEST(obviousTest, allFill) {
    for (size_t i = 1; i < 100; i++)
    {
        int kLength = (int)i;
        clues_t clues(1, kLength);
        nonogramSolver solver(kLength, clues);
        bool result = solver.solve();
        EXPECT_TRUE(result);
        EXPECT_EQ(solver.countFilledBox(), kLength);
    }
}

GTEST_TEST(obviousTest, allEmpty) {
    for (size_t i = 1; i < 100; i++)
    {
        int kLength = (int)i;
        clues_t clues(1, 0);
        nonogramSolver solver(kLength, clues);
        bool result = solver.solve();
        EXPECT_TRUE(result);
        EXPECT_EQ(solver.countFilledBox(), 0);
        EXPECT_EQ(solver.countEmptyBox(), kLength);
    }
}

GTEST_TEST(obviousTest, sumEqual) {
    for (int i = 2; i < 10; i++)
    {
        clues_t clues;
        for (int j = 0; j < i; j++)
        {
            clues.push_back(j + 1);
        }
        int kLength = (int)(clues.size() - 1 + ((i + 1) * i / 2));
        nonogramSolver solver(kLength, clues);
        bool result = solver.solve();
        EXPECT_TRUE(result);
    }
}

GTEST_TEST(partialFix, fourClues)
{
    clues_t clues;
    clues.push_back(3);
    clues.push_back(4);
    clues.push_back(2);
    clues.push_back(5);
    nonogramSolver solver(20, clues);
    patterns_t pattern(20, STATE_UNCERTAIN);
    pattern[7] = STATE_FILL;
    pattern[8] = STATE_FILL;
    pattern[12] = STATE_FILL;
    solver.overwritePattern(pattern);
    bool result = solver.solve();
    EXPECT_FALSE(result);
    EXPECT_EQ(solver.countFilledBox(), 7);
    EXPECT_EQ(solver[19], STATE_UNCERTAIN);
    EXPECT_EQ(solver[18], STATE_FILL);
}

GTEST_TEST(obviousTest, startFromEdge) {
    {
        // FFEEE
        clues_t clues(1, 2);
        nonogramSolver solver(5, clues);
        patterns_t pattern(5, STATE_UNCERTAIN);
        pattern[0] = STATE_FILL;
        solver.overwritePattern(pattern);
        bool result = solver.solve();
        EXPECT_TRUE(result);
        EXPECT_EQ(solver.countFilledBox(), 2);
    }
    {
        // FFEFF
        clues_t clues(2, 2);
        nonogramSolver solver(5, clues);
        patterns_t pattern(5, STATE_UNCERTAIN);
        pattern[0] = STATE_FILL;
        pattern[4] = STATE_FILL;
        solver.overwritePattern(pattern);
        bool result = solver.solve();
        EXPECT_TRUE(result);
        EXPECT_EQ(solver.countFilledBox(), 4);
        EXPECT_EQ(solver[1], STATE_FILL);
        EXPECT_EQ(solver[3], STATE_FILL);
    }
}

GTEST_TEST(partialFix, singleClue) {
    {
        // UUUUUUUUFFFFUUUUUUUU
        clues_t clues(1, 12);
        int kLength = 20;
        nonogramSolver solver(kLength, clues);
        bool result = solver.solve();
        EXPECT_FALSE(result);
        EXPECT_EQ(solver.countFilledBox(), 4);
        EXPECT_EQ(solver[8], STATE_FILL);
        EXPECT_EQ(solver[9], STATE_FILL);
        EXPECT_EQ(solver[10], STATE_FILL);
        EXPECT_EQ(solver[11], STATE_FILL);
        EXPECT_EQ(solver[7], STATE_UNCERTAIN);
        EXPECT_EQ(solver[12], STATE_UNCERTAIN);
    }
    {
        clues_t clues(1, 51);
        int kLength = 100;
        nonogramSolver solver(kLength, clues);
        bool result = solver.solve();
        EXPECT_FALSE(result);
        EXPECT_EQ(solver.countFilledBox(), 2);
    }
}

// UFUUU
GTEST_TEST(partialFix, twoClues) {
    clues_t clues;
    clues.push_back(2);
    clues.push_back(1);
    int kLength = 5;
    nonogramSolver solver(kLength, clues);
    bool result = solver.solve();
    EXPECT_FALSE(result);
    EXPECT_EQ(solver.countFilledBox(), 1);
    EXPECT_EQ(solver[1], STATE_FILL);
    EXPECT_EQ(solver[0], STATE_UNCERTAIN);
    EXPECT_EQ(solver[4], STATE_UNCERTAIN);
}

GTEST_TEST(partialFix, multipleClues) {
    {
        // UUFUUUUUUU
        clues_t clues;
        clues.push_back(3);
        clues.push_back(2);
        clues.push_back(1);
        int kLength = 10;
        nonogramSolver solver(kLength, clues);
        bool result = solver.solve();
        EXPECT_FALSE(result);
        EXPECT_EQ(solver.countFilledBox(), 1);
        EXPECT_EQ(solver[2], STATE_FILL);
        EXPECT_EQ(solver[1], STATE_UNCERTAIN);
        EXPECT_EQ(solver[3], STATE_UNCERTAIN);
    }
    {
        clues_t clues;
        clues.push_back(3);
        clues.push_back(10);
        clues.push_back(3);
        int kLength = 20;
        nonogramSolver solver(kLength, clues);
        bool result = solver.solve();
        EXPECT_FALSE(result);
        EXPECT_EQ(solver.countFilledBox(), 10);
        EXPECT_EQ(solver[2],  STATE_FILL);
        EXPECT_EQ(solver[17], STATE_FILL);
        EXPECT_EQ(solver[1],  STATE_UNCERTAIN);
        EXPECT_EQ(solver[3],  STATE_UNCERTAIN);
        EXPECT_EQ(solver[16], STATE_UNCERTAIN);
        EXPECT_EQ(solver[18], STATE_UNCERTAIN);
    }
}
