#include <gtest/gtest.h>

#include "../../include/Spectrotool/Parsers/DataTableParser.hpp"

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

TEST_F(TestDataTableParser, Filter) {
    // Without filter
    DataTableDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "Filter" / "Filter.xlsx";

    DataTable unfiltered;
    ParserMessages msgs = parseDataTable(desc, unfiltered);
    printMessages(msgs);

    const auto& compounds = unfiltered.getCompounds();
    ASSERT_EQ(compounds.size(), 2);
    EXPECT_EQ(compounds[0].getName(), "PFBSA");
    EXPECT_EQ(compounds[0].getValues().size(), 5);
    EXPECT_EQ(compounds[1].getName(), "m-PFBSA");
    EXPECT_EQ(compounds[1].getValues().size(), 5);

    // with filter
    desc.excludeCompoundFilter = {"m-"};
    DataTable filtered;
    msgs = parseDataTable(desc, filtered);
    printMessages(msgs);

    const auto& compounds1 = filtered.getCompounds();
    ASSERT_EQ(compounds1.size(), 1);
    EXPECT_EQ(compounds1[0].getName(), "PFBSA");
    EXPECT_EQ(compounds1[0].getValues().size(), 5); // Following values not added to previous compound

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

