#pragma once

#include "io_source.h"

namespace tool {

namespace media
{

class FileSource : public IOSource {
public:
    FileSource() = default;

    ~FileSource() {}

    bool Open(const std::string &source) override;

    void Close() override;

    bool IsOpen() const override;

    std::string GetFileName() const override;

    bool IsLocal() const override;

private:
    std::string url_;
};

bool FileSource::Open(const std::string &source) {
    url_ = source;
}

void FileSource::Close() {

}

bool FileSource::IsOpen() const {
    
}

std::string FileSource::GetFileName() const {

}

bool FileSource::IsLocal() const {
    return true;
}

} // namespace media
}