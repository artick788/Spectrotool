#pragma once

#include "Defs.hpp"
#include "Compound.hpp"
#include <OpenXLSX.hpp>
#include <unordered_set>

namespace Spectrotool{

    struct WorksheetDesc{
        std::vector<std::string> sheetNames = {"Polar", "Neutral", "Apolar"};
    };

    class MassSpecFile{
    public:
        explicit MassSpecFile(const fs::path& path, const WorksheetDesc& desc = WorksheetDesc());

        ~MassSpecFile() = default;

        const std::vector<Compound>& getCompounds() const { return m_Compounds; }

        bool hasCompound(const std::string& name) const;

        std::vector<Compound*> getCompound(const std::string& name);

    private:
        void loadWorkSheet(const OpenXLSX::XLWorksheet& sheet);

        static std::string formatCompoundName(const std::string& name);

        static void addCompound(Compound& compound, const OpenXLSX::XLCell& row);

    private:
        std::vector<Compound> m_Compounds;
        std::unordered_set<std::string> m_CompoundNames;
    };

}
