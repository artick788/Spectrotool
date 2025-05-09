#pragma once

#include "Defs.hpp"
#include <unordered_map>
#include <OpenXLSX.hpp>

namespace Spectrotool {

    struct SampleInfo {
        std::string id;
        double weight; // in grams
        std::string matrix; // e.g. soil, water, etc.
    };

    class SampleList {
    public:
        SampleList() = default;

        ~SampleList() = default;

        const std::unordered_map<std::string, SampleInfo>& getSamples() const { return m_Samples; }

        SampleInfo& operator[](const std::string& id) { return m_Samples[id]; }

        const SampleInfo& operator[](const std::string& id) const { return m_Samples.at(id); }

        bool hasSample(const std::string& id) const { return m_Samples.find(id) != m_Samples.end(); }

    private:

        std::unordered_map<std::string, SampleInfo> m_Samples; // id -> sample info

    };



}