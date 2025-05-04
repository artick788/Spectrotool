#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "CorrectionFactor.hpp"
#include "SampleListFile.hpp"
#include "json.hpp"

namespace Spectrotool {

    struct CompoundValue {
        std::string name;
        std::string id;
        double rt = 0.0;
        double area = 0.0;
        double isArea = 0.0;
        double sDivN = 0.0;

        // data from SampleListFile
        double weight = 0.0; // Grams
        std::string matrix;

        // correction factors
        double correctedISArea = 0.0;
        double uncorrectedConcentration_pgg = 0.0; // pg/g
        double uncorrectedConcentration_microgkg = 0.0; //µg/kg
        double correctedConcentration = 0.0;

        std::string str() const;

        nlohmann::json toJson() const;

        void fromJson(const nlohmann::json& json);
    };

    class Compound {
    public:
        explicit Compound(std::string name);

        ~Compound() = default;

        [[nodiscard]] const std::string& getName() const { return m_Name;}

        void addValue(CompoundValue&& value);

        [[nodiscard]] const std::vector<CompoundValue>& getValues() const { return m_Values; }

        std::string str() const;

        void setSampleInfo(const SampleListFile& sampleListFile);

        void correct(const CorrectionValue& correctionValue);

        nlohmann::json toJson() const;

        void fromJson(const nlohmann::json& json);

    private:

        void istdCorrection(const CorrectionValue& correctionValue);

        void concentrationCorrection(const CorrectionValue& correctionValue);

    private:
        std::string m_Name;
        std::vector<CompoundValue> m_Values;
        std::unordered_set<std::string> m_CompoundValueID;

        // Correction Data (only valid when Correct() was called
        double m_BlankCorrection = 0.0;
    };
}
