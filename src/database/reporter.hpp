//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_REPORTER_HPP
#define WEBSITE_METERING_REPORTER_HPP

#include "../structs/timeframe.hpp"
#include "../dependencies/Scheduler/Scheduler.h"
#include "../const.hpp"
#include <vector>
#include "aggregate_builder.hpp"
#include "logger.hpp"
#include <unordered_set>

namespace datadog::database {
    class alerting_reporter {
    public:
        alerting_reporter(
                const datadog::structs::timeframe& aggr_timeframe,
                datadog::database::database* db)
            : timeframe(datadog::structs::timeframe{aggr_timeframe.interval + datadog::REPORT_OFFSET})
            , scheduler(datadog::MAX_THREADS)
            , db(db)
            , aggr_timeframe(aggr_timeframe) {
            init();
        }

        alerting_reporter() = default;

        void init() {
            scheduler.every(std::chrono::seconds(timeframe.interval), [&] () {
                for (auto& website : db->get_websites()) {
                    auto rep = db->get_last(website, aggr_timeframe);

                    if (rep.has_value()) {
                        if (rep.value().availability < datadog::AVAILABILITY_LIMIT && down.count(website) == 0) {
                            logger.alert(website, rep.value());
                            down.insert(website);
                        } else if (rep.value().availability >= datadog::AVAILABILITY_LIMIT && down.count(website) == 1) {
                            logger.alert(website, rep.value(), false);
                            down.erase(website);
                        }
                    }
                }
            });
        }

    private:
        datadog::structs::timeframe timeframe;
        Bosma::Scheduler scheduler;
        datadog::database::database* db;
        datadog::database::logger logger;
        datadog::structs::timeframe aggr_timeframe;
        std::unordered_set<std::string> down;
    };
}

#endif //WEBSITE_METERING_REPORTER_HPP
