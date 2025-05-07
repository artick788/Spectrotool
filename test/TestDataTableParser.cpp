#include <gtest/gtest.h>

#include "../include/Spectrotool/Parsers/DataTableParser.hpp"

using namespace Spectrotool;

class TestDataTableParser : public ::testing::Test {
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

TEST_F(TestDataTableParser, HappyDayParse) {
    DataTableDesc desc;
    desc.filePath = fs::current_path() / "TestResources" / "HappyDay" / "220115_PFASFORWARD_VMM.xlsx";

    DataTable table;
    ParserMessages msgs = parseDataTable(desc, table);
    printMessages(msgs);

    EXPECT_EQ(table.getCompounds().size(), 71);
    EXPECT_EQ(table.getSampleCount(), 6382);
}

