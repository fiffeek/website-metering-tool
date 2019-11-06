#ifndef SIK_ZAD2_FILE_HPP
#define SIK_ZAD2_FILE_HPP

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <string>
#include <vector>
#include "const.hpp"

namespace datadog::common {
    namespace {
        namespace fs = boost::filesystem;
    }

    class file {
    public:
        file(fs::path path)
                : file_path(path) {}

        std::vector<std::string> read_lines() {
            if (!fs::is_regular_file(file_path)) {
                throw std::runtime_error("Specified file is not a regular file");
            }

            std::ifstream infile(file_path.string());
            std::string line;
            std::vector<std::string> res;

            while (std::getline(infile, line)) {
                res.emplace_back(line);
            }

            return res;
        }

    private:
        fs::path file_path;
    };
}

#endif //SIK_ZAD2_FILE_HPP