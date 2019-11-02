//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_TIME_HELPER_HPP
#define WEBSITE_METERING_TIME_HELPER_HPP

#include <cstdint>
#include "../structs/curl_response.hpp"

namespace datadog::helpers {
    bool diff_exceeds(uint64_t t1, uint64_t t2, uint64_t frame) {
        if (t2 > t1) {
            return (t2 - t1) >= frame;
        }

        return diff_exceeds(t2, t1, frame);
    }

    bool diff_exceeds(
            const datadog::structs::curl_response& cr1,
            const datadog::structs::curl_response& cr2,
            uint64_t frame) {
        return diff_exceeds(cr1.timestamp, cr2.timestamp, frame);
    }
}

#endif //WEBSITE_METERING_TIME_HELPER_HPP
