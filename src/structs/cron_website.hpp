//
// Created by fiffeek on 29/10/19.
//

#ifndef WEBSITE_METERING_CRON_WEBSITE_HPP
#define WEBSITE_METERING_CRON_WEBSITE_HPP

#include <string>
#include <chrono>

namespace datadog::structs {
    struct cron_website {
        cron_website(std::string website_name, uint64_t seconds)
            : website_name(std::move(website_name)), seconds(seconds) {}

        std::string website_name;
        uint64_t seconds;
    };
}

#endif //WEBSITE_METERING_CRON_WEBSITE_HPP
