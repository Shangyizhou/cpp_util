#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

#include "demuxer.h"

namespace tool {

class Extractor {
public:
    struct ExtractConfig {
        std::string input_file;
        std::string output_dir;
        std::vector<std::string> output_files;
        std::function<void(const std::string &)> progress_callback;
    };
    
    Extractor(const std::string &filename, const std::string &output_dir);

private:
    std::unique_ptr<Demuxer> demuxer_;
};

}
