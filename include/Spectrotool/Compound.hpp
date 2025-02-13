#pragma once

#include <string>
#include <vector>
#include <unordered_set>

namespace Spectrotool {

    struct CompoundValue {
        std::string name;
        std::string id;
        double rt = 0.0;
        double area = 0.0;
        double isArea = 0.0;
        double sDivN = 0.0;

        std::string str() const;
    };

    class Compound {
    public:
        explicit Compound(std::string name);

        ~Compound() = default;

        [[nodiscard]] const std::string& getName() const { return m_Name;}

        void addValue(CompoundValue&& value);

        [[nodiscard]] const std::vector<CompoundValue>& getValues() const { return m_Values; }

        std::string str() const;

    private:
        std::string m_Name;
        std::vector<CompoundValue> m_Values;
        std::unordered_set<std::string> m_CompoundValueID;
    };
}
