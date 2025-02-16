#include "../include/Spectrotool/Compound.hpp"

#include <stdexcept>
#include <utility>

namespace Spectrotool {

    std::string CompoundValue::str() const {
        return "{" + name + " " + id + " " + std::to_string(rt) + " " + std::to_string(area) + " " + std::to_string(isArea) + " " + std::to_string(sDivN) + "}";
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



}