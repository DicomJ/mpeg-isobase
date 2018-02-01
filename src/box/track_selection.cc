#include <bitstream/omftag.h>
#include <mpeg-isobase/box/track_selection.h>


namespace mpeg {
namespace isobase {


void TrackSelectionBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(switch_group)    << "switch_group";
    stream.tag(attribute_list)  << "attribute_list";
}


}} // namespace mpeg::isobase

