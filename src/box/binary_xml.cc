#include <bitstream/omftag.h>
#include <mpeg-isobase/box/binary_xml.h>


namespace mpeg {
namespace isobase {


void BinaryXMLBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(data)  << "data";
}


}} // namespace mpeg::isobase

