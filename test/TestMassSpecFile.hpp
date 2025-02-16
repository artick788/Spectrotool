#pragma once

#include <gtest/gtest.h>

#include "../include/Spectrotool/MassSpecFile.hpp"

using namespace Spectrotool;

class TestMassSpecFile : public ::testing::Test {
protected:
    void SetUp() override ;

    void TearDown() override;

};