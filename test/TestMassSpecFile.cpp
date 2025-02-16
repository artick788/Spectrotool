#include "TestMassSpecFile.hpp"

void TestMassSpecFile::SetUp() {

}

void TestMassSpecFile::TearDown() {

}

TEST_F(TestMassSpecFile, TestHappyDayParse) {
    fs::path happyDayFile = fs::current_path() / "TestResources" / "HappyDay" / "220115_PFASFORWARD_VMM.xlsx";
    MassSpecFile worksheet(happyDayFile);
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

TEST_F(TestMassSpecFile, TestHappyDayParseWithFilter) {
    fs::path smallHappyDayFile = fs::current_path() / "TestResources" / "HappyDaySmall" / "220115_PFASFORWARD_VMM.xlsx";
    MassSpecFileDesc desc;
    desc.sheetNames = {"Polar"};
    MassSpecFile worksheet(smallHappyDayFile, desc);

    // Sanity check
    auto compounds = worksheet.getCompounds();
    EXPECT_EQ(worksheet.getCompounds().size(), 2);
    if (compounds.size() == 2) {
        auto mePFBSA = compounds[0];
        EXPECT_EQ(mePFBSA.getValues().size(), 89);
        auto pfbs = compounds[1];
        EXPECT_EQ(pfbs.getValues().size(), 89);
    }

    // Now read again with filter
    desc.excludeFilter = "Me";
    MassSpecFile worksheet2(smallHappyDayFile, desc);
    compounds = worksheet2.getCompounds();
    EXPECT_EQ(worksheet2.getCompounds().size(), 1);
    if (compounds.size() == 1) {
        auto pfbs = compounds[0];
        EXPECT_EQ(pfbs.getValues().size(), 89); // ensure that subsequent compounds are not added to the previous compound
    }
}
