#include <bitstream/omftag.h>
#include <mpeg-isobase/box/item_info_entry.h>


namespace mpeg {
namespace isobase {


void ItemInfoEntry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(item_ID)  << "item_ID";
    stream.tag(item_protection_index)  << "item_protection_index";
    stream.tag(item_name)  << "item_name";
    stream.tag(content_type)  << "content_type";
    stream.tag(content_encoding)  << "content_encoding";
}


}} // namespace mpeg::isobase

