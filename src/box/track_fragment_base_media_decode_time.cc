#include <bitstream/omftag.h>
#include <mpeg-isobase/box/track_fragment_base_media_decode_time.h>


namespace mpeg {
namespace isobase {


template <FullBox::Version version, int w>
void TrackFragmentBaseMediaDecodeTimeBox::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(baseMediaDecodeTime)              << "baseMediaDecodeTime";
}

template struct TrackFragmentBaseMediaDecodeTimeBox::Header<0>;
template struct TrackFragmentBaseMediaDecodeTimeBox::Header<1>;


void TrackFragmentBaseMediaDecodeTimeBox::output_fields(bitstream::output::meta::field::Stream &stream) const {
    FullBox::output_fields(stream);
    if (version == 1) {
        v1.output(stream);
    } else {
        v0.output(stream);
    }
}

}} // namespace mpeg::isobase

