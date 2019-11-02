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
            aggregates.emplace_back(aggregate_builder{tf});
            aggregates[aggregates.size() - 1].run();
        }

        void register_response(const datadog::structs::curl_response& response) {
            for (auto& aggregate : aggregates) {
                aggregate.add(response);
            }
        }

    private:
        std::vector<aggregate_builder> aggregates;
    };
}

#endif //WEBSITE_METERING_DATABASE_HPP
