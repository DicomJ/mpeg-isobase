#include <bitstream/omftag.h>
#include <mpeg-isobase/box/composition_to_decode.h>


namespace mpeg {
namespace isobase {


void CompositionToDecodeBox::output_fields(bitstream::output::meta::field::Stream &stream) const {
    FullBox::output_fields(stream);
    if (version == 0) {
        v0.output(stream);
    } else {
        v1.output(stream);
    }
}

template <FullBox::Version version, int w>
void CompositionToDecodeBox::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(compositionToDTSShift)           << "compositionToDTSShift";
    stream.tag(leastDecodeToDisplayDelta)       << "leastDecodeToDisplayDelta";
    stream.tag(greatestDecodeToDisplayDelta)    << "greatestDecodeToDisplayDelta";
    stream.tag(compositionStartTime)            << "compositionStartTime";
    stream.tag(compositionEndTime)              << "compositionEndTime";
}


}} // namespace mpeg::isobase

