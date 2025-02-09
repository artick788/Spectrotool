#pragma once

#include <gtest/gtest.h>

#include "../include/Spectrotool/Worksheet.hpp"

using namespace Spectrotool;

class TestWorkSheet : public ::testing::Test {
protected:
    void SetUp() override ;

    void TearDown() override;

protected:
    fs::path m_HappyDayFile;
};