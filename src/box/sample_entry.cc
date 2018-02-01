#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sample_entry.h>


namespace mpeg {
namespace isobase {


void SampleEntry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    Box::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(reserved)  << "reserved";
    stream.tag(data_reference_index)  << "data_reference_index";
}


}} // namespace mpeg::isobase

