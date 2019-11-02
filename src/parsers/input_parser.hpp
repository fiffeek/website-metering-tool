//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_INPUT_PARSER_HPP
#define WEBSITE_METERING_INPUT_PARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "../structs/cron_website.hpp"
#include "../file.hpp"

namespace datadog::parsers {
    class input_parser {
    private:
        using websites = std::vector<datadog::structs::cron_website>;

        void parse_websites() {
            auto lines = file.read_lines();
            for (const auto& line : lines) {
                std::istringstream ss(line);
                std::string name;
                uint64_t seconds;

                if (!(ss >> name >> seconds)) {
                    throw std::runtime_error("Wrong input, every line must contain a name and a number of seconds");
                }

                parsed.emplace_back(datadog::structs::cron_website{name, seconds});
            }
        }

    public:
        explicit input_parser(std::string filepath) : file(boost::filesystem::path{filepath}) {
            parse_websites();
        }

        websites get_websites() {
            return parsed;
        }

    private:
        datadog::common::file file;
        websites parsed;
    };
}

#endif //WEBSITE_METERING_INPUT_PARSER_HPP
