#include <bitstream/omftag.h>
#include <mpeg-isobase/box/item_protection.h>


namespace mpeg {
namespace isobase {



void ItemProtectionBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(protection_count)  << "protection_count";
}


}} // namespace mpeg::isobase

