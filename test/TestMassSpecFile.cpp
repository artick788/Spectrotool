#include "TestMassSpecFile.hpp"

void TestMassSpecFile::SetUp() {

}

void TestMassSpecFile::TearDown() {

}

TEST_F(TestMassSpecFile, HappyDayParse) {
    MassSpecFileDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "220115_PFASFORWARD_VMM.xlsx";
    MassSpecFile worksheet(desc);
    EXPECT_EQ(worksheet.getCompounds().size(), 71);
    EXPECT_EQ(worksheet.getSampleCount(), 6382);

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

TEST_F(TestMassSpecFile, HappyDayParseWithFilter) {
    MassSpecFileDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDaySmall" / "220115_PFASFORWARD_VMM.xlsx";
    desc.sheetNames = {"Polar"};
    MassSpecFile worksheet(desc);

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
    MassSpecFile worksheet2(desc);
    compounds = worksheet2.getCompounds();
    EXPECT_EQ(worksheet2.getCompounds().size(), 1);
    if (compounds.size() == 1) {
        auto pfbs = compounds[0];
        EXPECT_EQ(pfbs.getValues().size(), 89); // ensure that subsequent compounds are not added to the previous compound
    }
}

TEST_F(TestMassSpecFile, HappyDayExport) {
    MassSpecFileDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "220115_PFASFORWARD_VMM.xlsx";
    MassSpecFile worksheet(desc);

    MassSpecFileExportDesc exportDesc;
    exportDesc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "220115_PFASFORWARD_VMM_export.xlsx";
    worksheet.exportToExcel(exportDesc);

}

TEST_F(TestMassSpecFile, SmallHappyDayExport) {
    MassSpecFileDesc desc;
    desc.sheetNames = {"Polar"};
    desc.filePath = fs::current_path() / "TestResources" / "HappyDaySmall" / "220115_PFASFORWARD_VMM.xlsx";
    MassSpecFile worksheet(desc);

    MassSpecFileExportDesc exportDesc;
    exportDesc.filePath = fs::current_path() / "TestResources" / "HappyDaySmall" / "220115_PFASFORWARD_VMM_export.xlsx";
    worksheet.exportToExcel(exportDesc);

}
