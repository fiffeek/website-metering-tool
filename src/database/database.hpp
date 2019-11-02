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
    class database {
    public:
        void register_timeframe(const datadog::structs::timeframe& tf) {
            auto aggr = aggregate_builder{tf};
            aggregates.push_back(aggr);
            aggregates[aggregates.size() - 1].run();
        }

        void register_response(const datadog::structs::curl_response& response) {
            db[response.website_name].push_back(response);

            for (auto& aggregate : aggregates) {
                aggregate.add(response);
            }
        }

        datadog::structs::aggregate get_stats(const std::string& website_name, const datadog::structs::timeframe& tf) {
            auto& db_web = db[website_name];
            auto it = std::lower_bound(db_web.begin(), db_web.end(), std::time(nullptr) - tf.interval,
                    [] (const datadog::structs::curl_response& lhs, const datadog::structs::curl_response& rhs) {
                return lhs.timestamp < rhs.timestamp;
            });

            std::vector<datadog::structs::curl_response> cut(it, db_web.end());
            auto all = datadog::structs::aggregate{cut};

            return all;
        }

        std::vector<std::string> get_websites() {
            std::vector<std::string> res;

            for (auto& elem : db) {
                res.push_back(elem.first);
            }

            return res;
        }

    private:
        std::unordered_map<std::string, std::vector<datadog::structs::curl_response>> db;
        std::vector<aggregate_builder> aggregates;
    };
}

#endif //WEBSITE_METERING_DATABASE_HPP
