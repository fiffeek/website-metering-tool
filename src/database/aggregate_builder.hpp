//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_AGGREGATE_BUILDER_HPP
#define WEBSITE_METERING_AGGREGATE_BUILDER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "../structs/curl_response.hpp"
#include "../structs/timeframe.hpp"
#include "../structs/aggregate.hpp"
#include "../dependencies/Scheduler/Scheduler.h"
#include "../const.hpp"

namespace datadog::database {
    class aggregate_builder {
    public:
        explicit aggregate_builder(const datadog::structs::timeframe &tf)
                : tf(tf), scheduler(datadog::MAX_THREADS) {}

        explicit aggregate_builder(const aggregate_builder &aggr)
                : tf(aggr.tf), scheduler(datadog::MAX_THREADS) {
            run();
        }

        void run() {
            scheduler.every(std::chrono::seconds(tf.interval), [&]() {
                std::scoped_lock lock(mtx, aggregates_lock);

                for (auto &elem : db) {
                    aggregates[elem.first].emplace_back(datadog::structs::aggregate{elem.second});
                    elem.second.clear();
                }
            });
        }

        void add(const datadog::structs::curl_response &response) {
            std::scoped_lock lock(mtx);
            db[response.website_name].push_back(response);
        }

        std::vector<datadog::structs::aggregate> get_stats(
                const std::string& website_name,
                const datadog::structs::timeframe& tf) {
            std::scoped_lock lock(aggregates_lock);
            auto& db_web = aggregates[website_name];
            auto it = std::lower_bound(db_web.begin(), db_web.end(), std::time(nullptr) - tf.interval,
                    [] (const datadog::structs::aggregate& lhs, const datadog::structs::aggregate& rhs) {
                                           return lhs.timestamp < rhs.timestamp;
            });

            std::vector<datadog::structs::aggregate> cut(it, db_web.end());
            return cut;
        }

        datadog::structs::timeframe get_timeframe() {
            return tf;
        }

        std::optional<datadog::structs::aggregate> get_last(std::string website) {
            if (aggregates[website].empty()) {
                return std::nullopt;
            }

            return std::optional<datadog::structs::aggregate>(aggregates[website][aggregates[website].size() - 1]);
        }

        bool has_interval(const datadog::structs::timeframe& tf) {
            return tf.interval == this->tf.interval;
        }

    private:
        std::mutex mtx;
        std::mutex aggregates_lock;
        datadog::structs::timeframe tf;
        std::unordered_map<std::string, std::vector<datadog::structs::curl_response>> db;
        std::unordered_map<std::string, std::vector<datadog::structs::aggregate>> aggregates;
        Bosma::Scheduler scheduler;
    };
}

#endif //WEBSITE_METERING_AGGREGATE_BUILDER_HPP
