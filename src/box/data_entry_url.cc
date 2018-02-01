#include <bitstream/omftag.h>
#include <mpeg-isobase/box/data_entry_url.h>


namespace mpeg {
namespace isobase {



void DataEntryUrlBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(location)  << "location";
}


}} // namespace mpeg::isobase

