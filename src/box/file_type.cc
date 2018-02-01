#include <bitstream/omftag.h>
#include <mpeg-isobase/box/file_type.h>


namespace mpeg {
namespace isobase {


void FileTypeBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    Box::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(major_brand)         << "major_brand" << Format("fourcc");
    stream.tag(minor_version)       << "minor_version";
    stream.tag(compatible_brands)   << "compatible_brands" << Format("fourcc", "array_item");
}


}} // namespace mpeg::isobase

