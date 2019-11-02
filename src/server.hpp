//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_SERVER_HPP
#define WEBSITE_METERING_SERVER_HPP

#include "messages/server_init_message.hpp"
#include "parsers/input_parser.hpp"
#include "dependencies/Scheduler/Scheduler.h"
#include "database/database.hpp"
#include <boost/log/trivial.hpp>

namespace datadog::server {
    class server {
    private:
        void init_scheduling_for_websites() {
            for (const auto& website : parser.get_websites()) {
                scheduler.every(std::chrono::seconds(website.seconds), [&] () {
                    //  TODO FUCKING CURL HERE
                    int status = 200;
                    db.register_response();
                });
            }
        }

    public:
        server(const server_init_message& mess)
            : mess(mess), parser(mess.filename), scheduler(mess.max_threads) {}

        void run() {
            // TODO remove magical const
            db.register_timeframe(datadog::structs::timeframe{2 * 60});
            db.register_timeframe(datadog::structs::timeframe{10 * 60});

            init_scheduling_for_websites();

            while (true) {}
        }

    private:
        server_init_message mess;
        datadog::parsers::input_parser parser;
        Bosma::Scheduler scheduler;
        datadog::database::database db;
    };
}

#endif //WEBSITE_METERING_SERVER_HPP
