#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

// if any network failure, this exception is thrown
class network_exception: public std::runtime_error {
public:
    network_exception(std::string const & message) : std::runtime_error(message) {}
};

// if any argument failure, this exception is thrown
class argument_exception: public std::invalid_argument {
public:
    argument_exception(std::string const & message) : std::invalid_argument(message) {}
};

namespace utils {
    /**
     * Split string to vector of strings
     * 
     * @param str string to be split
     * @param delimeter character used as delimeter when splitting
     * @return vector of strings
     */
    std::vector<std::string> string_to_vector(std::string str, char delimeter);
    /**
     * Join vector of strings to one string
     * 
     * @param list vector of strings
     * @return joined strings into one string
     */
    std::string vector_to_string(std::vector<std::string> list);
}

#endif // UTILS_H
