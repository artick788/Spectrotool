#include <gtest/gtest.h>

#include "../include/Spectrotool/CorrectionFactor.hpp"

using namespace Spectrotool;

class TestCorrectionFactor : public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }

};

TEST_F(TestCorrectionFactor, HappyDayParse) {
    CorrectionFactorDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "Correction_factors_TG.xlsx";

    CorrectionFactor factor(desc);
    const auto& factors = factor.getCorrectionValues();
    ASSERT_EQ(factors.size(), 15);

    const CorrectionValue v1 = factor.getCorrectionValue("PFBA");
    EXPECT_EQ(v1.compoundName, "PFBA");
    EXPECT_EQ(v1.quantifiedWith, "mPFBA");
    EXPECT_EQ(v1.correctionFactor, 0.780715);
    EXPECT_EQ(v1.quantityISTD, 10);

    const CorrectionValue v2 = factor.getCorrectionValue("PFTeA");
    EXPECT_EQ(v2.compoundName, "PFTeA");
    EXPECT_EQ(v2.quantifiedWith, "mPFDoA");
    EXPECT_EQ(v2.correctionFactor, 0.457183);
    EXPECT_EQ(v2.quantityISTD, 10);
}
