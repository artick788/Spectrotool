#include <gtest/gtest.h>

#include "../include/Spectrotool/DataTable.hpp"
#include "../include/Spectrotool/Parsers/DataTableParser.hpp"
#include "../include/Spectrotool/Parsers/SampleListParser.hpp"

using namespace Spectrotool;

class TestDataTable: public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }

    static void printMessages(ParserMessages& msgs) {
        for (const auto&[severity, message, address, sheet]: msgs) {
            std::cout << "[" << sheet << "] [" << address << "] [" << severity << "]: " << message << std::endl;
        }
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

TEST_F(TestDataTable, AddSampleList) {
    DataTableDesc dataDesc;
    dataDesc.filePath = fs::current_path() / "TestResources" / "SampleList" / "SampleListData.xlsx";

    DataTable table;
    ParserMessages msgs = parseDataTable(dataDesc, table);
    printMessages(msgs);

    SampleListDesc listDesc;
    listDesc.filePath = fs::current_path() / "TestResources" / "SampleList" / "SampleList.xlsx";

    SampleList list;
    msgs = parseSampleList(listDesc, list);

    table.setSampleList(list);

    ASSERT_TRUE(table.hasCompound("A"));
    const auto& a = table.getCompound("A");
    const auto& aS1 = a["Test1"];
    EXPECT_EQ(aS1.weight, 10);
    EXPECT_EQ(aS1.matrix, "Soil");

    ASSERT_TRUE(table.hasCompound("B"));
    const auto& b = table.getCompound("B");
    const auto& bS2 = b["Test2"];
    EXPECT_EQ(bS2.weight, 20);
    EXPECT_EQ(bS2.matrix, "Water");

    ASSERT_TRUE(table.hasCompound("C"));
    const auto& c = table.getCompound("C");
    const auto& cS3 = c["Test3"];
    EXPECT_EQ(cS3.weight, 30);
    EXPECT_EQ(cS3.matrix, "Air");
}
