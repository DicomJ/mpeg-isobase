#include <bitstream/omftag.h>
#include <mpeg-isobase/box/fd_item_information.h>


namespace mpeg {
namespace isobase {


void FDItemInformationBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)    << "entry_count";
}


}} // namespace mpeg::isobase

