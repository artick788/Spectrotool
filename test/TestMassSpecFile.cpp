#include "TestMassSpecFile.hpp"

void TestMassSpecFile::SetUp() {
    m_HappyDayFile = fs::current_path() / "TestResources" / "220115_PFASFORWARD_VMM.xlsx";
}

void TestMassSpecFile::TearDown() {

}


TEST_F(TestMassSpecFile, TestHappyDayParse) {
    MassSpecFile worksheet(m_HappyDayFile);
    EXPECT_EQ(worksheet.getCompounds().size(), 71);

    // grab compound Me PFBSA
    auto c1 = worksheet.getCompound("Me PFBSA");
    EXPECT_EQ(c1.size(), 1); // should only be one compound with this name

    // grab compound m-PFTeDA
    auto c2 = worksheet.getCompound("m-PFTeDA");
    EXPECT_EQ(c2.size(), 1); // should only be one compound with this name

    // grab compound m-PFPeA
    auto c3 = worksheet.getCompound("m-PFPeA");
    EXPECT_EQ(c3.size(), 2); // should only be one compound with this name
}
