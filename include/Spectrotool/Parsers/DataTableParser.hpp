#pragma once

#include "ParserMessages.hpp"
#include "../DataTable.hpp"

namespace Spectrotool {

    struct DataTableDesc {
        fs::path filePath = fs::current_path();
        std::vector<std::string> excludeCompoundFilter;
    };

    ParserMessages parseDataTable(const DataTableDesc& desc, DataTable& table);

    void exportDataTable(const fs::path& filePath);

}
