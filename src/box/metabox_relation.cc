#include <bitstream/omftag.h>
#include <mpeg-isobase/box/metabox_relation.h>


namespace mpeg {
namespace isobase {


void MetaboxRelationBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(first_metabox_handler_type)  << "first_metabox_handler_type";
    stream.tag(second_metabox_handler_type) << "second_metabox_handler_type";
    stream.tag(metabox_relation)            << "metabox_relation";
}


}} // namespace mpeg::isobase

