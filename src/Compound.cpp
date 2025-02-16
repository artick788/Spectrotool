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

    void Compound::setSampleInfo(const SampleListFile &sampleListFile) {
        for (auto& value : m_Values) {
            if (sampleListFile.hasSample(value.id)) {
                const auto sampleInfo = sampleListFile[value.id];
                value.weight = sampleInfo.weight;
                value.matrix = sampleInfo.matrix;
            }
        }
    }

    nlohmann::json Compound::toJson() const {
        nlohmann::json json;
        json["name"] = m_Name;
        nlohmann::json values;
        for (const auto& value: m_Values) {
            values.push_back(value.toJson());
        }
        json["values"] = values;
        return json;
    }

    void Compound::fromJson(const nlohmann::json &json) {
        m_Name = json["name"].get<std::string>();
        for (const auto& valueJson: json["values"]) {
            m_Values.emplace_back();
            m_Values.back().fromJson(valueJson);
        }
    }

}