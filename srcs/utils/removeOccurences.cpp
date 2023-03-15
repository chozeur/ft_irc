#include "../../includes/utils.hpp"

void removeAllOccurrences(std::string& str, const std::string& substr) {
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        str.erase(pos, substr.length());
    }
}
