//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_SERVER_HPP
#define WEBSITE_METERING_SERVER_HPP

#include "messages/server_init_message.hpp"
#include "parsers/input_parser.hpp"
#include "dependencies/Scheduler/Scheduler.h"
#include "database/database.hpp"
#include "database/logger.hpp"
#include <boost/log/trivial.hpp>
#include <cpr/cpr.h>
#include "database/reporter.hpp"

namespace datadog::server {
    using namespace datadog::structs;

    class server {

    private:
        void init_scheduling_for_websites() {
            for (const auto &website : parser.get_websites()) {
                scheduler.every(std::chrono::seconds(website.seconds), [&, website]() {
                    auto response = cpr::Get(cpr::Url{website.website_name},
                                             cpr::VerifySsl{false});
                    db.register_response(curl_response{response});
                });
            }
        }

    public:
        explicit server(const server_init_message &mess)
                : mess(mess)
                , parser(mess.filename)
                , scheduler(mess.max_threads)
                , db()
                , logger_timeframes{timeframes{timeframe{STATS_DISPLAY_10S}, timeframe{STATS_DISPLAY_10M}},
                                    timeframes{timeframe{STATS_DISPLAY_1M}, timeframe{STATS_DISPLAY_1H}}}
                , logger(logger_timeframes, &db)
                , reporter(timeframe{AGRR_INTERVAL_2M}, &db) {}

        void run() {
            db.register_timeframe(timeframe{AGRR_INTERVAL_2M});
            db.register_timeframe(timeframe{AGGR_INTERVAL_10M});

            init_scheduling_for_websites();

            while (true) {}
        }

    private:
        server_init_message mess;
        datadog::parsers::input_parser parser;
        Bosma::Scheduler scheduler;
        datadog::database::database db;
        std::vector<timeframes> logger_timeframes;
        datadog::database::logger logger;
        datadog::database::alerting_reporter reporter;
    };
}

#endif //WEBSITE_METERING_SERVER_HPP
