#pragma once

#include "Defs.hpp"
#include "Compound.hpp"

namespace Spectrotool {

    class DataTable {
    public:
        DataTable() = default;

        ~DataTable() = default;

        std::vector<Compound>& getCompounds() { return m_Compounds; }

        std::size_t getSampleCount() const;

        bool hasCompound(const std::string& name) const;

        void addCompound(Compound&& compound);

        const Compound& getCompound(std::string_view name) const;

        nlohmann::json toJson() const;

        void fromJson(const nlohmann::json& json);

        auto begin(){ return m_Compounds.begin(); }

        auto end() { return m_Compounds.end(); }

    private:
        std::vector<Compound> m_Compounds;
    };
}