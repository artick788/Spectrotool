#pragma once

#include <filesystem>

typedef enum ST_COMPOUND_TYPE {
    ST_COMPOUND_TYPE_NONE = 0x00,
    ST_COMPOUND_TYPE_POLAR = 0x01,
    ST_COMPOUND_TYPE_NEUTRAL = 0x02,
    ST_COMPOUND_TYPE_APOLAR = 0x04
} ST_COMPOUND_TYPE;

namespace Spectrotool {

    namespace fs = std::filesystem;
    static std::string ST_REPLACEMENT_CHAR = "_";

}