//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_DATABASE_HPP
#define WEBSITE_METERING_DATABASE_HPP

#include <vector>
#include "../structs/timeframe.hpp"
#include "../structs/curl_response.hpp"
#include "aggregate_builder.hpp"

namespace datadog::database {
    struct aggr_info {
        virtual std::vector<std::string> get_websites() = 0;
        virtual std::optional<datadog::structs::aggregate> get_last(
                std::string website,
                const datadog::structs::timeframe& tf) = 0;
    };

    class database : public aggr_info {
    public:
        void register_timeframe(const datadog::structs::timeframe& tf) {
            aggregates.emplace_back(aggregate_builder{tf});
        }

        void register_response(const datadog::structs::curl_response& response) {
            std::scoped_lock lock(mtx);
            db[response.website_name].push_back(response);

            for (auto& aggregate : aggregates) {
                aggregate.add(response);
            }
        }

        datadog::structs::aggregate get_stats(
                const std::string& website_name,
                const datadog::structs::timeframe& tf) {
            std::scoped_lock lock(mtx);
            auto& db_web = db[website_name];
            auto it = std::lower_bound(db_web.begin(), db_web.end(), std::time(nullptr) - tf.interval,
                    [] (const datadog::structs::curl_response& lhs, const datadog::structs::curl_response& rhs) {
                return lhs.timestamp < rhs.timestamp;
            });

            std::vector<datadog::structs::curl_response> cut(it, db_web.end());
            auto all = datadog::structs::aggregate{cut};

            return all;
        }

        std::vector<datadog::structs::timeframe_aggr> get_stats_aggr(
                const std::string& website_name,
                const datadog::structs::timeframe& tf) {
            std::vector<datadog::structs::timeframe_aggr> res;

            for (auto& aggr : aggregates) {
                res.push_back({aggr.get_timeframe(), aggr.get_stats(website_name, tf)});
            }

            return res;
        }

        std::vector<std::string> get_websites() {
            std::vector<std::string> res;

            for (auto& elem : db) {
                res.push_back(elem.first);
            }

            return res;
        }

        std::optional<datadog::structs::aggregate> get_last(
                std::string website,
                const datadog::structs::timeframe& tf) {
            for (auto& aggr : aggregates) {
                if (aggr.has_interval(tf)) {
                    return aggr.get_last(website);
                }
            }

            return std::nullopt;
        }

    private:
        std::mutex mtx;
        std::unordered_map<std::string, std::vector<datadog::structs::curl_response>> db;
        std::vector<aggregate_builder> aggregates;
    };
}

#endif //WEBSITE_METERING_DATABASE_HPP
