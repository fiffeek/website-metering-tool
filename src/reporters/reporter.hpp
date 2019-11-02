//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_REPORTER_HPP
#define WEBSITE_METERING_REPORTER_HPP

#include "../structs/timeframe.hpp"
#include "../dependencies/Scheduler/Scheduler.h"
#include "../const.hpp"
#include <vector>

namespace datadog::reporter {
    class reporter {
    public:
        reporter(const std::vector<datadog::structs::timeframe>& v)
            : timeframes(v), scheduler(datadog::MAX_THREADS) {
            init();
        }

        void init() {
            for (auto& tf : timeframes) {
                scheduler.every(std::chrono::seconds(tf.interval), [&] () {
                    // call from db
                });
            }
        }

    private:
        std::vector<datadog::structs::timeframe> timeframes;
        Bosma::Scheduler scheduler;
    };
}

#endif //WEBSITE_METERING_REPORTER_HPP
