#include <bitstream/omftag.h>
#include <mpeg-isobase/box/original_format.h>


namespace mpeg {
namespace isobase {



void OriginalFormatBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    Box::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(data_format)  << "data_format";
}


}} // namespace mpeg::isobase

