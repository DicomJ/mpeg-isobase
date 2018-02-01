#include <bitstream/omftag.h>
#include <mpeg-isobase/box/primary_item.h>


namespace mpeg {
namespace isobase {


void PrimaryItemBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    if (version == 0) {
        v0.output(stream_);
    } else {
        v1.output(stream_);
    }
}

template <FullBox::Version version, int w>
void PrimaryItemBox::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(item_ID) << "item_ID";
}

template struct PrimaryItemBox::Header<0>;
template struct PrimaryItemBox::Header<1>;


}} // namespace mpeg::isobase

