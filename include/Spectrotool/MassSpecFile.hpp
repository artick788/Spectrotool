#pragma once

#include "Defs.hpp"
#include "Compound.hpp"
#include <OpenXLSX.hpp>
#include <unordered_set>

namespace Spectrotool{

    struct MassSpecFileDesc{
        fs::path filePath;
        std::string excludeFilter;
        std::vector<std::string> sheetNames = {"Polar", "Neutral", "Apolar"};
    };

    class MassSpecFile{
    public:
        MassSpecFile() = default;

        explicit MassSpecFile(const MassSpecFileDesc& desc);

        ~MassSpecFile() = default;

        const std::vector<Compound>& getCompounds() const { return m_Compounds; }

        std::size_t getReadCompoundCount() const { return m_ReadCompoundCount; }

        std::size_t getSampleCount() const { return m_SampleCount; }

        bool hasCompound(const std::string& name) const;

        std::vector<Compound*> getCompound(const std::string& name);

        void setSampleInfo(const SampleListFile& sampleListFile);

    private:
        void loadWorkSheet(const OpenXLSX::XLWorksheet& sheet, const MassSpecFileDesc& desc);

        static bool filterCompound(const std::string& name, const std::string& filter);

        static std::string formatCompoundName(const std::string& name);

    private:
        std::size_t m_ReadCompoundCount = 0;
        std::size_t m_SampleCount = 0;
        std::vector<Compound> m_Compounds;
        std::unordered_set<std::string> m_CompoundNames;
    };

}
