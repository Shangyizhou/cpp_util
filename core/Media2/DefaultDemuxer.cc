#include "DefaultDemuxer.h"

namespace tool {
    virtual int Open(std::map<std::string, std::string> *options = nullptr, const std::shared_ptr<IOSource> &io = nullptr) = 0;
        virtual std::string file_url() const = 0;

        virtual void SetIOSourceParam(const IOSourceParam &sourceParam) = 0;

        virtual void DisposeUnderlyingResource() = 0;

        virtual IOSourceState GetIOSourceState() = 0;

        virtual void set_fast_parse(bool fast_parse) = 0;

        virtual const AVFormatContext *fmt_ctx() const = 0;

        virtual const AVStream *video_stream() const = 0;

        virtual const AVStream *audio_stream() const = 0;

        virtual const AVStream *data_stream() const = 0;

        virtual bool read_metadata(const std::string &key, std::string *value) const = 0;

        virtual std::map<std::string, std::string> read_all_metadata() const = 0;

        virtual const AVStream *StreamAtIndex(unsigned int index) const = 0;

        virtual const AVStream *StreamOfType(AVMediaType type) const = 0;

        virtual int64_t video_duration() const = 0;

        virtual int64_t audio_duration() const = 0;

        virtual int64_t duration() const = 0;

        virtual int64_t bitrate() const = 0;

        virtual int video_stream_index() const = 0;

        virtual int audio_stream_index() const = 0;

        virtual int subtitle_stream_index() const = 0;

        virtual int data_stream_index() const = 0;

        virtual bool get_extra_media_data(std::vector<uint8_t> &extraMediaData) const = 0;

        virtual int nb_streams() const = 0;

        virtual int NextPacket(std::shared_ptr<AVPacket> &pkt) = 0;

        virtual int Seek(int stream_index, int64_t timestamp, int flags) = 0;

        virtual void Interrupt() = 0;

        virtual void FlushIO() = 0;

        virtual void Close() = 0;
} 