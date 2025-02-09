#include "../include/Spectrotool/Compound.hpp"

#include <utility>

namespace Spectrotool {

    std::string CompoundValue::str() const {
        return "{" + name + " " + id + " " + std::to_string(rt) + " " + std::to_string(area) + " " + std::to_string(isArea) + " " + std::to_string(sDivN) + "}";
    }


    Compound::Compound(std::string name):
    m_Name(std::move(name)){

    }

    std::string Compound::str() const {
        std::string result = m_Name + ":\n";
        for (const auto& value: m_Values) {
            result += value.str() + "\n";
        }
        return result;
    }


}