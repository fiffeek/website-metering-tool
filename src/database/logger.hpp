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

namespace datadog::database {
    class logger {
    private:
        void log_std(datadog::structs::aggregate& aggr,
                    const std::string& website,
                    const datadog::structs::timeframe& tf) {
            std::cout << "Info: website=[" << website << "] for the last seconds=["
            << tf.interval << "]. " << aggr.to_str() << ". ";
        }

        void log_single_timeframe_aggr(datadog::structs::timeframe_aggr& tf_aggr) {
            if (tf_aggr.aggr.empty()) {
                return;
            }

            std::string all_aggr;

            for (auto& single_aggr : tf_aggr.aggr) {
                all_aggr += " timestamp=[" + std::to_string(single_aggr.timestamp) + "] " + single_aggr.to_str() + " ";
            }

            std::cout << "Last [" << tf_aggr.aggr.size() << "] aggregates over seconds=[" << tf_aggr.tf.interval
            << "] interval" << all_aggr << " ";
        }

        void log(datadog::structs::aggregate& aggr,
                 const std::string& website,
                 const datadog::structs::timeframe& tf,
                 std::vector<datadog::structs::timeframe_aggr>& aggregates) {
            std::scoped_lock lock(mtx);

            log_std(aggr, website, tf);
            bool all_empty = true;

            for (auto& interval_aggr : aggregates) {
                if (!interval_aggr.aggr.empty()) {
                    all_empty = false;
                }
            }

            if (all_empty) {
                std::cout << "No aggregates. " << std::endl;
                return;
            }

            std::cout << "Aggregates: ";
            for (auto& interval_aggr : aggregates) {
                log_single_timeframe_aggr(interval_aggr);
            }
            std::cout << std::endl;
        }

        void logging_enabled(const datadog::structs::timeframes& tf) {
            std::scoped_lock lock(mtx);
            std::cout << "Logging from db will be done in seconds=[" << tf.frequency.interval << "] interval." <<
            std::endl;
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

        logger() = default;

        explicit logger(const logger &logger1)
                : timeframes(logger1.timeframes)
                , scheduler(datadog::MAX_THREADS)
                , db(logger1.db) {}

        void init() {
            for (auto& tf : timeframes) {
                logging_enabled(tf);
                scheduler.every(std::chrono::seconds(tf.frequency.interval), [&, tf] () {
                    auto all = db->get_websites();

                    for (auto& website : all) {
                        auto logs = db->get_stats(website, tf.aggregate);
                        auto aggr_logs = db->get_stats_aggr(website, tf.aggregate);

                        log(logs, website, tf.aggregate, aggr_logs);
                    }
                });
            }
        }

        void alert(std::string website, datadog::structs::aggregate& aggr, bool down = true) {
            std::scoped_lock lock(mtx);
            std::string is_down = down ? "down" : "up";

            std::cout << "Website [" << website << "] is " << is_down << ". availability=[" << aggr.availability <<
            "] ,time=[" << std::time(nullptr) << "]" << std::endl;
        }

    private:
        std::vector<datadog::structs::timeframes> timeframes;
        Bosma::Scheduler scheduler;
        datadog::database::database* db;
        std::mutex mtx;
    };
}

#endif //WEBSITE_METERING_LOGGER_HPP
