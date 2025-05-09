#include "../include/Spectrotool/Compound.hpp"

#include <stdexcept>
#include <utility>

namespace Spectrotool {

    std::string CompoundValue::str() const {
        return "{" + name + " " + id + " " + std::to_string(rt) + " " + std::to_string(area) + " " + std::to_string(isArea) + " " + std::to_string(sDivN) + "}";
    }

    nlohmann::json CompoundValue::toJson() const {
        nlohmann::json json;
        json["name"] = name;
        json["id"] = id;
        json["rt"] = rt;
        json["area"] = area;
        json["isArea"] = isArea;
        json["sDivN"] = sDivN;

        json["weight"] = weight;
        json["matrix"] = matrix;

        json["correctedISArea"] = correctedISArea;
        json["uncorrectedConcentration_pgg"] = uncorrectedConcentration_pgg;
        json["uncorrectedConcentration_microgkg"] = uncorrectedConcentration_microgkg;
        json["correctedConcentration"] = correctedConcentration;
        return json;
    }

    void CompoundValue::fromJson(const nlohmann::json &json) {
        name = json["name"].get<std::string>();
        id = json["id"].get<std::string>();
        rt = json["rt"].get<double>();
        area = json["area"].get<double>();
        isArea = json["isArea"].get<double>();
        sDivN = json["sDivN"].get<double>();

        weight = json["weight"].get<double>();
        matrix = json["matrix"].get<std::string>();

        correctedISArea = json["correctedISArea"].get<double>();
        uncorrectedConcentration_pgg = json["uncorrectedConcentration_pgg"].get<double>();
        uncorrectedConcentration_microgkg = json["uncorrectedConcentration_microgkg"].get<double>();
        correctedConcentration = json["correctedConcentration"].get<double>();
    }

    Compound::Compound(std::string name):
    m_Name(std::move(name)){

    }

    void Compound::addValue(CompoundValue &&value) {
        if (value.id != "ACN") {
            if (m_CompoundValueID.find(value.id) != m_CompoundValueID.end()) {
                throw std::runtime_error("Duplicate compound value ID: " + value.id + " for compound: " + m_Name);
            }
        }
        m_Values.push_back(value);
    }

    std::string Compound::str() const {
        std::string result = m_Name + ":\n";
        for (const auto& value: m_Values) {
            result += value.str() + "\n";
        }
        return result;
    }

    void Compound::setSampleInfo(const SampleList &list) {
        for (auto& value : m_Values) {
            if (list.hasSample(value.id)) {
                const auto& sampleInfo = list[value.id];
                value.weight = sampleInfo.weight;
                value.matrix = sampleInfo.matrix;
            }
        }
    }

    void Compound::correct(const CorrectionValue &correctionValue) {
        istdCorrection(correctionValue);
        concentrationCorrection(correctionValue);
    }

    nlohmann::json Compound::toJson() const {
        nlohmann::json json;
        json["name"] = m_Name;
        json["blankCorrection"] = m_BlankCorrection;
        nlohmann::json values;
        for (const auto& value: m_Values) {
            values.push_back(value.toJson());
        }
        json["values"] = values;
        return json;
    }

    void Compound::fromJson(const nlohmann::json &json) {
        m_Name = json["name"].get<std::string>();
        m_BlankCorrection = json["blankCorrection"].get<double>();
        for (const auto& valueJson: json["values"]) {
            m_Values.emplace_back();
            m_Values.back().fromJson(valueJson);
        }
    }

    void Compound::istdCorrection(const CorrectionValue &correctionValue) {
        for (CompoundValue& value: m_Values) {
            value.correctedISArea = value.isArea * correctionValue.correctionFactor;
        }
    }

    void Compound::concentrationCorrection(const CorrectionValue &correctionValue) {
        // 1. Uncorrected Concentration
        for (CompoundValue& value: m_Values) {
            const double area = value.area / value.correctedISArea;
            const double weight = (correctionValue.quantityISTD * 1000) / value.weight;
            value.uncorrectedConcentration_pgg = area * weight;
            value.uncorrectedConcentration_microgkg = value.uncorrectedConcentration_pgg * 1000;
        }

        // 2. Blank Calculation
        std::size_t BL_count = 0;
        double accValue = 0.0;
        for (CompoundValue& value: m_Values) {
            if (value.id.find("BL") != std::string::npos) {
                accValue += value.uncorrectedConcentration_microgkg;
                BL_count++;
            }
        }
        if (BL_count > 0) {
            m_BlankCorrection = accValue / static_cast<double>(BL_count);
        }

        // 3. Corrected Concentration
        for (CompoundValue& value: m_Values) {
            value.correctedConcentration = value.uncorrectedConcentration_microgkg - m_BlankCorrection;
        }
    }



}