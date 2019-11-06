//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_TIMEFRAME_HPP
#define WEBSITE_METERING_TIMEFRAME_HPP

#include <cstdint>
#include "aggregate.hpp"

namespace datadog::structs {
    struct timeframe {
        uint64_t interval;

        bool operator<(const timeframe &lhs) const {
            return lhs.interval < interval;
        }
    };

    struct timeframes {
        timeframe frequency;
        timeframe aggregate;

        timeframes(timeframe tf1, timeframe tf2)
                : frequency(tf1), aggregate(tf2) {}
    };

    struct timeframe_aggr {
        timeframe tf;
        std::vector<aggregate> aggr;
    };
}

#endif //WEBSITE_METERING_TIMEFRAME_HPP
