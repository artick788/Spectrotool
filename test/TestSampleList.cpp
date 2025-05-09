#include <gtest/gtest.h>

#include "../include/Spectrotool/SampleList.hpp"

using namespace Spectrotool;

class TestSampleList: public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }
};

TEST_F(TestSampleList, HappyFlow) {
    SampleList list;

    SampleInfo s1;
    s1.id = "A";
    s1.weight = 2.0;
    s1.matrix = "Soil";
    list["A"] = s1;

    SampleInfo s2;
    s2.id = "B";
    s2.weight = 5.0;
    s2.matrix = "Water";
    list["B"] = s1;

    EXPECT_TRUE(list.hasSample("A"));
    EXPECT_FALSE(list.hasSample("C"));
    EXPECT_EQ(list.getSamples().size(), 2);
}