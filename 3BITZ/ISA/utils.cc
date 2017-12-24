#include <iostream>
#include <sstream>
#include <algorithm>

#include <cctype>

#include "utils.h"

namespace utils {
    std::vector<std::string> string_to_vector(std::string str, char delimeter) {
        std::vector<std::string> strings;
        if (str.empty()) {
            return strings;
        }

        std::stringstream list_str{str};
        std::string s;

        // split string
        while (std::getline(list_str, s, delimeter)) {
            strings.push_back(s);
        }

        return strings;
    }
    std::string vector_to_string(std::vector<std::string> list) {
        if (list.empty()) {
            return "";
        }
        std::string str = list[0];
        // join strings
        for (unsigned i = 1; i < list.size(); ++i) {
            str += "," + list[i];
        }

        return str;
    }
}