#include <bitstream/omftag.h>
#include <mpeg-isobase/box/copyright.h>


namespace mpeg {
namespace isobase {


void CopyrightBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(pad)                         << "pad";
    stream.tag(language)                    << "language" << Format("ISO-639-2/T language code");
    stream.tag(notice)                      << "notice";
}


}} // namespace mpeg::isobase

