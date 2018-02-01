#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sample_group_description.h>


namespace mpeg {
namespace isobase {


void SampleGroupDescriptionBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(grouping_type)  << "grouping_type";
    stream.tag(entry_count)  << "entry_count";
}


}} // namespace mpeg::isobase

