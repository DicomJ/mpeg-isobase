#include <bitstream/omftag.h>
#include <mpeg-isobase/box/meta.h>


namespace mpeg {
namespace isobase {



void MetaBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
}


}} // namespace mpeg::isobase

