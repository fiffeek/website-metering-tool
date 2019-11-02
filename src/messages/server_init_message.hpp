//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_SERVER_INIT_MESSAGE_HPP
#define WEBSITE_METERING_SERVER_INIT_MESSAGE_HPP

#include <string>

namespace datadog::server {
    struct server_init_message {
        std::string filename;
        unsigned int max_threads;
    };
}

#endif //WEBSITE_METERING_SERVER_INIT_MESSAGE_HPP
