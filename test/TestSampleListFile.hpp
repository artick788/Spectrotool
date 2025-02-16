#pragma once

#include <gtest/gtest.h>

#include "../include/Spectrotool/SampleListFile.hpp"

using namespace Spectrotool;

class TestSampleListFile : public ::testing::Test {
protected:
    void SetUp() override ;

    void TearDown() override;

};