#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sub_track_information.h>


namespace mpeg {
namespace isobase {


void SubTrackInformation::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(switch_group)    << "switch_group";
    stream.tag(alternate_group) << "alternate_group";
    stream.tag(sub_track_ID)    << "sub_track_ID";
    stream.tag(attribute_list)  << "attribute_list";
}


}} // namespace mpeg::isobase

