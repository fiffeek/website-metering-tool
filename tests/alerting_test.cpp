//
// Created by fiffeek on 03.11.19.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch.hpp>
#include "../src/dependencies/FakeIt/single_header/catch/fakeit.hpp"
#include "../src/database/database.hpp"
#include "../src/database/reporter.hpp"
#include <string>
#include <iostream>
#include <sstream>

namespace {
    const char *website = "www.samoyedsarecool.com";
    constexpr double half_available = 0.5;
    constexpr double mostly_available = 0.9;
    const datadog::structs::timeframe tf{10};
}


TEST_CASE("vectors can be sized and resized", "[vector]") {
    using namespace fakeit;
    using Catch::Matchers::StartsWith;

    Mock<datadog::database::aggr_info> db_mock;
    When(Method(db_mock, get_websites)).AlwaysReturn(std::vector<std::string>{website});

    SECTION("write alert for availability less than 80%") {
        When(Method(db_mock, get_last)).Return(std::optional<datadog::structs::aggregate>{half_available});
        auto &db = db_mock.get();
        std::stringstream buffer;
        std::streambuf *prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());
        auto rep = datadog::database::alerting_reporter{tf, &db};

        rep.traverse_websites();
        std::string text = buffer.str();
        std::cout.rdbuf(prevcoutbuf);

        REQUIRE_THAT(text, StartsWith("Website [" + std::string{website} + "] is down. availability=[" +
                                      std::to_string(half_available) + "]"));
    }

    SECTION("write alert for recovering availability") {
        When(Method(db_mock, get_last))
                .Return(std::optional<datadog::structs::aggregate>{half_available})
                .Return(std::optional<datadog::structs::aggregate>{mostly_available});
        auto &db = db_mock.get();
        auto rep = datadog::database::alerting_reporter{tf, &db};
        rep.traverse_websites();

        std::stringstream buffer;
        std::streambuf *prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());
        rep.traverse_websites();
        std::string text = buffer.str();
        std::cout.rdbuf(prevcoutbuf);

        REQUIRE_THAT(text, StartsWith("Website [" + std::string{website} + "] is up. availability=[" +
                                      std::to_string(mostly_available) + "]"));
    }
}