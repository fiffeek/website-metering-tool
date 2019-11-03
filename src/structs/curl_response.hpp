//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_CURL_RESPONSE_HPP
#define WEBSITE_METERING_CURL_RESPONSE_HPP

#include <cstdint>
#include <optional>
#include <cpr/cpr.h>
#include <ctime>

namespace datadog::structs {
    struct curl_response {
        bool available;
        std::string website_name;
        std::optional<double> response_time;
        std::optional<uint64_t> response_code;
        uint64_t timestamp;

        curl_response(const cpr::Response& res)
            : available(cpr::status::is_success(res.status_code))
            , website_name(res.url)
            , response_time(res.elapsed)
            , response_code(res.status_code)
            , timestamp(std::time(0)) {}

        curl_response(uint64_t timestamp)
            : timestamp(timestamp) {}
    };
}

#endif //WEBSITE_METERING_CURL_RESPONSE_HPP
