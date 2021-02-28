#pragma once

// Reads a config file containing lines with format "KEY VALUE".
// KEY and VALUE can be separated by any manner of whitespace.
// Only first two whitespace-separated tokens are read on each line.
// If '#' is anywhere in line, the line is ignored as a comment.
// In the case of repeated keys, the last value is used.

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

class Config
{
public:
    Config(const std::string& file_path);

    template <typename ValueType = std::string>
    std::optional<ValueType> get(const std::string& key) const;

    template <typename ValueType>
    ValueType get(const std::string& key, const ValueType& default_value) const;

    bool has(const std::string& key) const;

private:
    using Variant = std::variant<std::string, bool, size_t, long, double>;
    std::unordered_map<std::string, Variant> m_dictionary;

};

inline Config::Config(const std::string& file_path)
{
    std::ifstream input_stream(file_path);
    std::string line;

    while (std::getline(input_stream, line)) {
        if (line.find('#') != std::string::npos) {  // entire line is comment.
            continue;
        }
        std::stringstream line_stream(line);

        std::string key;
        line_stream >> key;
        if (key.empty()) {
            continue;
        }

        std::string value;
        line_stream >> value;
        if (value.empty()) {
            continue;
        }

        // check boolean.
        auto lower {value};
        std::transform(std::begin(value), std::end(value), std::begin(lower), ::tolower);
        if (lower == "true") {
            m_dictionary[key] = true;
            continue;
        } else if (lower == "false") {
            m_dictionary[key] = false;
            continue;
        }

        // check positive integer.
        if (std::all_of(std::cbegin(value), std::cend(value), ::isdigit)) {
            m_dictionary[key] = std::stoul(value);
            continue;
        }

        // check signed integer.
        const char* const parse_start = value.data();
        char* observed_parse_end {nullptr};
        constexpr int base {10};
        const auto parsed_long = std::strtol(parse_start, &observed_parse_end, base);
        const char* const full_parse_end = parse_start + value.length();
        if (observed_parse_end == full_parse_end) {
            m_dictionary[key] = parsed_long;
            continue;
        }

        // check double.
        const auto parsed_double = std::strtod(parse_start, &observed_parse_end);
        if (observed_parse_end == full_parse_end) {
            m_dictionary[key] = parsed_double;
            continue;
        }

        m_dictionary[key] = value;
    }
}

template <typename ValueType>
std::optional<ValueType> Config::get(const std::string& key) const {
    if (has(key)) {
        return std::get<ValueType>(m_dictionary.at(key));
    }
    return std::nullopt;
}

template <typename ValueType>
ValueType Config::get(const std::string& key, const ValueType& default_value) const {
    if (has(key)) {
        return *get<ValueType>(key);
    }
    return default_value;
}

inline bool Config::has(const std::string& key) const {
    return m_dictionary.find(key) != std::cend(m_dictionary);
}

