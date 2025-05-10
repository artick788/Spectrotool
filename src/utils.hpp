#pragma once

#include <XLCell.hpp>
#include <XLSheet.hpp>

#include "Spectrotool/json.hpp"


namespace OpenXLSX {
    class XLWorksheet;
}

namespace Spectrotool {

    void sanitizeString(std::string& str);

    template<typename T>
    T getXLValue(const OpenXLSX::XLCell& cell) {
        static_assert("Invalid type given to getXLValue");
    }

    template<>
    inline std::string getXLValue(const OpenXLSX::XLCell& cell) {
        std::string value =  cell.value().getString();
        sanitizeString(value);
        return value;
    }

    template<>
    inline double getXLValue(const OpenXLSX::XLCell& cell) {
        const std::string value = cell.value().getString();
        if (value.empty()) {
            return NAN;
        }
        return std::stod(value);
    }

    inline void setXLValue(OpenXLSX::XLWorksheet& sheet, const std::string& cellRef, const std::string& str) {
        sheet.cell(cellRef).value() = str;
    }

    inline void setXLValue(OpenXLSX::XLWorksheet& sheet, const std::string& cellRef, double dVal) {
        if (std::isnan(dVal)) {
            return; // leave cell empty
        }
        sheet.cell(cellRef).value() = dVal;
    }

    inline void setJsonValue(nlohmann::json& json, const std::string& key, double value) {
        if (std::isnan(value)) {
            json[key] = "NaN";
        }
        else {
            json[key] = value;
        }
    }
    inline void setJsonValue(nlohmann::json& json, const std::string& key, const std::string& value) {
        json[key] = value;
    }

    template<typename T>
    inline T getJsonValue(const nlohmann::json& json, const std::string& key) {
        static_assert("Invalid type given to getXLValue");
    }

    template<>
    inline double getJsonValue(const nlohmann::json& json, const std::string& key) {
        if (json.at(key) == "NaN") {
            return NAN;
        }
        return json.at(key).get<double>();
    }

    template<>
    inline std::string getJsonValue(const nlohmann::json& json, const std::string& key) {
        return json.at(key);
    }




}
