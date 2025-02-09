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

        const std::vector<Compound>& getPolarCompounds() const { return m_PolarCompounds; }
        const std::vector<Compound>& getNeutralCompounds() const { return m_NeutralCompounds; }
        const std::vector<Compound>& getApolarCompounds() const { return m_ApolarCompounds; }

        bool hasCompound(const std::string& name, ST_COMPOUND_TYPE type) const;

        std::vector<Compound*> getCompound(const std::string& name, ST_COMPOUND_TYPE type);

    private:
        static void loadWorkSheet(const OpenXLSX::XLWorksheet& sheet, std::vector<Compound>& compounds);

        static std::string formatCompoundName(const std::string& name);

        static void addCompound(Compound& compound, const OpenXLSX::XLCell& row);

        static bool searchInCompound(const std::string& name, const std::vector<Compound>& compounds);

        static std::vector<Compound*> getCompound(const std::string& name, std::vector<Compound>& compounds);

    private:
        std::vector<Compound> m_PolarCompounds;
        std::vector<Compound> m_NeutralCompounds;
        std::vector<Compound> m_ApolarCompounds;
    };

}