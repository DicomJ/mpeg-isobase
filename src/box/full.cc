#include <bitstream/omftag.h>
#include <mpeg-isobase/box/full.h>


namespace mpeg {
namespace isobase {


void FullBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    Box::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(version) << "version";
    stream.tag(flags) << "flags";
}


}} // namespace mpeg::isobase

