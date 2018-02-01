#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sound_media_header.h>


namespace mpeg {
namespace isobase {


void SoundMediaHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(balance)     << "balance" << Format("fixed-point");
    stream.tag(reserved)    << "reserved";
}


}} // namespace mpeg::isobase

