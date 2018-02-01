#include <bitstream/omftag.h>
#include <mpeg-isobase/box/extended_language.h>


namespace mpeg {
namespace isobase {


void ExtendedLanguageBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(extended_language)  << "extended_language";
}


}} // namespace mpeg::isobase

