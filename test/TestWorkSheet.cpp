#include "TestWorkSheet.hpp"

void TestWorkSheet::SetUp() {
    m_HappyDayFile = fs::current_path() / "TestResources" / "220115_PFASFORWARD_VMM.xlsx";
}

void TestWorkSheet::TearDown() {

}


TEST_F(TestWorkSheet, TestHappyDayParse) {
    Worksheet worksheet(m_HappyDayFile);
    EXPECT_EQ(worksheet.getPolarCompounds().size(), 25);
    EXPECT_EQ(worksheet.getNeutralCompounds().size(), 25);
    EXPECT_EQ(worksheet.getApolarCompounds().size(), 21);

    // grab polar compound Me PFBSA
    auto polarCompounds = worksheet.getCompound("Me PFBSA", ST_COMPOUND_TYPE_POLAR);
    EXPECT_EQ(polarCompounds.size(), 1); // should only be one compound with this name

    // grab neutral compound m-PFTeDA
    auto neutralCompounds = worksheet.getCompound("m-PFTeDA", ST_COMPOUND_TYPE_NEUTRAL);
    EXPECT_EQ(neutralCompounds.size(), 1); // should only be one compound with this name

    // grab apolar compound m-PFPeA
    auto apolarCompounds = worksheet.getCompound("m-PFPeA", ST_COMPOUND_TYPE_APOLAR);
    EXPECT_EQ(apolarCompounds.size(), 1); // should only be one compound with this name
}
