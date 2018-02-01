#include <bitstream/omftag.h>
#include <mpeg-isobase/box/item_location.h>


namespace mpeg {
namespace isobase {



void ItemLocationBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(item_count)  << "item_count";
    stream.tag(length_size)  << "length_size";
    stream.tag(base_offset_size)  << "base_offset_size";
    if (version == 1 || version == 2) {
        stream.tag(index_size)  << "index_size";
    } else {
        stream.tag(reserved)  << "reserved";
    }
    stream.tag(item_count)  << "item_count";
}


}} // namespace mpeg::isobase

