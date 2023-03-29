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

std::string remove_html_header(const std::string& html_string) {
    // Find the end of the header by searching for the "\r\n\r\n" sequence
    std::size_t header_end = html_string.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        // If the header end is not found, return the original string
        return html_string;
    } else {
        // Return the substring after the header end
        return html_string.substr(header_end + 4);
    }
}
