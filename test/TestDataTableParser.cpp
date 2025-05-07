#include <gtest/gtest.h>

#include "../include/Spectrotool/Parsers/DataTableParser.hpp"

using namespace Spectrotool;

class TestDataTableParser : public ::testing::Test {
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

TEST_F(TestDataTableParser, HappyDayParse) {
    DataTableDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "220115_PFASFORWARD_VMM.xlsx";

    DataTable table;
    ParserMessages msgs = parseDataTable(desc, table);
    printMessages(msgs);

    EXPECT_EQ(table.getCompounds().size(), 71);
    EXPECT_EQ(table.getSampleCount(), 6382);

    // grab compound Me PFBSA
    EXPECT_TRUE(table.hasCompound("Me_PFBSA"));

    // grab compound m-PFTeDA
    EXPECT_TRUE(table.hasCompound("m-PFTeDA"));

    // grab compound m-PFPeA
    EXPECT_TRUE(table.hasCompound("m-PFPeA"));
}

TEST_F(TestDataTableParser, HappyDaySmallParseWithFilter) {
    DataTableDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDaySmall" / "220115_PFASFORWARD_VMM.xlsx";

    DataTable table;
    ParserMessages msgs = parseDataTable(desc, table);
    printMessages(msgs);

    // Sanity check
    auto compounds = table.getCompounds();
    ASSERT_EQ(table.getCompounds().size(), 2);
    auto mePFBSA = compounds[0];
    EXPECT_EQ(mePFBSA.getValues().size(), 89);
    auto pfbs1 = compounds[1];
    EXPECT_EQ(pfbs1.getValues().size(), 89);

    // Now read again with filter
    desc.excludeCompoundFilter = {"Me"};
    DataTable table2;
    msgs = parseDataTable(desc, table2);
    compounds = table2.getCompounds();
    ASSERT_EQ(table2.getCompounds().size(), 1);
    auto pfbs2 = compounds[0];
    EXPECT_EQ(pfbs2.getValues().size(), 89); // ensure that subsequent compounds are not added to the previous compound
}

TEST_F(TestDataTableParser, ReplacementRules) {
    DataTableDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "ReplacementRules.xlsx";
    DataTable table;
    ParserMessages msgs = parseDataTable(desc, table);
    printMessages(msgs);

    EXPECT_FALSE(table.hasCompound("Me-PFBSA:"));
    ASSERT_TRUE(table.hasCompound("Me-PFBSA_"));

    const auto pfbsa = table.getCompound("Me-PFBSA_");
    const CompoundValue& v1 = pfbsa.getValues()[0];
    EXPECT_EQ(v1.name, "250115-VMM-Frouk__e-003");
    EXPECT_EQ(v1.id, "BL1_Soil");
}

