#include "TestSampleListFile.hpp"

void TestSampleListFile::SetUp() {

}

void TestSampleListFile::TearDown() {

}

TEST_F(TestSampleListFile, HappyDayParse) {
    SampleListFileDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "samplelist.xlsx";
    SampleListFile f(desc);

    const auto& samples = f.getSamples();
    EXPECT_EQ(samples.size(), 84);
    EXPECT_TRUE(f.hasSample("BL1_Soil"));
    EXPECT_TRUE(f.hasSample("BL2_Soil"));
    EXPECT_TRUE(f.hasSample("15)PF1_1.5_8_Soil"));

    // Inspect one sample further
    const auto& sample = samples.at("BL1_Soil");
    EXPECT_EQ(sample.id, "BL1_Soil");
    EXPECT_NEAR(sample.weight, 7.82f, 0.01);
    EXPECT_EQ(sample.matrix, "Soil");
}

TEST_F(TestSampleListFile, CorrectionFactorParse) {
    SampleListFileDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "CorrectionFactor" / "Spectrotool_Verifier_SampleList.xlsx";
    EXPECT_NO_THROW(SampleListFile f(desc));
}
