//
// Created by fiffeek on 02.11.19.
//

#ifndef WEBSITE_METERING_LOGGER_HPP
#define WEBSITE_METERING_LOGGER_HPP

#include <vector>
#include "../structs/timeframe.hpp"
#include "../const.hpp"
#include "../dependencies/Scheduler/Scheduler.h"
#include "database.hpp"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace datadog::database {
    class logger {
    private:
        void log(datadog::structs::aggregate& aggr,
                const std::string& website,
                const datadog::structs::timeframe& tf) {
            BOOST_LOG_TRIVIAL(info) << "Info for " << website << " for the last "
            << tf.interval << " seconds.\n" << aggr.to_str() << "\n";
        }
    public:
        explicit logger(
                const std::vector<datadog::structs::timeframes>& v,
                datadog::database::database* db)
                : timeframes(v)
                , scheduler(datadog::MAX_THREADS)
                , db(db) {
            init();
        }

        explicit logger(const logger &logger1)
                : timeframes(logger1.timeframes)
                , scheduler(datadog::MAX_THREADS)
                , db(logger1.db) {}

        void init() {
            for (auto& tf : timeframes) {
                std::cout << "wanna log " << tf.frequency.interval << std::endl;
                scheduler.every(std::chrono::seconds(tf.frequency.interval), [&, tf] () {
                    auto all = db->get_websites();
                    std::cout << "all website " << all.size() << std::endl;

                    for (auto& website : all) {
                        auto logs = db->get_stats(website, tf.aggregate);
                        log(logs, website, tf.aggregate);
                    }
                });
            }
        }

    private:
        std::vector<datadog::structs::timeframes> timeframes;
        Bosma::Scheduler scheduler;
        datadog::database::database* db;
    };
}

#endif //WEBSITE_METERING_LOGGER_HPP
