#include "demuxer.h"

namespace tool {

namespace media
{

class Demuxer {
public:
    Demuxer();
    ~Demuxer();

    void Open(const std::string& url);
    void Start();
    void Pause();
    void SeekTo();
    void Stop();

    float GetDuration();

private:
    std::string url_;
};

Demuxer::Demuxer() {

}

Demuxer::~Demuxer() {

}

void Demuxer::Open(const std::string& url) {
    url_ = url;
}

void Demuxer::Start() {

}

void Demuxer::Pause() {

}

void Demuxer::SeekTo() {

}

void Demuxer::Stop() {

}

float Demuxer::GetDuration() {

}

} // namespace media
}