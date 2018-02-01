#include <bitstream/omftag.h>
#include <mpeg-isobase/box/track_header.h>


namespace mpeg {
namespace isobase {


void TrackHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    if (version == 1) {
        v1.output(stream_);
    } else {
        v0.output(stream_);
    }
    stream.tag(reserved2)                   << "reserved";
    stream.tag(layer)                       << "layer";
    stream.tag(alternate_group)             << "alternate_group";
    stream.tag(volume)                      << "volume" << Format("fixed-point");
    stream.tag(reserved3)                   << "reserved";
    stream.tag(matrix)                      << "matrix";
    stream.tag(width)                       << "width" << Format("fixed-point");
    stream.tag(height)                      << "height" << Format("fixed-point");
}


template <FullBox::Version version, int w>
void TrackHeaderBox::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(creation_time)       << "creation_time" << Format("time since 1904");
    stream.tag(modification_time)   << "modification_time" << Format("time since 1904");
    stream.tag(track_ID)            << "track_ID";
    stream.tag(reserved)            << "reserved";
    stream.tag(duration)            << "duration";
}

template struct TrackHeaderBox::Header<0>;
template struct TrackHeaderBox::Header<1>;


}} // namespace mpeg::isobase

