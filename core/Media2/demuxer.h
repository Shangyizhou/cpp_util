#pragma once

#include "IOSource/file_source.h"

namespace tool {

namespace media
{

class Demuxer {
public:
    Demuxer();
    ~Demuxer();

    void Open();
    void Start();
    void Pause();
    void SeekTo();
    void Stop();

    float GetDuration();
};

} // namespace media
}