#include <bitstream/omftag.h>
#include <mpeg-isobase/box/data_reference.h>


namespace mpeg {
namespace isobase {


void DataReferenceBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)  << "entry_count";
}


}} // namespace mpeg::isobase

