#include "../../includes/utils.hpp"

void removeAllOccurrences(std::string& str, const std::string& substr) {
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        str.erase(pos, substr.length());
    }
}

void cleanLine(std::string& str) {
    std::string::size_type start_pos = 0;
	std::string::size_type end_pos = str.find("\r\n", start_pos);
	str = str.substr(start_pos, end_pos - start_pos);
	removeAllOccurrences(str, "\n");
	removeAllOccurrences(str, "\r");
}
