#include <bitstream/omftag.h>
#include <mpeg-isobase/box/track_fragment_header.h>


namespace mpeg {
namespace isobase {


void TrackFragmentHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(track_ID)                         << "track_ID";
    if (flags & 0x000001) {
        stream.tag(base_data_offset)                 << "base_data_offset";
    }
    if (flags & 0x000002) {
        stream.tag(default_sample_description_index) << "default_sample_description_index";
    }
    if (flags & 0x000008) {
        stream.tag(default_sample_duration)          << "default_sample_duration";
    }
    if (flags & 0x000010) {
        stream.tag(default_sample_size)              << "default_sample_size";
    }
    if (flags & 0x000020) {
        stream.tag(default_sample_flags)             << "default_sample_flags";
    }
}


}} // namespace mpeg::isobase

