#include <bitstream/omftag.h>
#include <mpeg-isobase/box/item_info.h>


namespace mpeg {
namespace isobase {


void ItemInfoBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    if (version == 0) {
        stream.tag(entry_count16)  << "entry_count";
    } else {
        stream.tag(entry_count32)  << "entry_count";
    }
}


}} // namespace mpeg::isobase

