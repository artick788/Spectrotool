#include <iostream>
#include "src/Worksheet.hpp"

using namespace Spectrotool;

void test_openxlsx(const fs::path& path) {
    WorksheetDesc desc;
    Worksheet worksheet(path, desc);
    int x = 0;
}

int main(int argc, char *argv[]) {
    try {
        fs::path path = fs::current_path() / "TestResources" / "220115_PFASFORWARD_VMM.xlsx";
        test_openxlsx(path);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
