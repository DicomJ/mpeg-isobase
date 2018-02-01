#include <bitstream/omftag.h>
#include <mpeg-isobase/box/movie_header.h>


namespace mpeg {
namespace isobase {


void MovieHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    if (version == 1) {
        v1.output(stream_);
    } else {
        v0.output(stream_);
    }
    stream.tag(rate)                        << "rate" << Format("fixed-point");
    stream.tag(volume)                      << "volume" << Format("fixed-point");
    stream.tag(reserved)                    << "reserved";
    stream.tag(reserved2)                   << "reserved";
    stream.tag(matrix)                      << "matrix";
    stream.tag(pre_defined)                 << "pre_defined";
    stream.tag(next_track_ID)               << "next_track_ID";
}


template <FullBox::Version version, int w>
void MovieHeaderBox::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(creation_time)       << "creation_time" << Format("time since 1904");
    stream.tag(modification_time)   << "modification_time" << Format("time since 1904");
    stream.tag(timescale)           << "timescale";
    stream.tag(duration)            << "duration";
}

template struct MovieHeaderBox::Header<0>;
template struct MovieHeaderBox::Header<1>;


}} // namespace mpeg::isobase

