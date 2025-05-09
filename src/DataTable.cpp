#include "../include/Spectrotool/DataTable.hpp"
#include <SyriusUtils/SyriusUtils.hpp>

namespace Spectrotool {

    std::size_t DataTable::getSampleCount() const {
        std::size_t samples = 0;
        for (const auto& compound: m_Compounds) {
            samples += compound.getValues().size();
        }
        return samples;
    }


    bool DataTable::hasCompound(const std::string &name) const {
        for (const auto& compound : m_Compounds) {
            if (compound.getName() == name) {
                return true;
            }
        }
        return false;
    }

    void DataTable::addCompound(Compound &&compound) {
        if (hasCompound(compound.getName())) {
            SR_LOG_WARNING("DataTable", "Duplicate compound name {}", compound.getName());
        }
        m_Compounds.push_back(compound);
    }

    const Compound &DataTable::getCompound(std::string_view name) const {
        for (const auto& compound : m_Compounds) {
            if (compound.getName() == name) {
                return compound;
            }
        }
        SR_LOG_THROW("DataTable", "No compound with name {} found", name);
    }

    nlohmann::json DataTable::toJson() const {
        nlohmann::json json;
        json["samplesAdded"] = m_SamplesAdded;
        json["correctionFactorsAdded"] = m_CorrectionFactorsAdded;
        json["compounds"] = nlohmann::json::array();
        for (const auto& compound: m_Compounds) {
            json["compounds"].push_back(compound.toJson());
        }
        return json;
    }

    void DataTable::fromJson(const nlohmann::json &json) {
        m_SamplesAdded = json.at("samplesAdded");
        m_CorrectionFactorsAdded = json.at("correctionFactorsAdded");
        for (const auto& compoundJson: json.at("compounds")) {
            m_Compounds.emplace_back();
            m_Compounds.back().fromJson(compoundJson);
        }
    }

    void DataTable::setSampleList(const SampleList &list) {
        m_SamplesAdded = true;
        for (Compound& compound: m_Compounds) {
            compound.setSampleInfo(list);
        }
    }

}
