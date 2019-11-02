//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_AGREGATE_HPP
#define WEBSITE_METERING_AGREGATE_HPP

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include "curl_response.hpp"

namespace datadog::structs {
    struct aggregate {
        double availability;
        double average_response_time;
        double max_response_time;
        double min_response_time;
        std::unordered_map<uint64_t, uint64_t> codes_count;

        aggregate(std::vector<datadog::structs::curl_response>& responses)
        : min_response_time(std::numeric_limits<double>::max())
        , max_response_time(std::numeric_limits<double>::min()) {
            int available_ctr = 0;
            double responses_aggr = 0;
            int not_empty_res = 0;

            for (const auto& res : responses) {
                if (res.response_code.has_value()) {
                    ++codes_count[res.response_code.value()];
                }
                min_response_time = std::min(min_response_time, res.response_time.value_or(min_response_time));
                max_response_time = std::max(max_response_time, res.response_time.value_or(max_response_time));
                available_ctr += res.available;
                if (res.response_time.has_value()) {
                    responses_aggr += res.response_time.value();
                    ++not_empty_res;
                }
            }

            availability = (double) available_ctr / responses.size();
            average_response_time = responses_aggr / not_empty_res;
        }

        std::string to_str() {
            return "{ \"availability\": " + std::to_string(availability)  + ", "
            + "average_response_time" + std::to_string(average_response_time) + "}";
        }
    };
}

#endif //WEBSITE_METERING_AGREGATE_HPP
