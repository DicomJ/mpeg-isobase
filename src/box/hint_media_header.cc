#include <bitstream/omftag.h>
#include <mpeg-isobase/box/hint_media_header.h>


namespace mpeg {
namespace isobase {



void HintMediaHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(maxPDUsize)  << "maxPDUsize";
    stream.tag(avgPDUsize)  << "avgPDUsize";
    stream.tag(maxbitrate)  << "maxbitrate";
    stream.tag(avgbitrate)  << "avgbitrate";
    stream.tag(reserved)    << "reserved";
}


}} // namespace mpeg::isobase

