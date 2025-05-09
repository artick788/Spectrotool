#include <gtest/gtest.h>

#include "../../include/Spectrotool/Parsers/SampleListParser.hpp"

using namespace Spectrotool;

class TestSampleListParser : public ::testing::Test {
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

TEST_F(TestSampleListParser, HappyDayParse) {
    SampleListDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "samplelist.xlsx";
    SampleList f;
    ParserMessages msgs = parseSampleList(desc, f);
    printMessages(msgs);

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