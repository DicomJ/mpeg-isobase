#include <bitstream/omftag.h>
#include <mpeg-isobase/box/data_entry_urn.h>


namespace mpeg {
namespace isobase {


void DataEntryUrnBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(name)  << "name";
    stream.tag(location)  << "location";
}


}} // namespace mpeg::isobase

