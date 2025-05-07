#include <gtest/gtest.h>

#include "../include/Spectrotool/DataTable.hpp"

using namespace Spectrotool;

class TestDataTable: public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }
};

TEST_F(TestDataTable, HappyFlow) {
    DataTable table;

    Compound c1("A");
    Compound c2("B");
    table.addCompound(std::move(c1));
    table.addCompound(std::move(c2));

    EXPECT_TRUE(table.hasCompound("A"));
    EXPECT_TRUE(table.hasCompound("B"));
    EXPECT_FALSE(table.hasCompound("C"));

    for (const auto& compound : table) {
        EXPECT_TRUE((compound.getName() ==  "A") || (compound.getName() == "B"));
    }
}

TEST_F(TestDataTable, DuplicateCompounds) {
    DataTable table;

    Compound c1("A");
    Compound c2("A");
    table.addCompound(std::move(c1));
    table.addCompound(std::move(c2));

    EXPECT_TRUE(table.hasCompound("A"));
    EXPECT_EQ(table.getCompounds().size(), 2);
}
