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
#include "../helpers/time_helper.hpp"
#include "../structs/agregate.hpp"

namespace datadog::database {
    class aggregate_builder {
    public:
        explicit aggregate_builder(const datadog::structs::timeframe &tf)
                : tf(tf), scheduler(datadog::MAX_THREADS) {}

        explicit aggregate_builder(const aggregate_builder &aggr)
                : tf(aggr.tf), scheduler(datadog::MAX_THREADS) {}

        void run() {
            scheduler.every(std::chrono::seconds(tf.interval), [&]() {
                std::scoped_lock lock(mtx);

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

    private:
        std::mutex mtx;
        datadog::structs::timeframe tf;
        std::unordered_map<std::string, std::vector<datadog::structs::curl_response>> db;
        std::unordered_map<std::string, std::vector<datadog::structs::aggregate>> aggregates;
        Bosma::Scheduler scheduler;
    };
}

#endif //WEBSITE_METERING_AGGREGATE_BUILDER_HPP
