#pragma once

#include "IOSource/file_source.h"
#include "Foundation/error.h"
#include "demuxer.h"

namespace tool {

namespace media
{

class FrameExtractor {
public:
    FrameExtractor();

    ~FrameExtractor();

    std::shared_ptr<Error> Init();

    std::shared_ptr<Error> NextPacket();

    std::shared_ptr<Error> Release();

private:
    std::shared_ptr<IOSource> io_source_;
    std::unique_ptr<Demuxer> demuexer_;
};

} // namespace media
}