#include <bitstream/omftag.h>
#include <mpeg-isobase/box/media_header.h>


namespace mpeg {
namespace isobase {


void MediaHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    if (version == 1) {
        v1.output(stream_);
    } else {
        v0.output(stream_);
    }
    stream.tag(pad)                         << "pad";
    stream.tag(language)                    << "language" << Format("ISO-639-2/T language code");
    stream.tag(pre_defined)                 << "pre_defined";
}

template <FullBox::Version version, int w>
void MediaHeaderBox::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(creation_time)        << "creation_time" << Format("time since 1904");
    stream.tag(modification_time)    << "modification_time" << Format("time since 1904");
    stream.tag(timescale)            << "timescale";
    stream.tag(duration)             << "duration";
}

template struct MediaHeaderBox::Header<0>;
template struct MediaHeaderBox::Header<1>;


}} // namespace mpeg::isobase

