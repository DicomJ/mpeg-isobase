#include <bitstream/omftag.h>
#include <mpeg-isobase/box/movie_extends_header.h>


namespace mpeg {
namespace isobase {


void MovieExtendsHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    if (version==1) {
        stream.tag(fragment_duration64)  << "fragment_duration";
    } else {
        stream.tag(fragment_duration32)  << "fragment_duration";
    }
}


}} // namespace mpeg::isobase

