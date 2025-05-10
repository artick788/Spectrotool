#include "../include/Spectrotool/Compound.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <utility>

namespace Spectrotool {

    std::string CompoundValue::str() const {
        return "{" + name + " " + id + " " + std::to_string(rt) + " " + std::to_string(area) + " " + std::to_string(isArea) + " " + std::to_string(sDivN) + "}";
    }

    nlohmann::json CompoundValue::toJson() const {
        nlohmann::json json;
        setJsonValue(json, "name", name);
        setJsonValue(json, "name", name);
        setJsonValue(json, "id", id);
        setJsonValue(json, "rt", rt);
        setJsonValue(json, "area", area);
        setJsonValue(json, "isArea", isArea);
        setJsonValue(json, "sDivN", sDivN);

        setJsonValue(json, "weight", weight);
        setJsonValue(json, "matrix", matrix);

        setJsonValue(json, "correctedISArea", correctedISArea);
        setJsonValue(json, "uncorrectedConcentration_pgg", uncorrectedConcentration_pgg);
        setJsonValue(json, "uncorrectedConcentration_microgkg", uncorrectedConcentration_microgkg);
        setJsonValue(json, "correctedConcentration", correctedConcentration);
        return json;
    }

    void CompoundValue::fromJson(const nlohmann::json &json) {
        name = getJsonValue<std::string>(json, "name");
        id = getJsonValue<std::string>(json, "id");
        rt = getJsonValue<double>(json, "rt");
        area = getJsonValue<double>(json, "area");
        isArea = getJsonValue<double>(json, "isArea");
        sDivN = getJsonValue<double>(json, "sDivN");

        weight = getJsonValue<double>(json, "weight");
        matrix = getJsonValue<std::string>(json, "matrix");

        correctedISArea = getJsonValue<double>(json, "correctedISArea");
        uncorrectedConcentration_pgg = getJsonValue<double>(json, "uncorrectedConcentration_pgg");
        uncorrectedConcentration_microgkg = getJsonValue<double>(json, "uncorrectedConcentration_microgkg");
        correctedConcentration = getJsonValue<double>(json, "correctedConcentration");
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
        setJsonValue(json, "name", m_Name);
        setJsonValue(json, "blankCorrection", m_BlankCorrection);
        nlohmann::json values;
        for (const auto& value: m_Values) {
            values.push_back(value.toJson());
        }
        json["values"] = values;
        return json;
    }

    void Compound::fromJson(const nlohmann::json &json) {
        m_Name = getJsonValue<std::string>(json, "name");
        m_BlankCorrection = getJsonValue<double>(json, "blankCorrection");
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
            value.uncorrectedConcentration_microgkg = value.uncorrectedConcentration_pgg / 1000;
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