//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_CONST_HPP
#define WEBSITE_METERING_CONST_HPP

namespace datadog {
    constexpr int ERROR = 1;
    constexpr int SUCCESS = 0;
    constexpr unsigned int MAX_THREADS = 12;
    constexpr unsigned int AGRR_INTERVAL_2M = 2 * 60;
    constexpr unsigned int AGGR_INTERVAL_10M = 10 * 60;
    constexpr unsigned int STATS_DISPLAY_10S = 10;
    constexpr unsigned int STATS_DISPLAY_1M = 1 * 60;
    constexpr unsigned int STATS_DISPLAY_10M = 10 * 60;
    constexpr unsigned int STATS_DISPLAY_1H = 60 * 60;
    constexpr unsigned int REPORT_OFFSET = 3;
    constexpr double AVAILABILITY_LIMIT = 0.8;
}

#endif //WEBSITE_METERING_CONST_HPP
