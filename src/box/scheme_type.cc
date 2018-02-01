#include <bitstream/omftag.h>
#include <mpeg-isobase/box/scheme_type.h>


namespace mpeg {
namespace isobase {


void SchemeTypeBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(scheme_type)  << "scheme_type" << Format("fourcc");
    stream.tag(scheme_version)  << "scheme_version";
    stream.tag(scheme_uri)  << "scheme_uri";
    if (flags & 0x000001) {
        stream.tag(scheme_version)  << "scheme_version";
    }
}


}} // namespace mpeg::isobase

