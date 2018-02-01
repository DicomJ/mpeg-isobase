#include <bitstream/omftag.h>
#include <mpeg-isobase/box/track_extends.h>


namespace mpeg {
namespace isobase {


void TrackExtendsBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(track_ID)                         << "track_ID";
    stream.tag(default_sample_description_index) << "default_sample_description_index";
    stream.tag(default_sample_duration)          << "default_sample_duration";
    stream.tag(default_sample_size)              << "default_sample_size";
    stream.tag(default_sample_flags)             << "default_sample_flags";
}


}} // namespace mpeg::isobase

