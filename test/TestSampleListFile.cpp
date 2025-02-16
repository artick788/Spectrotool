#include "TestSampleListFile.hpp"

void TestSampleListFile::SetUp() {

}

void TestSampleListFile::TearDown() {

}

TEST_F(TestSampleListFile, HappyDayParse) {
    SampleListFileDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "samplelist.xlsx";
    SampleListFile worksheet(desc);

    const auto& samples = worksheet.getSamples();
    EXPECT_EQ(samples.size(), 84);

}
