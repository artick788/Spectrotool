#include "utils.hpp"

#include <Spectrotool/Defs.hpp>

namespace Spectrotool {

    void sanitizeString(std::string& str) {
        const std::string invalid_characters = "\\/:; ";

        for (const char& c : invalid_characters) {
            for (char& original_char: str) {
                if (original_char == c) {
                    original_char = ST_REPLACEMENT_CHAR;
                }
            }
        }
    }
}
