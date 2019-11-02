//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_AGREGATE_HPP
#define WEBSITE_METERING_AGREGATE_HPP

#include <cstdint>
#include <unordered_map>
#include <vector>
#include "curl_response.hpp"

namespace datadog::structs {
    struct aggregate {
        double availability;
        uint64_t average_response_time;
        uint64_t max_response_time;
        uint64_t min_response_time;
        std::unordered_map<uint64_t, uint64_t> codes_count;

        aggregate(std::vector<datadog::structs::curl_response>& responses)
            : availability(1.0), max_response_time(0), min_response_time(UINT64_MAX)  {
            // TODO for and map here
        }
    };
}

#endif //WEBSITE_METERING_AGREGATE_HPP
