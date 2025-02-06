#include <iostream>
#include <OpenXLSX.hpp>
#include <filesystem>

namespace fs = std::filesystem;

void test_openxlsx(const fs::path& path) {
    OpenXLSX::XLDocument doc;
    doc.open(path.string());

    auto polarSheet = doc.workbook().worksheet("Polar");
    auto range = polarSheet.range();
    for (const auto& cell : range) {
        std::cout << cell.value() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    try {
        fs::path path = fs::current_path() / "TestResources" / "220115_PFASFORWARD_VMM.xlsx";
        test_openxlsx(path);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
