#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sample_scale.h>


namespace mpeg {
namespace isobase {


void SampleScaleBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(reserved)  << "reserved";
    stream.tag(constraint_flag)  << "constraint_flag";
    stream.tag(scale_method)  << "scale_method";
    stream.tag(display_center_x)  << "display_center_x";
    stream.tag(display_center_y)  << "display_center_y";
}


}} // namespace mpeg::isobase

