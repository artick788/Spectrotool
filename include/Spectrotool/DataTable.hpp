#pragma once

#include "Defs.hpp"
#include "Compound.hpp"
#include "SampleList.hpp"
#include "CorrectionFactor.hpp"

namespace Spectrotool {

    class DataTable {
    public:
        DataTable() = default;

        ~DataTable() = default;

        std::vector<Compound>& getCompounds() { return m_Compounds; }

        const std::vector<Compound>& getCompounds() const { return m_Compounds; }

        std::size_t getSampleCount() const;

        bool hasCompound(const std::string& name) const;

        void addCompound(Compound&& compound);

        const Compound& getCompound(std::string_view name) const;

        nlohmann::json toJson() const;

        void fromJson(const nlohmann::json& json);

        void setSampleList(const SampleList& list);

        std::vector<std::string> setCorrectionFactor(const CorrectionFactor& factor);

        auto begin(){ return m_Compounds.begin(); }
        auto end() { return m_Compounds.end(); }

        bool hasSamplesAdded() const { return m_SamplesAdded; }
        bool hasCorrectionFactorsAdded() const { return m_CorrectionFactorsAdded; }

    private:
        std::vector<Compound> m_Compounds;
        bool m_SamplesAdded = false;
        bool m_CorrectionFactorsAdded = false;
    };
}