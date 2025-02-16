#pragma once

#include "Defs.hpp"
#include <unordered_map>
#include <OpenXLSX.hpp>

namespace Spectrotool {

    struct SampleListFileDesc {
        fs::path filePath;
    };

    struct SampleInfo {
        std::string id;
        double weight; // in grams
        std::string matrix; // e.g. soil, water, etc.
    };

    class SampleListFile {
    public:
        explicit SampleListFile(const SampleListFileDesc& desc);

        ~SampleListFile() = default;

        const std::unordered_map<std::string, SampleInfo>& getSamples() const { return m_Samples; }

        const SampleInfo& operator[](const std::string& id) const { return m_Samples.at(id); }

    private:

        void loadSheet(const OpenXLSX::XLWorksheet& sheet);

    private:

        std::unordered_map<std::string, SampleInfo> m_Samples; // id -> sample info

    };



}