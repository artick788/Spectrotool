#pragma once

#include "Defs.hpp"
#include "Compound.hpp"
#include <OpenXLSX.hpp>

namespace Spectrotool{

    struct WorksheetDesc{
        std::string polarSheetName = "Polar";
        std::string neutralSheetName = "Neutral";
        std::string apolarSheetName = "Apolar";
    };

    class Worksheet{
    public:
        explicit Worksheet(const fs::path& path, const WorksheetDesc& desc = WorksheetDesc());

        ~Worksheet() = default;

    private:
        static void loadWorkSheet(const OpenXLSX::XLWorksheet& sheet, std::vector<Compound>& compounds);

        static std::string formatCompoundName(const std::string& name);

    private:
        std::vector<Compound> m_PolarCompounds;
        std::vector<Compound> m_NeutralCompounds;
        std::vector<Compound> m_ApolarCompounds;
    };

}