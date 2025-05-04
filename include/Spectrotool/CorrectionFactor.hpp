#pragma once

#include "Defs.hpp"

#include "OpenXLSX.hpp"

#include <unordered_map>

namespace Spectrotool {

    struct CorrectionValue{
        std::string compoundName;
        std::string quantifiedWith; // Also a compound
        double correctionFactor = 0.0;
        double quantityISTD = 0.0;
    };

    class CorrectionFactor{
    public:
        explicit CorrectionFactor(const fs::path& filePath);

        ~CorrectionFactor() = default;

        const std::unordered_map<std::string, CorrectionValue>& getCorrectionValues() const { return m_CorrectionValues;}

        const CorrectionValue& getCorrectionValue(const std::string& name) const;

    private:
        void loadWorkSheet(const OpenXLSX::XLWorksheet& sheet);

    private:
        std::unordered_map<std::string, CorrectionValue> m_CorrectionValues;

    };

}