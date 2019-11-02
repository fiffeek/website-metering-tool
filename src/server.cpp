#include <boost/program_options.hpp>
#include <iostream>
#include <cstdint>
#include <optional>
#include <csignal>
#include "const.hpp"
#include "messages/server_init_message.hpp"
#include "server.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
            (",f", po::value<std::string>()->required(), "INPUT_FILEPATH")
            (",t", po::value<unsigned int>()->default_value(datadog::MAX_THREADS), "MAX_THREADS")
            ("help", "HELP");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch(po::error &e) {
        std::cerr << e.what() << std::endl;
        return datadog::ERROR;
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return datadog::SUCCESS;
    }

    try {
        datadog::server::server_init_message msg{
                vm["-f"].as<std::string>(),
                vm["-t"].as<unsigned int>()
        };

        datadog::server::server server{msg};
        server.run();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
        return datadog::ERROR;
    }

    return datadog::SUCCESS;
}