#include <gtest/gtest.h>

#include "../../include/Spectrotool/Parsers/CorrectionFactorParser.hpp"

using namespace Spectrotool;

class TestCorrectionFactorParser : public ::testing::Test {
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

TEST_F(TestCorrectionFactorParser, HappyDayParseVMM) {
    CorrectionFactorDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "CorrectionFactor" / "VMM.xlsx";

    CorrectionFactor factor;
    ParserMessages msgs = parseCorrectionFactor(desc, factor);
    printMessages(msgs);

    EXPECT_EQ(factor.getCorrectionValues().size(), 42);

    ASSERT_TRUE(factor.hasCorrectionValue("PFBA"));
    const auto& pfba = factor.getCorrectionValue("PFBA");
    EXPECT_NEAR(pfba.correctionFactor, 0.804898, 1e-4);
    EXPECT_EQ(pfba.quantityISTD, 20);
}

TEST_F(TestCorrectionFactorParser, HappyDayParseWellington) {
    CorrectionFactorDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "CorrectionFactor" / "Wellington.xlsx";

    CorrectionFactor factor;
    ParserMessages msgs = parseCorrectionFactor(desc, factor);
    printMessages(msgs);

    EXPECT_EQ(factor.getCorrectionValues().size(), 15);

    ASSERT_TRUE(factor.hasCorrectionValue("PFBS"));
    const auto& pfbs = factor.getCorrectionValue("PFBS");
    EXPECT_NEAR(pfbs.correctionFactor, 4.960112, 1e-4);
    EXPECT_EQ(pfbs.quantityISTD, 10);
}

TEST_F(TestCorrectionFactorParser, HappyDayParseEPA) {
    CorrectionFactorDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "CorrectionFactor" / "EPA.xlsx";

    CorrectionFactor factor;
    ParserMessages msgs = parseCorrectionFactor(desc, factor);
    printMessages(msgs);

    EXPECT_EQ(factor.getCorrectionValues().size(), 13);

    ASSERT_TRUE(factor.hasCorrectionValue("NaDONA"));
    const auto& nadona = factor.getCorrectionValue("NaDONA");
    EXPECT_NEAR(nadona.correctionFactor, 4.37186696602193, 1e-4);
    EXPECT_EQ(nadona.quantityISTD, 10);
}
