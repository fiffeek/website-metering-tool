//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_CURL_RESPONSE_HPP
#define WEBSITE_METERING_CURL_RESPONSE_HPP

#include <cstdint>
#include <optional>

namespace datadog::structs {
    struct curl_response {
        bool available;
        std::string website_name;
        std::optional<uint64_t> response_time;
        std::optional<uint64_t> response_code;
        uint64_t timestamp;
        // TODO more metrics
    };
}

#endif //WEBSITE_METERING_CURL_RESPONSE_HPP
