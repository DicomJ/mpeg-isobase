#include <bitstream/omftag.h>
#include <mpeg-isobase/box/movie_fragment_header.h>


namespace mpeg {
namespace isobase {


void MovieFragmentHeaderBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(sequence_number)  << "sequence_number";
}


}} // namespace mpeg::isobase

