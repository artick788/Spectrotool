#include "Compound.hpp"

#include <utility>

namespace Spectrotool {

    Compound::Compound(std::string name):
    m_Name(std::move(name)){

    }

}