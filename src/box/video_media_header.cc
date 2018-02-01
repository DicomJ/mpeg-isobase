#include <bitstream/omftag.h>
#include <mpeg-isobase/box/video_media_header.h>


namespace mpeg {
namespace isobase {


void VideoMediaHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(graphicsmode)    << "graphicsmode";
    stream.tag(opcolor)         << "opcolor";
}


}} // namespace mpeg::isobase

