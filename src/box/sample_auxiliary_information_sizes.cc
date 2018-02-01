#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sample_auxiliary_information_sizes.h>


namespace mpeg {
namespace isobase {


void SampleAuxiliaryInformationSizesBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    if (flags & 1) {
        stream.tag(aux_info_type)  << "aux_info_type";
        stream.tag(aux_info_type_parameter)  << "aux_info_type_parameter";
    }
    stream.tag(default_sample_info_size)  << "default_sample_info_size";
    stream.tag(sample_count)  << "sample_count";
    if (default_sample_info_size == 0) {
        stream.tag(sample_info_size)  << "sample_info_size";
    }
}


}} // namespace mpeg::isobase

